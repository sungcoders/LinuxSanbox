case1:
void Init(AVCodecContext* codecCtx);  // (1)
void Init(AVCodecContext& codecCtx);  // (2)
=> Cả hai đều liên quan đến địa chỉ
Pointer = bạn cầm địa chỉ
Reference = bạn dùng object nhưng thực chất là alias tới địa chỉ
Pointer = làm việc trực tiếp với địa chỉ
Reference = alias, không phải con trỏ


