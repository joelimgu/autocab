import rclpy
from std_msgs.msg import Bool
import asyncio
import websockets


start_status = False
prev_start_status = None  # Variable pour stocker la valeur précédente de start_status


def start_status_callback(msg):
    global start_status, prev_start_status
    start_status = msg.data


'''
async def send_message():
    global start_status
    uri = "ws://127.0.0.1:5501"
    async with websockets.connect(uri) as websocket:
        try:
            while True:
                message = str(start_status)
                await websocket.send(message)
                print(f"Sent message: {message}")

        except websockets.exceptions.ConnectionClosedError as e:
            print(f"Connexion fermée de manière inattendue. Erreur : {e}")

        except websockets.exceptions.ConnectionClosedOK:
            print("Connexion fermée par le serveur.")

asyncio.run(send_message())
'''


def main():
    rclpy.init()
    node = rclpy.create_node('start_status_subscriber')

    # Crée un objet Subscriber pour le topic "start_status" avec le type de message Bool
    subscriber = node.create_subscription(Bool, 'start_status', start_status_callback, 10)

    print("Waiting for messages. Press Ctrl+C to exit.")
    if start_status_callback(msg):
        # Vérifier si start_status a changé
        if start_status != prev_start_status:
            prev_start_status = start_status
            print(f"Received start status: {start_status}")
    
    rclpy.spin(node)

    # Arrêtez correctement le nœud
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()