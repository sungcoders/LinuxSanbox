project(Playback)

find_package(PkgConfig REQUIRED)

pkg_check_modules(FFMPEG REQUIRED
    libavformat
    libavcodec
    libavutil
    libswscale
)

pkg_check_modules(SDL2 REQUIRED sdl2)

include_directories(
    ${FFMPEG_INCLUDE_DIRS}
    ${SDL2_INCLUDE_DIRS}
)

link_directories(
    ${FFMPEG_LIBRARY_DIRS}
    ${SDL2_LIBRARY_DIRS}
)

set(SOURCES 
    main.cpp 
    PlaybackPlayer.cpp 
    PlaybackDemux.cpp
    decode/PlaybackDecode.cpp
    decode/PlaybackDecodeAudio.cpp
    decode/PlaybackDecodeVideo.cpp
    output/PlaybackOutput.cpp
    output/PlaybackOutputAudio.cpp
    utils/PlaybackPacket.cpp
    utils/PlaybackFrame.cpp
    utils/PlaybackClock.cpp
    utils/UtilsWindow.cpp
    window/PlaybackWindow.cpp
    window/WindowAudio.cpp
    stream/server/StreamServer.cpp
    stream/server/FramedSourceVideo.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/decode
    ${CMAKE_CURRENT_SOURCE_DIR}/output
    ${CMAKE_CURRENT_SOURCE_DIR}/window
    ${CMAKE_CURRENT_SOURCE_DIR}/stream/server
    ${CMAKE_CURRENT_SOURCE_DIR}/stream/client
)

target_link_libraries(${PROJECT_NAME}
    ${FFMPEG_LIBRARIES}
    ${SDL2_LIBRARIES}
    swresample
    utils
    Live555
)
