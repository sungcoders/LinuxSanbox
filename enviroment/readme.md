sudo usermod -aG docker $USER
newgrp docker

docker run -it --rm ...
docker run: chạy container mới
-it:
-i → giữ STDIN (cho nhập lệnh)
-t → cấp terminal (giống bash thật)
--rm: container sẽ tự xóa sau khi exit

--name "$CONTAINER_NAME"
Đặt tên container (dễ quản lý, debug)

-v "$WORKSPACE_HOST":"$WORKSPACE_CONT"
-w "$WORKSPACE_CONT"
-v: mount thư mục từ host → container
code bạn ở host dùng trực tiếp trong container
-w: set working directory trong container
vào container là đứng ngay thư mục project

--device /dev/snd
Cho container truy cập sound card của host
/dev/snd = ALSA device (low-level audio)

-e XDG_RUNTIME_DIR=/run/user/$(id -u)
-v /run/user/$(id -u):/run/user/$(id -u)
XDG_RUNTIME_DIR: nơi Linux lưu socket runtime (PulseAudio nằm ở đây)
$(id -u) = user ID hiện tại (ví dụ 1000)
mount thư mục runtime từ host vào container để:
container thấy được PulseAudio socket
tránh lỗi kiểu: "connection refused"

-e SDL_AUDIODRIVER=pulseaudio
ép SDL2 dùng PulseAudio thay vì ALSA
nếu không set → có thể SDL chọn sai driver

-e PULSE_SERVER=unix:/run/user/$(id -u)/pulse/native
chỉ đường đến socket PulseAudio
pulse/native = file socket để client connect
rất quan trọng nếu:
chạy trong Docker
hoặc nhiều audio backend

-e DISPLAY=$DISPLAY
-v /tmp/.X11-unix:/tmp/.X11-unix
DISPLAY: biến môi trường để app biết vẽ ra màn hình nào
/tmp/.X11-unix: socket X11
giúp container:
mở window SDL2
hiển thị GUI ra host

--user $(id -u):$(id -g)
chạy container với UID/GID của bạn
tránh:
file bị root ownership
lỗi permission khi build

"$IMAGE_NAME" \
/bin/bash
$IMAGE_NAME: image bạn build trước đó
/bin/bash: vào shell để làm việc

=== tom tat =====
Lệnh này tạo container với:
📁 dùng code trực tiếp từ host
🔊 phát audio qua PulseAudio
🖥️ hiển thị GUI (SDL/X11)
👤 chạy đúng user (không bị permission lỗi)
🧹 tự xóa sau khi thoát

