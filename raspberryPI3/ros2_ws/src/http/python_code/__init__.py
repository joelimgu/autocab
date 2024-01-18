import json

import rclpy
import rclpy
from std_msgs.msg import Bool
import asyncio
import websockets
from interfaces.msg import Toserveur

start_status = False
prev_start_status = None  # Variable pour stocker la valeur précédente de start_status
uri = "ws://127.0.0.1:5501"
ws_manager = None
node = None

async def start_status_callback(msg):
    global start_status, prev_start_status
    start_status = msg.data

    # Vérifier si start_status a changé
    if start_status != prev_start_status:
        prev_start_status = start_status
        print(f"Received start status: {start_status}")
        dict = {
            "type": "status",
            "data": msg
        }
        await send_message(dict)  # Utiliser 'await' pour attendre la fin de send_message


async def send_message(msg):
    global node
    if node is None:
        print("Node not initialized")
        return None

    uri = "ws://127.0.0.1:5501"
    try:
        async with websockets.connect(uri) as websocket:
            message = str(msg)
            await websocket.send(message)
            node.get_logger().info('This is an information message')
            print(f"Sent message: {message}")
    except websockets.exceptions.ConnectionClosedError as e:
        print(f"Connexion fermée de manière inattendue. Erreur : {e}")
    except websockets.exceptions.ConnectionClosedOK:
        print("Connexion fermée par le serveur.")
    except Exception as e:
        print(f"An unexpected error occurred: {e}")


async def to_serveur_callback(msg):
    dict = {
        "type": "toServeur",
        "data": msg
    }
    await send_message(json.dump(dict))


async def main():
    global node, ws_manager
    rclpy.init()
    node = rclpy.create_node('start_status_subscriber')
    ws_manager = WebSocketManager()

    # Crée un objet Subscriber pour le topic "start_status" avec le type de message Bool
    subscriber = node.create_subscription(Bool, 'start_status', start_status_callback, 10)
    node.create_subscription(Toserveur, 'to_serveur', to_serveur_callback, 10)

    print("Waiting for messages. Press Ctrl+C to exit.")

    try:
        while rclpy.ok():
            rclpy.spin_once(node)
            await asyncio.sleep(0.1)  # Add a small sleep to release control to the event loop
    except KeyboardInterrupt:
        pass
    finally:
        # Arrêtez correctement le nœud
        node.destroy_node()
        rclpy.shutdown()


# thanks to https://stackoverflow.com/questions/71871037/python-async-callback-receiver-for-websocket
class WebSocketManager:
    def __init__(self):
        self.websocket = None
        self.messages: list[str] = []
        asyncio.run(self.start())

    async def start(self):
        connexion = asyncio.create_task(self.connexion())
        sender = asyncio.create_task(self.sed_message())
        await asyncio.gather(connexion, sender)

    async def connexion(self):
        async for websocket in websockets.connect(uri):
            try:
                self.websocket = websocket
                async for message in websocket:
                    print("recv websocket: ", message)
            except websockets.ConnectionClosed:
                self.websocket = None
                continue

    async def sed_message(self):
        if self.websocket is None:
            await asyncio.sleep(2)
        try:
            for message in self.messages:
                await self.websocket.send(message)
        except Exception as e:
            print(f"An unexpected error occurred: {e}")
        await asyncio.sleep(0.1)





if __name__ == '__main__':
    asyncio.run(main())
