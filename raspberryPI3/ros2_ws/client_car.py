import rclpy
from std_msgs.msg import Bool
import asyncio
import websockets

async def start_status_callback(msg):
    global start_status
    start_status = msg.data
    print(f"Received start status: {start_status}")

async def ros2_websocket_client():
    global start_status

    # Initialiser le nœud ROS 2
    rclpy.init()
    node = rclpy.create_node('ros2_websocket_client')

    # Créer un objet Subscriber pour le topic "start_status" avec le type de message Bool
    subscriber = node.create_subscription(Bool, 'start_status', start_status_callback, 10)

    # Adresse du serveur WebSocket
    uri = "ws://127.0.0.1:5501"

    try:
        # Se connecter au serveur WebSocket
        async with websockets.connect(uri) as websocket:
            print(f"Connected to WebSocket server at {uri}")

            while rclpy.ok():
                # Envoyer la variable start_status au serveur WebSocket
                await websocket.send(str(start_status))
                print(f"Sent message to WebSocket server: {start_status}")

                # Attendre un certain temps avant d'envoyer la prochaine mise à jour
                await asyncio.sleep(1)
    except websockets.exceptions.ConnectionClosedError as e:
        print(f"Connection closed unexpectedly. Error: {e}")
    except KeyboardInterrupt:
        pass
    finally:
        # Arrêter correctement le nœud ROS 2
        node.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    asyncio.run(ros2_websocket_client())



'''
import rclpy
from std_msgs.msg import Bool
import asyncio
import websockets
import threading

start_status = False  # Variable partagée
start_status_lock = threading.Lock()  # Utilisation de threading.Lock

def send_message(websocket):
    global start_status
    with start_status_lock:
        try:
            # Convertir le booléen en chaîne de caractères avant l'envoi
            message = str(start_status)
            asyncio.ensure_future(websocket.send(message))
            print(f"Sent message: {message}")
        except websockets.exceptions.ConnectionClosedError as e:
            print(f"Connexion fermée de manière inattendue. Erreur : {e}")
        except websockets.exceptions.ConnectionClosedOK:
            print("Connexion fermée par le serveur.")


def start_status_callback(msg):
    global start_status
    with start_status_lock:
        start_status = msg.data
        print(f"Received start status: {start_status}")

async def main():
    global start_status

    rclpy.init()
    node = rclpy.create_node('start_status_subscriber')

    # Crée un objet Subscriber pour le topic "start_status" avec le type de message Bool
    subscriber = node.create_subscription(Bool, 'start_status', start_status_callback, 10)

    uri = "ws://127.0.0.1:5501"
    try:
        websocket = await websockets.connect(uri)

        # Attendez que le nœud ROS publie au moins un message
        #while not start_status:
            #await asyncio.sleep(0.1)

        while rclpy.ok():
            try:
                await asyncio.sleep(0.1)
                send_message(websocket)
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