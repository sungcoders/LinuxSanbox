#!/bin/bash

# =============================
# Config default
# =============================
DEFAULT_IMAGE="linux-sanbox"
DEFAULT_CONTAINER="linux-sanbox-container"
WORKSPACE_HOST="$(cd "$(pwd)/.." && pwd)"
WORKSPACE_CONT="/workspace"
DOCKERFILE="Dockerfile"
FLAG_USED=false

VERSION="1.0.0"

# =============================
# functions helper
# =============================
show_help() {
    cat << EOF
Usage: $0 [OPTIONS]

OPTIONS:
  -v        Version script
  -h        Help
  -r        Run container (image: $DEFAULT_IMAGE, container: $DEFAULT_CONTAINER)
  -b        Build image from Dockerfile (image: $DEFAULT_IMAGE)
  -c        Delete dangling images
  -i        Show information Docker (images, containers)

Examples:
  $0 -r     # run container: cpp-env-container, image: cpp-env
  $0 -b     # build image name: cpp-env
  $0 -c     # delete dangling images
EOF
}

#run container
run_container() {
    CONTAINER_NAME=$DEFAULT_CONTAINER
    IMAGE_NAME=$DEFAULT_IMAGE

    echo "running container: $CONTAINER_NAME (image: $IMAGE_NAME) build context: $WORKSPACE_HOST"
    docker run -it --rm \
        --name "$CONTAINER_NAME" \
        -v "$WORKSPACE_HOST":"$WORKSPACE_CONT" \
        -w "$WORKSPACE_CONT" \
        --device /dev/snd \
        -e XDG_RUNTIME_DIR=/run/user/$(id -u) \
        -v /run/user/$(id -u):/run/user/$(id -u) \
        -e SDL_AUDIODRIVER=pulseaudio \
        -e DISPLAY=$DISPLAY \
        -v /tmp/.X11-unix:/tmp/.X11-unix \
        -p 8554:8554 \
        "$IMAGE_NAME" \
        /bin/bash
}

#build image
build_image() {
    IMAGE_NAME=$DEFAULT_IMAGE
    echo "Build image: $IMAGE_NAME"
    # Run the build with the repo root as context
    docker build -f "$DOCKERFILE" -t "$IMAGE_NAME" "$WORKSPACE_HOST"
}

#clean dangling images
clean_dangling() {
    echo "Delete dangling images..."
    docker image prune -f
}

#show docker info
show_info() {
    echo "Docker Images:"
    docker images
    echo
    echo "Docker Containers:"
    docker ps -a
}

# =============================
# Parse options
# =============================
while getopts "vhrbci" opt; do
    FLAG_USED=true
    case $opt in
        v)
            echo "run_docker.sh version $VERSION"
            exit 0
            ;;
        h)
            show_help
            exit 0
            ;;
        r)
            run_container
            exit 0
            ;;
        b)
            build_image
            exit 0
            ;;
        c)
            clean_dangling
            exit 0
            ;;
        i)
            show_info
            exit 0
            ;;
        *)
            show_help
            exit 0
            ;;
    esac
done

# default action: build and run if no flags are provided
if [ "$FLAG_USED" = false ]; then
    build_image
    run_container
fi