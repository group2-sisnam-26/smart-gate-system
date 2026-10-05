# Smart Gate System — Kontrol Deteksi Jarak dan Objek

**Focus Group - Kelompok 2**
Ahmad Fatih Faizi (2206819344)

Arief Ridzki Darmawan (2306210115)

Hadyan Fachri (2306245030)

Sistem Tertanam 2026/2027

---

## Overview

Smart Gate System adalah sistem tertanam yang mendeteksi keberadaan dan jarak objek menggunakan kombinasi sensor ultrasonik dan infrared, lalu menggerakkan servo untuk membuka atau menutup gerbang secara otomatis.

## Tujuan

Mengeksplorasi karakteristik sensor ultrasonik, PIR, dan infrared dalam mendeteksi objek, serta mengintegrasikannya dengan aktuator servo dan stepper motor dalam satu sistem kontrol gerbang otomatis.

---

## 1. Komponen dan Cara Kerja

### Ultrasonic Sensor (PING))) / HC-SR04)
Mengukur jarak dengan memancarkan gelombang ultrasonik lewat pin trigger, lalu menghitung waktu tempuh gelombang tersebut memantul kembali ke pin echo. Waktu tempuh dikonversi menjadi jarak dalam cm. Pada modul PING))), trigger dan echo menggunakan pin sinyal yang sama secara bergantian.

### IR Obstacle (FC-51)
Memancarkan cahaya infrared lewat LED dan mendeteksi pantulannya menggunakan fototransistor. Jika ada objek dalam jangkauan pendek, cahaya pantul terdeteksi dan output berubah menjadi LOW. Sinyal ini bersifat digital sederhana, cocok dipakai sebagai trigger interrupt untuk respons cepat.

> Modul ini memiliki dua komponen mirip LED: satu **bening** sebagai pemancar infrared (transmitter), satu lagi **berwarna gelap** sebagai penerima (fototransistor) yang sekaligus berfungsi sebagai filter cahaya tampak agar deteksi lebih akurat.

### PIR HC-SR501
Mendeteksi perubahan radiasi inframerah **pasif** yang dipancarkan tubuh objek hangat (misalnya manusia) yang bergerak dalam jangkauannya. Output berupa digital HIGH saat gerakan terdeteksi, dan kembali LOW setelah durasi tertentu (diatur lewat potensiometer time delay). Sensor ini mendeteksi gerakan, bukan jarak, sehingga objek diam tidak akan terus terdeteksi.

**Kendala eksperimen:** modul PIR yang digunakan mengalami kerusakan, output tetap HIGH secara konstan meski tidak ada objek. Setelah pemeriksaan jumper trigger dan pengaturan potensiometer tidak menyelesaikan masalah, kelompok memutuskan untuk tidak menggunakan PIR pada sistem integrasi akhir.

### Servo (SG90)
Digerakkan langsung ke sudut tertentu (0°–180°) melalui sinyal PWM. Posisi servo ditentukan oleh lebar pulsa yang dikirim, sehingga servo bisa langsung diperintah ke sudut tujuan (0° untuk tutup, 90° untuk buka) tanpa perlu menghitung langkah seperti stepper.

### Stepper Motor (28BYJ-48)
Bergerak dalam langkah-langkah diskrit (step) yang dikendalikan lewat urutan pengaktifan koil, sehingga posisinya bersifat relatif terhadap posisi sebelumnya, bukan sudut absolut. Dengan library AccelStepper, jumlah step dan kecepatan dapat diatur untuk menghasilkan gerakan yang presisi dan halus.

---

## 2. Analisis Tambahan

### 2.1 PIR vs IR Obstacle — Perbedaan Mendasar

| Aspek | PIR (HC-SR501) | IR Obstacle (FC-51) |
|---|---|---|
| Jenis infrared | **Pasif** — hanya menerima radiasi panas dari objek | **Aktif** — memancarkan cahaya infrared sendiri, deteksi via pantulan |
| Yang dideteksi | Gerakan objek bersuhu (manusia, hewan, kendaraan panas) | Keberadaan objek apa pun di depan sensor, panas atau tidak |
| Objek diam | Tidak terdeteksi setelah beberapa saat (output balik LOW) | Tetap terdeteksi selama objek ada di depan sensor |
| Jangkauan | Jauh (± 3–7 meter) | Pendek (umumnya < 30 cm) |
| Gangguan utama | Perubahan suhu lingkungan, sumber panas lain | Warna permukaan objek, cahaya infrared lingkungan (matahari) |

**Intinya:** PIR mendeteksi *"ada sesuatu yang bergerak dan hangat"*, sedangkan IR Obstacle mendeteksi *"ada sesuatu secara fisik di depan sensor, bergerak atau tidak"*.

### 2.2 Konsekuensi Jika FC-51 Diganti PIR

Pada sistem integrasi, FC-51 berperan sebagai konfirmasi kedua setelah ultrasonik mendeteksi jarak dekat (state `TUNGGU_IR`). Jika diganti PIR:

1. **Logika interrupt perlu diubah** — trigger `FALLING` (FC-51, LOW = objek) tidak cocok dengan PIR yang HIGH saat gerakan terdeteksi; perlu diubah ke `RISING` plus penanganan reset tambahan.
2. **Objek diam berisiko tidak terkonfirmasi** — desain `TUNGGU_IR` mengandalkan sensor yang responsif terhadap objek diam, sementara PIR membutuhkan gerakan kontinu untuk tetap HIGH.
3. **Delay warm-up tambahan** — PIR butuh 30–60 detik stabilisasi saat power-on, FC-51 tidak.
4. **Risiko false trigger lebih tinggi** — PIR bisa salah mendeteksi sumber panas lain yang lewat di sekitar sensor, bukan hanya kendaraan yang dituju.

**Kesimpulan:** mengganti FC-51 dengan PIR pada skenario ini **menurunkan keandalan sistem**, karena kelemahan utama PIR (tidak mendeteksi objek diam) bertentangan dengan fungsi state `TUNGGU_IR` yang butuh konfirmasi objek yang mungkin sedang diam menunggu.

### 2.3 Servo vs Stepper: Perbandingan Teknis

| | Stepper Motor | Servo Motor |
|---|---|---|
| Sistem Kontrol | Open-loop (tidak tahu posisi sendiri) | Closed-loop (tahu posisi sendiri) |
| Karakteristik | Torsi tinggi di RPM rendah, turun di RPM tinggi | Torsi stabil dari RPM rendah sampai tinggi |
| Kecepatan | Rendah–sedang (umumnya < 1000–2000 RPM) | Sangat tinggi dan responsif |
| Risiko | Step bisa terskip jika beban terlalu berat (kalibrasi kacau) | Mengoreksi posisi otomatis jika ada hambatan |
| Holding Torque & Arus | Bisa menahan beban tinggi saat diam, tapi terus menarik arus walau diam | Menarik arus hanya saat bergerak atau ada beban eksternal |

### 2.4 Servo vs Stepper: Plus Minus Implementasi

| Aspek | Servo (SG90) | Stepper (28BYJ-48) |
|---|---|---|
| Kapan dipakai | Gerbang kecil/ringan, gerak cepat, sudut terbatas 0°–180° | Gerbang lebih berat, butuh torsi tahan beban atau presisi posisi tinggi |
| Kenapa tidak dipakai | Torsi kecil, tidak cocok beban berat atau gerbang lebar | Perlu driver tambahan, kabel lebih banyak, kode lebih kompleks |
| Kemudahan implementasi | Sangat simpel — 1 kabel sinyal, langsung `write(sudut)` | Perlu 4 pin + library AccelStepper, perlu kalibrasi posisi awal (homing) |
| Risiko di lapangan | Minim, karena closed-loop | Step bisa terlewat jika gerbang macet, posisi jadi tidak sinkron tanpa sensor tambahan |
| Ketergantungan sensor tambahan | Tidak perlu | Idealnya perlu limit switch sebagai referensi posisi awal |

**Kesimpulan implementasi:** untuk prototipe gerbang seperti pada eksperimen ini, servo lebih praktis dan memadai. Stepper lebih unggul jika skala gerbang diperbesar, namun membutuhkan penanganan tambahan untuk kalibrasi dan potensi missed-step.

---

## 3. Eksperimen

### 3.1 Smart Gate System (Ultrasonic + IR Obstacle)

**Komponen**
- 1x Arduino Mega
- 1x Servo SG-90
- 1x Infrared Sensor FC-51
- 1x PING))) Ultrasonic Distance Sensor

**Wiring**

| Arduino Mega | Tersambung ke |
|---|---|
| 5V | VCC PING))), VCC FC-51, merah servo |
| GND | GND PING))), GND FC-51, coklat servo |
| 2 | OUT FC-51 |
| 4 | SIG PING))) |
| 9 | kuning servo (sinyal) |

**Konfigurasi**
- Library: `Servo.h`, Timer2 dari `avr/interrupt.h`
- Servo: 0° = tertutup, 90° = terbuka
- Jarak mobil = 30 cm, interval pembacaan = 100 ms, jeda penutupan = 3000 ms
- Timer2 mode CTC, prescaler 64, `OCR2A = 249` → interrupt tiap 1 ms
- Interrupt IR pada pin 2 (mode `FALLING`) — FC-51 HIGH saat normal, LOW saat objek terdeteksi
- Komunikasi serial 115200 baud
- Jarak PING))) dihitung dari `durasi / 58` (µs ke cm), timeout `pulseIn` 25 ms

**Cara kerja:** ultrasonik mendeteksi objek dalam jarak < 30 cm → sistem masuk state `TUNGGU_IR` → menunggu konfirmasi dari interrupt IR (FC-51) → servo membuka gerbang. Gerbang menutup otomatis setelah objek pergi dan jeda 3 detik terlampaui.

### 3.2 Stepper Motor Control (Eksperimen Independen)

Eksperimen independen yang mengeksplorasi karakteristik, implementasi, dan setup dari stepper motor.

**Komponen**
- 1x Arduino Mega
- 1x 28BYJ-48 Stepper Motor
- 1x Driver board ULN2003AN

**Wiring**

| Driver Board | Pin Arduino |
|---|---|
| IN1 | 8 |
| IN2 | 9 |
| IN3 | 10 |
| IN4 | 11 |

**Konfigurasi**
- Library: `AccelStepper.h`
- `MotorInterfaceType = 8` (half step)
- Steps per revolution = 4096
- Max speed = 1000 steps/s, max acceleration = 200 steps/s²

**Cara kerja:** mikrokontroler menerima input bilangan integer lewat Serial Monitor, lalu menggerakkan stepper motor ke posisi (step) yang ditunjuk oleh bilangan tersebut. Arah gerakan ditentukan oleh urutan pulsa coil, kecepatan ditentukan oleh frekuensi pulsa — setiap pulsa menggerakkan motor satu step.

---

## 4. Kendala dan Pembelajaran

Modul PIR HC-SR501 yang digunakan mengalami kerusakan — output tetap HIGH secara konstan sehingga sistem selalu mendeteksi adanya gerakan meski tidak ada objek. Pemeriksaan jumper trigger dan pengaturan potensiometer sensitivity/time delay tidak menyelesaikan masalah. Kelompok menyimpulkan modul kemungkinan cacat produksi, dan memutuskan mengganti strategi deteksi menjadi kombinasi **Ultrasonic + IR Obstacle (FC-51)** untuk sistem integrasi akhir.

Kendala ini menjadi pembelajaran penting mengenai pentingnya validasi hardware sebelum diintegrasikan ke sistem yang lebih besar, serta pertimbangan karakteristik sensor (pasif vs aktif, objek diam vs bergerak) dalam menentukan desain state machine yang sesuai.

---

## 5. Kesimpulan

Kombinasi ultrasonik dan IR Obstacle terbukti lebih andal dibanding PIR untuk kasus deteksi objek pada gerbang otomatis, terutama karena kemampuannya mendeteksi objek diam. Untuk aktuator, servo lebih praktis untuk prototipe skala kecil, sementara stepper motor berpotensi lebih unggul pada skala yang lebih besar dengan beban yang lebih berat, dengan catatan membutuhkan penanganan tambahan untuk kalibrasi posisi.