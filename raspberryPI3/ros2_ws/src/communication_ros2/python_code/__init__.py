#!/usr/bin/env python3

import json
import threading
import time
import rclpy
from std_msgs.msg import Bool
import asyncio
import websockets
from interfaces.msg import Toserveur


from interfaces.msg import Serveur

start_status = False
prev_start_status = None  # Variable pour stocker la valeur précédente de start_status
uri = "ws://autocab.joel.rs:5501"
ws_manager: 'WebSocketManager' = None
node = None


def start_status_callback(msg):
    global start_status, prev_start_status
    start_status = msg.data

    # Vérifier si start_status a changé
    if start_status != prev_start_status:
        prev_start_status = start_status
        print(f"Received start status: {start_status}")
        dict = {
            "type": "status",
            "data": {
                "current_latitude": msg.current_latitude,
                "current_longitude": msg.current_longitude,
                "mode": msg.mode,
                "arrived": msg.arrived,
                "on": msg.on
            }
        }
        send_message(json.dumps(dict))  # Utiliser 'await' pour attendre la fin de send_message


def send_message(msg):
    global node, ws_manager
    if node is None:
        return None
    if ws_manager is None:
        node.get_logger().error('unable to send message websocket manager not initialized')
        return None
    node.get_logger().info(f'sending message: {msg}')
    ws_manager.messages.append(msg)


def to_serveur_callback(msg):
    dict = {
        "type": "toServeur",
        "data": {
            "current_latitude": msg.current_latitude,
            "current_longitude": msg.current_longitude,
            "mode": msg.mode,
            "arrived": msg.arrived,
            "on": msg.on
        }
    }
    send_message(json.dumps(dict))


def init_manager():
    global ws_manager
    ws_manager = WebSocketManager()


def main():
    global node, ws_manager
    rclpy.init()
    node = rclpy.create_node('start_status_subscriber')
    node.get_logger().info('websocket node started')
    # ws_manager = WebSocketManager()
    threading.Thread(target=init_manager).start()
    node.get_logger().info('websocket manager started')

    # Crée un objet Subscriber pour le topic "start_status" avec le type de message Bool
    node.create_subscription(Bool, 'start_status', start_status_callback, 10)
    node.create_subscription(Toserveur, 'to_serveur', to_serveur_callback, 10)

    try:
        while rclpy.ok():
            rclpy.spin_once(node)
            time.sleep(0.1)
            # await asyncio.sleep(0.1)  # Add a small sleep to release control to the event loop
    except KeyboardInterrupt:
        pass
    finally:
        # Arrêtez correctement le nœud
        node.destroy_node()
        rclpy.shutdown()


# thanks to https://stackoverflow.com/questions/71871037/python-async-callback-receiver-for-websocket
class WebSocketManager:

    def __init__(self):
        global ws_manager
        self.websocket = None
        self.messages: list[str] = []
        self.order: Serveur = None
        self.request_number = 1
        ws_manager = self
        self.server_pub = node.create_publisher(Serveur, "serveur_data", 10)
        asyncio.run(self.start())

    def route_to_ros(self, msg):
        global node
        if node is None:
            return None
        try:
            msg = json.loads(msg)
            if msg["type"] == "objective":
                msg["data"]["departure_point"] = ord(msg["data"]["departure_point"])
                msg["data"]["final_point"] = ord(msg["data"]["final_point"])
                #self.order = Serveur(**msg["data"])
                new_order = Serveur(**msg["data"])
                new_order.request_number = self.request_number
                if new_order == self.order:
                    return
                self.request_number += 1
                self.order = new_order
                #self.server_pub.publish(Serveur(**msg["data"]))
        except json.decoder.JSONDecodeError:
            return

    async def start(self):
        connexion = asyncio.create_task(self.connexion())
        sender = asyncio.create_task(self.send_message())
        loop = asyncio.create_task(self.send_route_loop())
        await asyncio.gather(connexion, sender, loop)

    async def send_route_loop(self):
        global node
        while True:
            await asyncio.sleep(0.5)
            if node is None:
                continue
            if self.order is None:
                continue
            self.server_pub.publish(self.order)

    async def connexion(self):
        global node
        node.get_logger().info('connexion')
        async for websocket in websockets.connect(uri):
            node.get_logger().info('connecting websocket')
            try:
                self.websocket = websocket
                async for message in websocket:
                    node.get_logger().info(f'recv websocket: ${message}')
                    self.route_to_ros(message)
            except websockets.ConnectionClosed:
                self.websocket = None
                continue

    async def send_message(self):
        while True:
            if self.websocket is None:
                await asyncio.sleep(2)
            try:
                for message in self.messages:
                    await self.websocket.send(message)
                    self.websocket.pop(0)

            except Exception as e:
                print(f"An unexpected error occurred: {e}")
            await asyncio.sleep(0.1)


if __name__ == '__main__':
    main()
