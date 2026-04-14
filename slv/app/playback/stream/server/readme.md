VLC → RTSPServer → ServerMediaSession → FramedSource

TaskScheduler
     ↓
UsageEnvironment
     ↓
RTSPServer
     ↓
Client (VLC)

rtsp://localhost:8554/live

RTSPServer
   ↓
ServerMediaSession (filestream)
   ↓
Subsession (video/audio)
   ↓
FramedSource (FFmpeg queue của bạn)

