//Build
mkdir build
cd build
cmake ..
cmake --build .

// build clean
cmake --build . --target clean

// rebuild
cmake --build . --clean-first

// video.mp
.\player.exe


| Mục đích                   | Lệnh                                                         | Khi nào dùng                                |
| -------------------------- | ------------------------------------------------------------ | ------------------------------------------- |
|  Clean + build sạch 100%   | `rm -rf build && cmake -S . -B build && cmake --build build` | Lỗi lạ, đổi lib (FFmpeg, SDL), đổi compiler |
|  Clean nhẹ                 | `cmake --build build --target clean`                         | Chỉ xoá file build (.o, .obj)               |
|  Clean + build nhanh       | `cmake --build build --clean-first`                          | Build lại bình thường                       |
|  Build                     | `cmake --build build`                                        | Build lại code                              |
|  Generate (config)         | `cmake -S . -B build`                                        | Sau khi sửa CMakeLists                      |

//Cmakelist
cmakelist không trực tiếp build code của bạn mà nó sẽ đọc bản thiết kế của bạn (cmakelists.txt) rồi kiểm tra xem bạn cấu hình đúng hay chưa và có sai sót gì không và có đầy đủ môi trường cần build chưa.
sau đó nó sẽ tạo ra bản chỉ dẫn cho các công cụ build thực hiện (linux/macos makefile, window sln, công cụ hiện đại .ninja)
cmake --build . => gọi các công cụ build rôi các công cụ build gọi trình biên dịch thành các file object và linker sẽ linker sẽ liên kết các file object lại với nhau.

// condition
demux (packet, decode)
decode (packet, frame, output)
output (frame)
