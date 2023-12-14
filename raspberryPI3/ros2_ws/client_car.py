import rclpy
from std_msgs.msg import Bool
import asyncio
import websockets

async def send_message(websocket, start_status):
    try:
        message = str(start_status)
        await websocket.send(message)
        print(f"Sent message: {message}")
    except websockets.exceptions.ConnectionClosedError as e:
        print(f"Connexion fermée de manière inattendue. Erreur : {e}")
    except websockets.exceptions.ConnectionClosedOK:
        print("Connexion fermée par le serveur.")

async def start_status_callback(msg, websocket):
    start_status = msg.data
    print(f"Received start status: {start_status}")

    # Envoyer la variable start_status via WebSocket
    await send_message(websocket, start_status)

def main():
    rclpy.init()

    node = rclpy.create_node('start_status_subscriber')
    executor = rclpy.executors.SingleThreadedExecutor()

    uri = "ws://127.0.0.1:5501"
    try:
        websocket = asyncio.get_event_loop().run_until_complete(websockets.connect(uri))

        # Utilisez une boucle asyncio distincte pour gérer la communication WebSocket
        asyncio.ensure_future(send_message(websocket, False))

        # Créez l'objet Subscriber après avoir établi la connexion WebSocket
        subscriber = node.create_subscription(Bool, 'start_status', lambda msg: start_status_callback(msg, websocket), 10)

        while rclpy.ok():
            try:
                executor.spin_once(node, timeout_sec=0.1)
            except KeyboardInterrupt:
                break

    finally:
        if 'websocket' in locals():
            asyncio.get_event_loop().run_until_complete(websocket.close())

        # Arrêtez correctement le nœud
        node.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()







'''

import rclpy
from std_msgs.msg import Bool

def start_status_callback(msg):
    start_status = msg.data
    print(f"Received start status: {start_status}")

def main():
    rclpy.init()
    node = rclpy.create_node('start_status_subscriber')

    # Crée un objet Subscriber pour le topic "start_status" avec le type de message Bool
    subscriber = node.create_subscription(Bool, 'start_status', start_status_callback, 10)

    print("Waiting for messages. Press Ctrl+C to exit.")
    rclpy.spin(node)

    # Arrêtez correctement le nœud
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()

'''