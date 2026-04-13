Live555 có các thành phần chính:

TaskScheduler: quản lý event loop
UsageEnvironment: môi trường log/debug
RTSPServer: server RTSP
ServerMediaSession: đại diện cho 1 stream
FramedSource: nguồn dữ liệu (file/video/raw)
RTPSink: encode và gửi RTP

Luồng cơ bản:
Video File → FramedSource → RTPSink → RTSP Server → Client (VLC)

