import rclpy
from std_msgs.msg import Bool
import asyncio
import websockets

start_status = False  # Variable partagée
start_status_lock = asyncio.Lock()  # Verrou pour synchroniser l'accès à la variable

async def send_message(websocket):
    global start_status
    async with start_status_lock:
        try:
            message = str(start_status)
            await websocket.send(message)
            print(f"Sent message: {message}")
        except websockets.exceptions.ConnectionClosedError as e:
            print(f"Connexion fermée de manière inattendue. Erreur : {e}")
        except websockets.exceptions.ConnectionClosedOK:
            print("Connexion fermée par le serveur.")

async def start_status_callback(msg):
    global start_status
    async with start_status_lock:
        start_status = msg.data
        print(f"Received start status: {start_status}")

def main():
    global start_status

    rclpy.init()
    node = rclpy.create_node('start_status_subscriber')

    # Crée un objet Subscriber pour le topic "start_status" avec le type de message Bool
    subscriber = node.create_subscription(Bool, 'start_status', start_status_callback, 10)

    executor = rclpy.executors.SingleThreadedExecutor()
    executor.add_node(node)

    uri = "ws://127.0.0.1:5501"
    try:
        websocket = await websockets.connect(uri)

        # Attendez que le nœud ROS publie au moins un message
        while not start_status:
            await asyncio.sleep(0.1)

        while rclpy.ok():
            try:
                await asyncio.sleep(0.1)  # Peut être nécessaire pour éviter un blocage
                await send_message(websocket)
                executor.spin_once(timeout_sec=0.1)
            except KeyboardInterrupt:
                break

    except KeyboardInterrupt:
        pass
    finally:
        if 'websocket' in locals():
            await websocket.close()

        # Arrêtez correctement le nœud
        node.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    asyncio.run(main())









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