FROM arm64v8/ros:humble-ros-base
RUN apt-get update
RUN apt-get install -y openssh-server net-tools nano vim apt-utils \
  ros-humble-navigation2 \
  ros-humble-nav2-bringup \
  v4l-utils \
  python3-rosdep

WORKDIR /root/
COPY jetsonNano/.bashrc /root/.bashrc
COPY jetsonNano/ros2_ws ./ros2_ws

WORKDIR /root/ros2_ws
RUN rosdep update
RUN rosdep install --from-paths src --ignore-src -y

RUN . /opt/ros/humble/setup.sh && colcon build
