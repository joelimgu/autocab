import rclpy
from std_msgs.msg import Bool
import asyncio
import websockets



def start_status_callback(msg):
    global start_status
    start_status = msg.data
    print(f"Received start status: {start_status}")

async def send_message():
    uri = "ws://127.0.0.1:5501"
    async with websockets.connect(uri) as websocket:
        try:
            while True:
                '''
                message = input("Entrez votre message (ou 'exit' pour quitter) : ")
                if message.lower() == 'exit':
                    break
'''
                await websocket.send(message)
                print(f"Sent message: {message}")
        except websockets.exceptions.ConnectionClosedError as e:
            print(f"Connexion fermée de manière inattendue. Erreur : {e}")

        except websockets.exceptions.ConnectionClosedOK:
            print("Connexion fermée par le serveur.")
asyncio.run(send_message())

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
import rclpy
from std_msgs.msg import Bool
import asyncio
import websockets



def start_status_callback(msg):
    global start_status
    start_status = msg.data
    print(f"Received start status: {start_status}")

async def send_message():
    uri = "ws://127.0.0.1:5501"
    async with websockets.connect(uri) as websocket:
        try:
            while True:
                message = input("Entrez votre message (ou 'exit' pour quitter) : ")
                if message.lower() == 'exit':
                    break

                await websocket.send(message)
                print(f"Sent message: {message}")

        except websockets.exceptions.ConnectionClosedError as e:
            print(f"Connexion fermée de manière inattendue. Erreur : {e}")

        except websockets.exceptions.ConnectionClosedOK:
            print("Connexion fermée par le serveur.")

asyncio.run(send_message())

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