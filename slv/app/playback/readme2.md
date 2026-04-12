Sample = một giá trị đo biên độ âm thanh tại một thời điểm
Âm thanh ngoài đời là liên tục (analog)
Máy tính chỉ lưu được rời rạc → chia thành nhiều điểm

sample_rate = số sample trong 1 giây (Càng nhiều sample → âm thanh càng mịn)
44100 Hz → 44100 sample / giây
48000 Hz → 48000 sample / giây

sample = 1 / sample_rate giây
có nb_samples sample
=> tổng thời gian = nb_samples × (1 / sample_rate)

frame->nb_samples = 1024;
=> Frame này chứa 1024 sample liên tiếp

Thucwj tees:
sample_rate = 48000;
nb_samples = 1024;
mỗi giây có 48000 sample
frame này có 1024 sample
=> am thanh kéo dài = 1024 / 48000 ≈ 0.021 giây (~21ms)
