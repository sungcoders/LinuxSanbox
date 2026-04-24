FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    build-essential \
    git \
    cmake \
    pkg-config \
    ffmpeg \
    libavcodec-dev \
    libavformat-dev \
    libavutil-dev \
    libswscale-dev \
    libsdl2-dev \
    libfmt-dev \
    libssl-dev \
    yasm \
    nasm \
    gdb \
    && rm -rf /var/lib/apt/lists/*

# add user 'sunglv'
RUN useradd -ms /bin/bash sunglv && echo "sunglv ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers
USER sunglv

# workspace docker
WORKDIR /workspace
