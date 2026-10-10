# Smart Gate System — Kontrol Deteksi Jarak dan Objek
**Focus Group - Kelompok 2**
* Ahmad Fatih Faizi (2206819344)
* Arief Ridzki Darmawan (2306210115)
* Hadyan Fachri (2306245030)
* Sistem Tertanam 2026/2027

--------------------------------------------------------------------------------

## Overview
Smart Gate System adalah sistem tertanam yang mendeteksi keberadaan dan jarak objek menggunakan kombinasi sensor ultrasonik dan infrared, lalu menggerakkan servo untuk membuka atau menutup gerbang secara otomatis. Proyek ini merupakan eksplorasi Kelompok 2 pada tema Kontrol Deteksi Jarak dan Objek.

## Tujuan
Mengeksplorasi karakteristik sensor ultrasonik, PIR, dan infrared dalam mendeteksi objek, mengintegrasikannya dengan aktuator (servo dan stepper motor), serta menganalisis spesifikasi teknis dan antarmuka komponen tersebut dengan mikrokontroler Arduino Mega.

--------------------------------------------------------------------------------

## A. Pendahuluan
Smart Gate System adalah prototipe sistem tertanam yang bertujuan untuk mendeteksi keberadaan dan jarak objek menggunakan kombinasi sensor ultrasonik dan sensor infrared (IR Obstacle). Sistem ini memproses data sensor untuk kemudian menggerakkan motor servo sebagai aktuator yang membuka atau menutup gerbang secara otomatis.

Laporan ini bertujuan untuk mengeksplorasi karakteristik sensor ultrasonik, PIR, dan infrared dalam mendeteksi objek, mengintegrasikannya dengan aktuator (servo dan stepper motor), serta menganalisis spesifikasi teknis dan antarmuka komponen tersebut dengan mikrokontroler Arduino Mega.

--------------------------------------------------------------------------------

## B. Prinsip Kerja Setiap Komponen
1. **Ultrasonic Sensor (PING))) / HC-SR04)**  
   Mengukur jarak dengan memancarkan gelombang suara ultrasonik (umumnya 40 kHz) lewat *transmitter*, lalu menghitung waktu tempuh gelombang tersebut hingga memantul kembali dan diterima oleh *receiver*. Waktu tempuh ini dikonversi menjadi jarak. Pada modul PING))), proses *trigger* dan penerimaan pantulan (*echo*) menggunakan satu pin sinyal yang sama secara bergantian.
2. **IR Obstacle (FC-51)**  
   Modul ini memancarkan cahaya infrared lewat LED *transmitter* (bening) dan mendeteksi pantulannya menggunakan fototransistor *receiver* (gelap/hitam) yang sekaligus berfungsi sebagai filter cahaya tampak agar deteksi lebih akurat. Jika ada objek dalam jangkauan jarak pendek di depannya, cahaya memantul dan terdeteksi. Output sensor akan berubah menjadi LOW melalui komparator (LM393). Sinyal ini bersifat digital sederhana, cocok dipakai sebagai trigger interrupt untuk respons cepat.
3. **PIR (HC-SR501)**  
   Mendeteksi perubahan radiasi inframerah *pasif* yang dipancarkan oleh objek bersuhu (seperti tubuh manusia atau mesin kendaraan) yang bergerak dalam jangkauannya. Output berupa sinyal digital HIGH saat gerakan terdeteksi, dan kembali LOW setelah durasi waktu (*delay*) yang diatur terlampaui. Sensor mendeteksi *gerakan*, bukan eksistensi objek diam.
4. **Servo Motor (SG90)**  
   Motor aktuator *closed-loop* yang dikendalikan melalui sinyal PWM (Pulse Width Modulation). Lebar pulsa yang dikirimkan menentukan posisi absolut sudut motor (umumnya 0°–180°). Ini membuat servo sangat responsif untuk membuka/menutup palang tanpa perlu perhitungan putaran rumit.
5. **Stepper Motor (28BYJ-48)**  
   Motor aktuator *open-loop* yang bergerak dalam langkah-langkah diskrit (*step*) dengan cara mengaktifkan elektromagnet (koil) secara berurutan, sehingga posisinya bersifat relatif terhadap posisi sebelumnya, bukan sudut absolut. Untuk menggerakkannya, mikrokontroler mengirimkan pola sinyal ke IC *Driver* (ULN2003). Dengan library AccelStepper, jumlah step dan kecepatan dapat diatur untuk menghasilkan gerakan yang presisi dan halus.

--------------------------------------------------------------------------------

## C. Spesifikasi Teknis
Berikut adalah spesifikasi teknis dari komponen yang digunakan (referensi dari standar *datasheet* pabrikan):

### 1. Parallax PING))) Ultrasonic Sensor
| Parameter | Spesifikasi |
| ------ | ------ |
| Tegangan Kerja (VCC) | 5V DC |
| Arus Operasional | ~30 mA |
| Rentang Jarak Deteksi | 2 cm - 300 cm |
| Frekuensi Suara | 40 kHz |
| Antarmuka Komunikasi | 1 Pin TTL (Trigger dan Echo berbagi pin yang sama) |
| Indikator | LED (menyala saat memancarkan gelombang) |

### 2. IR Obstacle Sensor (FC-51)
| Parameter | Spesifikasi |
| ------ | ------ |
| Tegangan Kerja (VCC) | 3.3V - 5V DC |
| Rentang Jarak Deteksi | 2 cm - 30 cm (dapat diatur via potensiometer) |
| Sudut Deteksi | ± 35 derajat |
| Sinyal Output | Digital TTL (LOW saat mendeteksi objek, HIGH saat normal) |
| Chip Komparator | LM393 |

### 3. PIR Sensor (HC-SR501)
| Parameter | Spesifikasi |
| ------ | ------ |
| Tegangan Kerja (VCC) | 4.5V - 20V DC |
| Tegangan Output | Digital 3.3V (HIGH/Terdeteksi), 0V (LOW/Normal) |
| Rentang Jarak Deteksi | 3 meter - 7 meter (dapat diatur via potensiometer) |
| Sudut Deteksi | < 100 derajat (bentuk kerucut) |
| Waktu Tunda (Delay Time) | 0.3 detik hingga ~5 menit (dapat diatur) |
| Waktu Blokir (Blockade Time) | 2.5 detik (default) |

### 4. Micro Servo Motor (SG90)
| Parameter | Spesifikasi |
| ------ | ------ |
| Tegangan Kerja | 4.8V - 6.0V DC |
| Torsi (Stall Torque) | 1.8 kg-cm (pada 4.8V) |
| Kecepatan Operasi | 0.12 detik / 60 derajat (pada 4.8V tanpa beban) |
| Sudut Putaran | 0 - 180 derajat |
| Sinyal Kontrol | PWM (Periode: 20 ms / 50 Hz) |
| Rentang Pulsa | 1 ms (0°) hingga 2 ms (180°) |

### 5. Stepper Motor (28BYJ-48) & Driver ULN2003
| Parameter | Spesifikasi |
| ------ | ------ |
| Tegangan Kerja | 5V DC |
| Tipe / Fase | Unipolar / 4 Fase |
| Rasio Gear Reduksi | 1 / 64 |
| Langkah per Revolusi | 2048 (Full-step) / 4096 (Half-step) |
| Sudut per Langkah (Stride) | 5.625° / 64 (sekitar 0.088° per langkah *half-step*) |
| IC Driver | ULN2003 (Darlington Transistor Array) |

--------------------------------------------------------------------------------

## D. Koneksi dengan Fitur Mikrokontroler
Agar sistem berjalan optimal, antarmuka ke mikrokontroler memanfaatkan berbagai fitur perangkat keras khusus Arduino Mega:
1. **Timer/Counter (Timer2):** Digunakan untuk melakukan *tick* internal (menghitung waktu tanpa *blocking*) melalui fitur **Interrupt (ISR)**. Timer2 diatur pada mode CTC dengan prescaler 64 untuk memicu *interrupt* secara presisi setiap 1 milidetik (`ISR(TIMER2_COMPA_vect)`).
2. **External Interrupt (INT4 pada Pin 2):** Pin sinyal FC-51 dihubungkan ke fitur External Interrupt Arduino (`attachInterrupt`). Mode diatur ke `FALLING` agar mikrokontroler segera merespons seketika saat sinyal berubah dari HIGH ke LOW (objek terdeteksi) tanpa perlu melakukan *polling* secara terus-menerus di fungsi `loop()`.
3. **GPIO Digital (I/O):** Digunakan untuk dua hal:
   * **Membaca jarak:** Pin 4 Arduino difungsikan secara bergantian sebagai *OUTPUT* (untuk trigger pulsa 5µs) dan sebagai *INPUT* untuk membaca durasi gema pantulan via fungsi `pulseIn()`.
   * **Menggerakkan Stepper:** Menggunakan 4 pin GPIO sebagai *OUTPUT* digital untuk mengaktifkan koil stepper motor secara berurutan.
4. **Timer untuk Sinyal Servo (Timer5 via Servo.h):** Library `Servo.h` membangkitkan sinyal kontrol 50 Hz (pulsa 1-2 ms) di pin 9 menggunakan interrupt timer internal (Timer5 pada Arduino Mega), sehingga tidak bentrok dengan Timer2 yang dipakai untuk tick 1 ms.
5. **UART / Serial Communication:** Menggunakan hardware UART (melalui kabel USB ke PC) pada *baud rate* 115200 (untuk sistem gate) dan 9600 (untuk eksperimen stepper) untuk keperluan *debugging*, input bilangan via Serial Monitor, dan laporan status.

--------------------------------------------------------------------------------

## E. Rangkaian Antarmuka & Konfigurasi
### 1. Smart Gate System (Utama)
**Komponen:** 1x Arduino Mega, 1x Servo SG-90, 1x Infrared Sensor FC-51, 1x PING))) Ultrasonic Distance Sensor

| Arduino Mega | Modul / Sinyal | Tersambung ke Pin | Keterangan |
| ------ | ------ | ------ | ------ |
| 5V | PING))) | VCC | Catu daya sensor jarak |
| 5V | FC-51 | VCC | Catu daya sensor IR |
| 5V | Servo SG90 | Kabel Merah | Catu daya motor servo |
| GND | Semua Modul | GND / Kabel Coklat | *Common Ground* |
| D2 | FC-51 | OUT | External Interrupt (`FALLING`) |
| D4 | PING))) | SIG | GPIO (Trigger/Echo bergantian) |
| D9 | Servo SG90 | Kabel Kuning/Oranye | Sinyal Kontrol (PWM via `Servo.h`) |

**Konfigurasi & Setup:**
* **Library:** `Servo.h`, Timer2 dari `avr/interrupt.h`
* **Servo:** 0° = tertutup, 90° = terbuka
* **Aturan Jarak & Waktu:** Jarak mobil = 30 cm, interval pembacaan jarak = 100 ms, jeda penutupan gerbang = 3000 ms (3 detik)
* **Timer2:** Mode CTC, prescaler 64, OCR2A = 249 → interrupt terjadi tiap 1 ms
* **Interrupt IR:** Pin 2 (mode `FALLING`), sensor FC-51 bernilai HIGH saat normal dan berubah ke LOW saat objek terdeteksi. Jarak objek diatur secara manual secara fisik pada potensiometer sensor.
* **Komunikasi Serial:** 115200 baud
* **Formula Jarak:** PING))) dihitung dari `durasi / 58` (mikrodetik ke cm), dengan timeout 25 ms pada `pulseIn()`

### 2. Eksperimen Stepper Motor Control (Independen)
Eksperimen independen yang mengeksplorasi karakteristik, implementasi, dan setup dari stepper motor.

**Komponen:** 1x Arduino Mega, 1x 28BYJ-48 Stepper Motor, 1x Driver board ULN2003AN

| Driver Board ULN2003 | Pin Arduino Mega | Keterangan |
| ------ | ------ | ------ |
| 5V | 5V | Catu daya stepper |
| GND | GND | *Common Ground* |
| IN1 | D8 | GPIO Output (Coil 1) |
| IN2 | D9 | GPIO Output (Coil 2) |
| IN3 | D10 | GPIO Output (Coil 3) |
| IN4 | D11 | GPIO Output (Coil 4) |

**Konfigurasi & Setup:**
* **Library:** `AccelStepper.h`
* **MotorInterfaceType:** 8 (half step)
* **Steps per Revolution:** 4096
* **Kecepatan & Akselerasi:** `maxSpeed = 1000` (steps/s), `maxAcceleration = 200` (steps/s²)
* **Cara Kerja:** Mikrokontroler menerima input bilangan integer lewat Serial Monitor, lalu menggerakkan stepper motor ke posisi (*step*) yang ditunjuk oleh bilangan tersebut. Arah gerakan ditentukan oleh urutan pulsa coil, kecepatan ditentukan oleh frekuensi pulsa — setiap pulsa menggerakkan motor satu *step*.

--------------------------------------------------------------------------------

## F. Program Dasar

### 1. Program Integrasi Smart Gate (Ultrasonic + IR + Servo)
Program ini mengimplementasikan *State Machine* (Mesin Status) dan berbasis waktu mandiri (*non-blocking timer*).

```cpp
#include <avr/interrupt.h>
#include <Servo.h>

// ---------- pin ----------
const byte pin_ir    = 2;   // OUT FC-51 (LOW = objek terdeteksi)
const byte pin_ping  = 4;   // SIG PING)))
const byte pin_servo = 9;

// ---------- pengaturan ----------
const uint16_t jarak_mobil_cm   = 30;    // lebih dekat dari ini = ada mobil
const uint16_t jeda_tutup_ms    = 3000;  // tunggu setelah mobil pergi
const uint16_t interval_baca_ms = 100;   // seberapa sering ukur jarak
const byte sudut_tutup = 0;
const byte sudut_buka  = 90;

// ---------- status ----------
enum Status { TUTUP, TUNGGU_IR, TERBUKA, JEDA_TUTUP };
Status status = TUTUP;

volatile uint32_t tick_ms = 0; // bertambah 1 tiap 1 ms (Timer2)
volatile bool gerakan_ada = false; // di-set oleh interrupt ir
volatile bool menunggu_ir = false; // ir hanya diproses saat TUNGGU_IR

bool mobil_dekat = false;
uint32_t waktu_baca_terakhir = 0;
uint32_t waktu_jeda_mulai = 0;

Servo gerbang;

// dipanggil otomatis oleh Timer2 setiap 1 ms
ISR(TIMER2_COMPA_vect) {
  tick_ms++;
}

// dipanggil otomatis saat FC-51 berubah HIGH -> LOW (ada objek)
void gerakan_terdeteksi() {
  if (menunggu_ir) {
    gerakan_ada = true;
  }
}

// membaca tick yang juga berubah di dalam ISR
uint32_t baca_tick() {
  noInterrupts();
  uint32_t tick = tick_ms;
  interrupts();
  return tick;
}

// Timer2: 16 MHz / 64 = 250 kHz -> 250 hitungan = 1 ms
void atur_timer() {
  noInterrupts();
  TCCR2A = 0;
  TCCR2B = 0;
  TCNT2 = 0;
  OCR2A = 249;
  TCCR2A |= (1 << WGM21); // mode CTC
  TCCR2B |= (1 << CS22); // prescaler 64
  TIMSK2 |= (1 << OCIE2A);
  interrupts();
}

uint16_t baca_jarak_cm() {
  // 1. pin jadi output, kirim pulsa trigger 5 us
  pinMode(pin_ping, OUTPUT);
  digitalWrite(pin_ping, LOW);
  delayMicroseconds(2);
  digitalWrite(pin_ping, HIGH);
  delayMicroseconds(5);
  digitalWrite(pin_ping, LOW);

  // 2. pin yang sama jadi input, ukur pulsa echo
  pinMode(pin_ping, INPUT);
  unsigned long durasi = pulseIn(pin_ping, HIGH, 25000); // timeout 25 ms
  if (durasi == 0) return 999; // tidak ada pantulan = jauh
  return durasi / 58;
}

void buka_gerbang() {
  gerbang.write(sudut_buka);
  Serial.println("gerbang TERBUKA");
}

void tutup_gerbang() {
  gerbang.write(sudut_tutup);
  Serial.println("gerbang TERTUTUP");
}

void setup() {
  pinMode(pin_ir, INPUT);
  Serial.begin(115200);
  gerbang.attach(pin_servo);
  attachInterrupt(digitalPinToInterrupt(pin_ir), gerakan_terdeteksi, FALLING);
  atur_timer();
  tutup_gerbang();
  Serial.println("siap");
}

void loop() {
  uint32_t sekarang = baca_tick();

  // 1. ukur jarak secara berkala
  if (sekarang - waktu_baca_terakhir >= interval_baca_ms) {
    waktu_baca_terakhir = sekarang;
    mobil_dekat = (baca_jarak_cm() < jarak_mobil_cm);
  }

  // 2. jalankan state machine
  switch (status) {
    case TUTUP:
      if (mobil_dekat) {
        gerakan_ada = false;
        menunggu_ir = true;
        status = TUNGGU_IR;
        Serial.println("mobil terdeteksi, menunggu ir...");
      }
      break;

    case TUNGGU_IR:
      if (!mobil_dekat) {
        menunggu_ir = false;
        status = TUTUP;
        Serial.println("mobil pergi, batal");
      } else if (gerakan_ada || digitalRead(pin_ir) == LOW) {
        menunggu_ir = false;
        gerakan_ada = false;
        buka_gerbang();
        status = TERBUKA;
      }
      break;

    case TERBUKA:
      if (!mobil_dekat) {
        waktu_jeda_mulai = sekarang;
        status = JEDA_TUTUP;
        Serial.println("mobil sudah lewat, hitung mundur...");
      }
      break;

    case JEDA_TUTUP:
      if (mobil_dekat) {
        status = TERBUKA;
      } else if (sekarang - waktu_jeda_mulai >= jeda_tutup_ms) {
        tutup_gerbang();
        status = TUTUP;
      }
      break;
  }
}
```

### 2. Program Kendali Stepper Motor via Serial
Program independen untuk mengevaluasi karakteristik gerak stepper menggunakan input berbasis Serial.

```cpp
#include <AccelStepper.h>
#include <ctype.h>

#define MP1 8
#define MP2 9
#define MP3 10
#define MP4 11

#define MotorInterfaceType 8
AccelStepper stepper = AccelStepper(MotorInterfaceType, MP1, MP3, MP2, MP4);
const int SPR = 4096; //Steps per revolution

boolean isValidNumber(String str) {
  str.trim();
  if (str.length() == 0) return false;

  byte startIndex = 0;
  if (str.charAt(0) == '-') {
    if (str.length() == 1) return false;
    startIndex = 1;
  }

  for (byte i = startIndex; i < str.length(); i++) {
    if (!isDigit(str.charAt(i))) {
      return false;
    }
  }
  return true;
}

void setup() {
  Serial.begin(9600);
  stepper.setMaxSpeed(1000);
  stepper.setAcceleration(200);
  stepper.moveTo(0);
  stepper.runToPosition();
}

void loop() {
  Serial.println("Enter target position: ");
  while (Serial.available() == 0) {
    // wait for input
  }
  String input = Serial.readStringUntil('\n');

  if(isValidNumber(input)) {
    Serial.println("Moving to " + input);
    stepper.moveTo(input.toInt());
    stepper.runToPosition();
  } else {
    Serial.println("Error: Target input is not a number.");
  }
}
```

--------------------------------------------------------------------------------

## G. Eksperimen Karakterisasi, Analisis Tambahan, & Pembelajaran

### 1. Karakterisasi Sensor: PIR vs IR Obstacle
Dalam rancangan awal, sistem direncanakan menggunakan ultrasonik sebagai penentu jarak dan PIR sebagai konfirmator. Namun dalam proses eksperimen ditemukan karakteristik fundamental yang membedakan kinerja keduanya pada skenario *Smart Gate*:
* **PIR (HC-SR501):** Mendeteksi gerakan secara inframerah pasif (panas tubuh). Kelemahan utamanya adalah sensor ini **dapat mendeteksi semua objek yang memancarkan inframerah**. Sehingga jika ada sumber panas lain di sekitar (misal: matahari, lampu, atau kendaraan lain), sensor ini akan memberikan sinyal HIGH yang menandakan adanya objek bergerak. Selain itu, PIR memiliki *warm-up delay* 30-60 detik saat pertama kali dihidupkan, sehingga tidak dapat merespons secara instan.
* **IR Obstacle (FC-51):** Beroperasi secara aktif memancarkan dan menerima cahaya. Sensor ini **hanya merespons objek yang berada di depannya dalam jarak pendek**. Outputnya LOW saat ada objek, dan HIGH saat normal. Sensor ini sangat reaktif dan dapat digunakan untuk konfirmasi cepat (via *Interrupt*) tanpa jeda pemanasan.

**Tabel Komparasi Perbedaan Mendasar Sensor:**
| Aspek | PIR (HC-SR501) | IR Obstacle (FC-51) |
| ------ | ------ | ------ |
| Jenis infrared | **Pasif** — hanya menerima radiasi panas dari objek | **Aktif** — memancarkan cahaya infrared sendiri, deteksi via pantulan |
| Yang dideteksi | Gerakan objek bersuhu (manusia, hewan, kendaraan panas) | Keberadaan objek apa pun di depan sensor, panas atau tidak |
| Objek diam | Tidak terdeteksi setelah beberapa saat (output balik LOW) | Tetap terdeteksi selama objek ada di depan sensor |
| Jangkauan | Jauh (± 3–7 meter) | Pendek (umumnya < 30 cm) |
| Gangguan utama | Perubahan suhu lingkungan, sumber panas lain | Warna permukaan objek, cahaya infrared lingkungan (matahari) |

> **Intinya:** PIR mendeteksi *"ada sesuatu yang bergerak dan hangat"*, sedangkan IR Obstacle mendeteksi *"ada sesuatu secara fisik di depan sensor, bergerak atau tidak"*.

### 2. Analisis: Konsekuensi Jika FC-51 Diganti PIR
Pada sistem integrasi, FC-51 berperan sebagai konfirmasi kedua setelah ultrasonik mendeteksi jarak dekat (`state TUNGGU_IR`). Jika FC-51 diganti dengan PIR:
1. **Logika interrupt perlu diubah** — trigger `FALLING` (FC-51, LOW = objek) tidak cocok dengan PIR yang HIGH saat gerakan terdeteksi; perlu diubah ke `RISING` plus penanganan reset tambahan.
2. **Objek diam berisiko tidak terkonfirmasi** — desain `TUNGGU_IR` mengandalkan sensor yang responsif terhadap objek diam, sementara PIR membutuhkan gerakan kontinu untuk tetap HIGH.
3. **Delay warm-up tambahan** — PIR butuh 30–60 detik stabilisasi saat power-on, FC-51 tidak.
4. **Risiko false trigger lebih tinggi** — PIR bisa salah mendeteksi sumber panas lain yang lewat di sekitar sensor, bukan hanya kendaraan yang dituju.

> **Kesimpulan:** Mengganti FC-51 dengan PIR pada skenario ini **menurunkan keandalan sistem**, karena kelemahan utama PIR (tidak mendeteksi objek diam) bertentangan dengan fungsi state `TUNGGU_IR` yang butuh konfirmasi objek yang mungkin sedang diam menunggu.

### 3. Karakterisasi Aktuator: Servo vs Stepper
* **Servo SG90:** Sangat ideal untuk palang gerbang prototipe berskala kecil. Sifat kendalinya yang *closed-loop* dan pemanfaatan PWM langsung membuatnya dapat bergerak dari sudut 0° ke 90° dengan satu baris perintah `gerbang.write()`.
* **Stepper 28BYJ-48:** Menawarkan kestabilan torsi saat diam (*holding torque*) namun memerlukan manajemen langkah (*step*) yang spesifik. Pada eksperimen serial, sistem membuktikan bahwa pergerakan bergantung pada pulsa `runToPosition()` yang harus selalu dipanggil untuk mencapai posisi yang ditargetkan tanpa *feedback* posisi asli.

**Tabel Komparasi Teknis Aktuator:**
| Karakteristik | Stepper Motor (28BYJ-48) | Servo Motor (SG90) |
| ------ | ------ | ------ |
| Sistem Kontrol | Open-loop (tidak tahu posisi sendiri) | Closed-loop (tahu posisi sendiri) |
| Torsi & RPM | Torsi tinggi di RPM rendah, turun di RPM tinggi | Torsi stabil dari RPM rendah sampai tinggi |
| Kecepatan | Rendah–sedang (umumnya < 1000–2000 RPM) | Sangat tinggi dan responsif |
| Risiko Operasional | Step bisa terskip jika beban terlalu berat (kalibrasi kacau) | Mengoreksi posisi otomatis jika ada hambatan |
| Holding Torque & Arus | Bisa menahan beban tinggi saat diam, tapi terus menarik arus walau diam | Menarik arus hanya saat bergerak atau ada beban eksternal |

**Tabel Plus Minus Implementasi Aktuator:**
| Aspek | Servo (SG90) | Stepper (28BYJ-48) |
| ------ | ------ | ------ |
| Kapan dipakai | Gerbang kecil/ringan, gerak cepat, sudut terbatas 0°–180° | Gerbang lebih berat, butuh torsi tahan beban atau presisi posisi tinggi |
| Kenapa tidak dipakai | Torsi kecil, tidak cocok beban berat atau gerbang lebar | Perlu driver tambahan, kabel lebih banyak, kode lebih kompleks |
| Kemudahan implementasi | Sangat simpel — 1 kabel sinyal, langsung `write(sudut)` | Perlu 4 pin + library AccelStepper, perlu kalibrasi posisi awal (*homing*) |
| Risiko di lapangan | Minim, karena closed-loop | Step bisa terlewat jika gerbang macet, posisi jadi tidak sinkron tanpa sensor tambahan |
| Ketergantungan sensor | Tidak perlu | Idealnya perlu limit switch sebagai referensi posisi awal |

> **Kesimpulan Implementasi Aktuator:** Untuk sistem palang gerbang cepat (*boom gate*) seperti pada prototipe ini, aktuator servo jauh lebih efisien dan praktis dalam penulisan algoritma karena tidak membutuhkan sensor mekanis limit-switch tambahan untuk kalibrasi (*homing*) lokasi 0° seperti halnya Stepper. Stepper lebih unggul jika skala gerbang diperbesar, namun membutuhkan penanganan tambahan untuk kalibrasi dan potensi *missed-step*.

### 4. Kendala Eksperimental dan Pembelajaran
Modul PIR HC-SR501 yang diuji coba mengalami kerusakan — output secara konstan *latch* di posisi HIGH meskipun pengaturan sensitivitas maupun mode jumper (L/H) telah dikalibrasi. Hal ini menyebabkan sistem merespons seolah selalu ada objek bergerak.

**Solusi & Pembelajaran:**
Berdasarkan analisa kebutuhan state `TUNGGU_IR` (menunggu konfirmasi sebelum gerbang benar-benar terbuka) yang mensyaratkan deteksi objek yang sedang diam (menunggu), kelompok memutuskan mengganti strategi deteksi menjadi kombinasi **PING))) Ultrasonik + FC-51 IR Obstacle** untuk sistem integrasi akhir. IR FC-51 bertindak sangat reaktif (dihubungkan via *Interrupt*) yang merespons seketika tanpa jeda pemanasan (*warm-up delay* 30-60 detik) yang dialami PIR.

Kendala ini menjadi pembelajaran penting mengenai pentingnya **validasi hardware** sebelum diintegrasikan ke sistem yang lebih besar, serta pertimbangan karakteristik sensor (pasif vs aktif, objek diam vs bergerak) dalam menentukan desain *state machine* yang sesuai.

<<<<<<< HEAD
### 5. Kesimpulan Final
1. Kombinasi sensor **PING))) Ultrasonik + FC-51 IR Obstacle** terbukti paling ideal, tangguh (*robust*), dan andal dibanding PIR untuk kasus deteksi objek pada gerbang otomatis, terutama karena kemampuannya mendeteksi objek diam serta meminimalisir status deteksi yang salah (*false positive*).
2. Untuk aktuator, **Servo SG90** lebih praktis dan memadai untuk prototipe skala kecil. Sementara **Stepper Motor 28BYJ-48** berpotensi lebih unggul pada skala yang lebih besar dengan beban yang lebih berat, dengan catatan membutuhkan penanganan tambahan untuk kalibrasi posisi (*homing*) dan driver tambahan.
=======
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

---

## Catatan Tambahan
- **IR di Wokwi bukan FC-51 asli** 

Receiver IR Wokwi hanya memberi pulsa LOW singkat saat ada sinyal (nilai Command dan Address tidak berpengaruh), sedangkan FC-51 asli tetap LOW selama ada objek. Interrupt `gerakan_ada` bisa menangkap pulsa yang singkat itu, tetapi ini tidak merepresentasikan behavior receiver IR asli.

- **Penggunaan PWM**

Proyek ini tidak memakai PWM hardware. Tidak ada `analogWrite()` sama sekali.

PWM sering disebut dalam pembahasan servo karena sinyal kontrol servo secara konsep mirip PWM, yaitu pulsa diulang setiap 20 ms (50 Hz) dan lebar pulsa menentukan sudut (sekitar 0,5 ms untuk 0° dan 1,5 ms untuk 90°). Namun fitur PWM Arduino tidak kita gunakan. Library `Servo` membangkitkan pulsa itu sendiri dengan interrupt Timer1 pada pin 9, dan kita hanya memanggil `gerbang.write(sudut)`. Timer2 dipakai sebagai penghitung waktu 1 ms (mode CTC), bukan untuk menghasilkan PWM.

Efek sampingnya, library Servo menonaktifkan `analogWrite()` di pin 9 dan 10, dan karena Timer2 dipakai untuk tick, `analogWrite()` di pin 3 dan 11 juga akan bentrok.
>>>>>>> 8058ecf36d05d691c663ae4a65b0b913279b7acf
