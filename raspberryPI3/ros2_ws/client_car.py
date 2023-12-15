import rclpy
from std_msgs.msg import Bool
import asyncio
import websockets

async def ros_callback(msg):
    global start_status
    start_status = msg.data

async def ros_listener():
    rclpy.init()
    node = rclpy.create_node('ros_listener_node')
    subscription = node.create_subscription(Bool, 'start_status', ros_callback, 10)

    while rclpy.ok():
        rclpy.spin_once(node)

async def send_to_websocket():
    uri = "ws://127.0.0.1:5501"
    async with websockets.connect(uri) as websocket:
        global start_status
        while True:
            await asyncio.sleep(0.1)  # Adjust the delay as needed
            if start_status is not None:
                message = str(start_status)
                await websocket.send(message)
                print(f"Sent message to WebSocket server: {message}")
                start_status = None

if __name__ == "__main__":
    start_status = None

    try:
        loop = asyncio.get_event_loop()
        ros_listener_task = loop.create_task(ros_listener())
        websocket_task = loop.create_task(send_to_websocket())
        loop.run_until_complete(asyncio.gather(ros_listener_task, websocket_task))
    except KeyboardInterrupt:
        pass
    finally:
        rclpy.shutdown()

