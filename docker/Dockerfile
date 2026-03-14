FROM ros:humble-ros-base-jammy

RUN echo "source /opt/ros/humble/setup.bash" >> ~/.bashrc

# https://askubuntu.com/a/1430072/1008445
ENV DEBIAN_FRONTEND=noninteractive

ENV ROS_DISTRO=${ROS_DISTRO}
ENV LD_LIBRARY_PATH=/usr/lib/x86_64-linux-gnu:${LD_LIBRARY_PATH}

# Update and install dependencies
RUN apt-get update && apt-get install -y \
    git git-core \
    bash-completion \
    # SSH with agent forwarding
    openssh-client \
    software-properties-common \
    # https://stackoverflow.com/a/68666500/6010333
    libgl1 \
    # https://github.com/microsoft/wslg/issues/864
    libasound2-plugins \
    nano \
    vim \
    wget \
    curl \
    bzip2 \
    ca-certificates \
    sudo \
    && rm -rf /var/lib/apt/lists/*


# For terminal prompt to have color
RUN echo 'PS1="\[\033[01;32m\]\u@\h\[\033[00m\]:\[\033[01;34m\]\w\[\033[00m\]\$ "' >> /root/.bashrc

#ros packages
RUN apt-get update && apt-get install -y \
  ros-humble-plotjuggler*

# Set the default shell
SHELL ["/bin/bash", "-c"]

RUN mkdir -p /root/home/ros2_ws/src
WORKDIR /root/home/ros2_ws

# add stuff to bashrc
#RUN /bin/bash -c "echo \"source /root/home/ros2_ws/install/setup.bash\" >> ~/.bashrc"

# make image new and shiny
RUN apt update && apt dist-upgrade -y && rm -rf /var/lib/apt/lists/*

# Default command (can be overridden when running the container)
CMD ["/bin/bash"]
