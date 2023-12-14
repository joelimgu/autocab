import rclpy
from std_msgs.msg import Bool
import threading
import websockets

start_status = False

def start_status_callback(msg):
    global start_status
    start_status = msg.data
    print(f"Received start status: {start_status}")

def send_message():
    uri = "ws://127.0.0.1:5501"
    while True:
        message = input("Entrez votre message (ou 'exit' pour quitter) : ")
        if message.lower() == 'exit':
            break

        try:
            with websockets.connect(uri) as websocket:
                websocket.send(message)
                print(f"Sent message: {message}")
        except websockets.exceptions.ConnectionClosedError as e:
            print(f"Connexion fermée de manière inattendue. Erreur : {e}")
        except websockets.exceptions.ConnectionClosedOK:
            print("Connexion fermée par le serveur.")

def main():
    rclpy.init()
    node = rclpy.create_node('start_status_subscriber')

    subscriber = node.create_subscription(Bool, 'start_status', start_status_callback, 10)

    print("Waiting for messages. Press Ctrl+C to exit.")

    # Démarrer la fonction send_message dans un thread séparé
    message_thread = threading.Thread(target=send_message)
    message_thread.start()

    rclpy.spin(node)

    # Attendre que le thread de message se termine avant de détruire le nœud
    message_thread.join()

    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()

