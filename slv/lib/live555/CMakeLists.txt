add_library(Live555 INTERFACE)

target_include_directories(Live555 INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/liveMedia
    ${CMAKE_CURRENT_SOURCE_DIR}/include/groupsock
    ${CMAKE_CURRENT_SOURCE_DIR}/include/BasicUsageEnvironment
    ${CMAKE_CURRENT_SOURCE_DIR}/include/UsageEnvironment
)

target_link_libraries(Live555 INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/libliveMedia.a
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/libgroupsock.a
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/libBasicUsageEnvironment.a
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/libUsageEnvironment.a
)
