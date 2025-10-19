# 🚦 FreeRTOS LED Blink with Dynamic Frequency and Duty Cycle  
### STM32F103 + FreeRTOS + Queue Communication  

---

## 📘 MÔ TẢ DỰ ÁN  
Dự án này minh họa cách sử dụng **FreeRTOS Queue** để **truyền dữ liệu giữa hai task** trên vi điều khiển **STM32F103C8T6 (Blue Pill)**.  
Mục tiêu: điều khiển LED nhấp nháy với **tần số** và **độ rộng xung (duty cycle)** thay đổi liên tục, được sinh ngẫu nhiên bởi một task khác.

---

## ⚙️ CẤU TRÚC HỆ THỐNG  
Hệ thống được xây dựng trên **FreeRTOS**, gồm **hai task** hoạt động song song và **một hàng đợi (Queue)** để truyền dữ liệu giữa chúng.

### 🧱 Thành phần chính:
- **TaskGen**: Sinh ngẫu nhiên giá trị tần số (freq) và độ rộng xung (duty), sau đó gửi vào hàng đợi.
- **TaskBlink**: Nhận dữ liệu từ hàng đợi, tính toán chu kỳ bật/tắt và điều khiển LED chớp tương ứng.
- **xBlinkQueue**: Hàng đợi trung gian lưu trữ cấu trúc dữ liệu `BlinkParam_t` giữa hai task.

---

## 🧩 CHỨC NĂNG CỤ THỂ  

### 🧠 TaskGen()  
- Sinh ngẫu nhiên hai giá trị điều khiển LED:
  - `freq`: tần số (1–5 Hz)
  - `duty`: độ rộng xung (10–90%)
- Gửi cấu trúc `BlinkParam_t` chứa hai giá trị trên vào hàng đợi `xBlinkQueue`.
- Mỗi **2 giây** gửi một lần để LED thay đổi kiểu nhấp nháy.

### 💡 TaskBlink()  
- Nhận dữ liệu từ hàng đợi `xBlinkQueue`.
- Tính toán chu kỳ và thời gian bật/tắt LED dựa theo:
period = 1000 / freq
onTime = (period * duty) / 100
offTime = period - onTime

- Bật/Tắt LED tương ứng trong các khoảng thời gian này.
- Nếu không có dữ liệu mới trong 10 ms → giữ tham số cũ.

---

## 📦 CẤU TRÚC THƯ MỤC  
├── Src/
│ ├── main.c # File chính chứa 2 task và cấu hình
├── Inc/
│ ├── FreeRTOS.h
│ ├── task.h
│ ├── queue.h
│ └── stm32f10x.h
├── README.md
└── FreeRTOSConfig.h # Cấu hình FreeRTOS
---

## 🔧 CẤU HÌNH PHẦN CỨNG  

| Thành phần | Kết nối | Mô tả |
|-------------|----------|--------|
| LED         |      | LED trên board Blue Pill (Active Low) |
| Clock       | 72 MHz   | Xung hệ thống chuẩn của STM32F103 |
| FreeRTOS    | v10.x    | Hệ điều hành thời gian thực |

---
🚀 HƯỚNG DẪN BIÊN DỊCH & NẠP
⚙️ Yêu cầu:
- Keil uVision, STM32CubeIDE, hoặc PlatformIO
- ST-Link V2 hoặc tương đương để nạp firmware
- Thư viện FreeRTOS đã được thêm vào project
📖 Các bước thực hiện:
- Tạo project mới cho STM32F103C8T6.
- Thêm các file FreeRTOS, task.h, queue.h, stm32f10x.h.
- Sao chép mã trong main.c vào project.
- Build và nạp chương trình.
- Quan sát LED PC14 nhấp nháy với tần số và duty thay đổi ngẫu nhiên.

🧠 NGUYÊN LÝ HOẠT ĐỘNG
- TaskGen gửi dữ liệu BlinkParam_t → vào Queue.
- TaskBlink nhận dữ liệu từ Queue → cập nhật cách chớp LED.
- Hai task hoạt động độc lập, an toàn nhờ cơ chế truyền tin của FreeRTOS Queue.

