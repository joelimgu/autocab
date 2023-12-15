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
        await send_message(node)  # Utiliser 'await' pour attendre la fin de send_message


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