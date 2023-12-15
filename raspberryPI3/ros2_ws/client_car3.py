import rclpy
from std_msgs.msg import Bool
import asyncio
import websockets


start_status = False


def send_message(start_status):
    uri = "ws://127.0.0.1:5501"
    try:
        websocket = websockets.connect(uri)
        while True:
            message = str(start_status)
            websocket.send(message)
            print(f"Sent message: {message}")

    except websockets.exceptions.ConnectionClosedError as e:
        print(f"Connexion fermée de manière inattendue. Erreur : {e}")

    except websockets.exceptions.ConnectionClosedOK:
        print("Connexion fermée par le serveur.")


def start_status_callback(msg):
    global start_status
    start_status = msg.data
    print(f"Received start status: {start_status}")
    send_message(start_status)



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