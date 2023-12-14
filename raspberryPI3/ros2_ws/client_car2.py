import rclpy
from std_msgs.msg import Bool
import asyncio
import websockets

start_status = False
prev_start_status = None  # Variable pour stocker la valeur précédente de start_status

def start_status_callback(msg):
    global start_status, prev_start_status
    start_status = msg.data
    print(f"Received start status: {start_status}")

    # Vérifier si start_status a changé
    if start_status != prev_start_status:
        prev_start_status = start_status
        node.create_task(send_message())  # Utiliser la boucle d'événements de ROS 2

async def send_message():
    global start_status
    uri = "ws://127.0.0.1:5501"
    async with websockets.connect(uri) as websocket:
        try:
            message = str(start_status)  # Convertir start_status en chaîne avant de l'envoyer
            await websocket.send(message)
            print(f"Sent message: {message}")

        except websockets.exceptions.ConnectionClosedError as e:
            print(f"Connexion fermée de manière inattendue. Erreur : {e}")

        except websockets.exceptions.ConnectionClosedOK:
            print("Connexion fermée par le serveur.")

def main():
    global start_status
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