# Laporan Spesifikasi dan Karakterisasi Komponen Smart Gate System

**Focus Group - Kelompok 2**

- Ahmad Fatih Faizi (2206819344)
- Arief Ridzki Darmawan (2306210115)
- Hadyan Fachri (2306245030)

---

## A. Pendahuluan

Smart Gate System adalah prototi sistem tertanam yang bertujuan untuk mendeteksi keberadaan dan jarak objek menggunakan kombinasi sensor ultrasonik dan sensor infrared (IR Obstacle). Sistem ini memproses data sensor untuk kemudian menggerakkan motor servo sebagai aktuator yang membuka atau menutup gerbang secara otomatis.

Laporan ini bertujuan untuk mengeksplorasi karakteristik sensor ultrasonik, PIR, dan infrared dalam mendeteksi objek, mengintegrasikannya dengan aktuator (servo dan stepper motor), serta menganalisis spesifikasi teknis dan antarmuka komponen tersebut dengan mikrokontroler Arduino Mega.

---

## B. Prinsip Kerja Setiap Komponen

1. **Ultrasonic Sensor (PING))) / HC-SR04)**
   Mengukur jarak dengan memancarkan gelombang suara ultrasonik (umumnya 40 kHz) lewat _transmitter_, lalu menghitung waktu tempuh gelombang tersebut hingga memantul kembali dan diterima oleh _receiver_. Waktu tempuh ini dikonversi menjadi jarak. Pada modul PING))), proses _trigger_ dan penerimaan pantulan (echo) menggunakan satu pin sinyal yang sama secara bergantian.

2. **IR Obstacle (FC-51)**
   Modul ini memancarkan cahaya infrared lewat LED _transmitter_ (bening) dan mendeteksi pantulannya menggunakan fototransistor _receiver_ (gelap/hitam). Jika ada objek dalam jangkauan jarak pendek di depannya, cahaya memantul dan terdeteksi. Output sensor akan berubah menjadi LOW melalui komparator (LM393).

3. **PIR (HC-SR501)**
   Mendeteksi perubahan radiasi inframerah pasif yang dipancarkan oleh objek bersuhu (seperti tubuh manusia atau mesin kendaraan) yang bergerak. Output berupa sinyal digital HIGH saat gerakan terdeteksi, dan kembali LOW setelah durasi waktu (delay) yang diatur terlampaui. Sensor mendeteksi _gerakan_, bukan eksistensi objek diam.

4. **Servo Motor (SG90)**
   Motor aktuator _closed-loop_ yang dikendalikan melalui sinyal PWM (Pulse Width Modulation). Lebar pulsa yang dikirimkan menentukan posisi absolut sudut motor (umumnya 0°–180°). Ini membuat servo sangat responsif untuk membuka/menutup palang tanpa perlu perhitungan putaran rumit.

5. **Stepper Motor (28BYJ-48)**
   Motor aktuator _open-loop_ yang bergerak dalam langkah-langkah diskrit (step) dengan cara mengaktifkan elektromagnet (koil) secara berurutan. Posisinya bersifat relatif terhadap posisi sebelumnya. Untuk menggerakkannya, mikrokontroler mengirimkan pola sinyal ke IC _Driver_ (ULN2003).

---

## C. Spesifikasi Teknis

Berikut adalah spesifikasi teknis dari komponen yang digunakan (referensi dari standar _datasheet_ pabrikan):

### 1. Parallax PING))) Ultrasonic Sensor

| Parameter             | Spesifikasi                                        |
| --------------------- | -------------------------------------------------- |
| Tegangan Kerja (VCC)  | 5V DC                                              |
| Arus Operasional      | ~30 mA                                             |
| Rentang Jarak Deteksi | 2 cm - 300 cm                                      |
| Frekuensi Suara       | 40 kHz                                             |
| Antarmuka Komunikasi  | 1 Pin TTL (Trigger dan Echo berbagi pin yang sama) |
| Indikator             | LED (menyala saat memancarkan gelombang)           |

### 2. IR Obstacle Sensor (FC-51)

| Parameter             | Spesifikasi                                               |
| --------------------- | --------------------------------------------------------- |
| Tegangan Kerja (VCC)  | 3.3V - 5V DC                                              |
| Rentang Jarak Deteksi | 2 cm - 30 cm (dapat diatur via potensiometer)             |
| Sudut Deteksi         | ± 35 derajat                                              |
| Sinyal Output         | Digital TTL (LOW saat mendeteksi objek, HIGH saat normal) |
| Chip Komparator       | LM393                                                     |

### 3. PIR Sensor (HC-SR501)

| Parameter                    | Spesifikasi                                        |
| ---------------------------- | -------------------------------------------------- |
| Tegangan Kerja (VCC)         | 4.5V - 20V DC                                      |
| Tegangan Output              | Digital 3.3V (HIGH/Terdeteksi), 0V (LOW/Normal)    |
| Rentang Jarak Deteksi        | 3 meter - 7 meter (dapat diatur via potensiometer) |
| Sudut Deteksi                | < 100 derajat (bentuk kerucut)                     |
| Waktu Tunda (Delay Time)     | 0.3 detik hingga ~5 menit (dapat diatur)           |
| Waktu Blokir (Blockade Time) | 2.5 detik (default)                                |

### 4. Micro Servo Motor (SG90)

| Parameter            | Spesifikasi                                     |
| -------------------- | ----------------------------------------------- |
| Tegangan Kerja       | 4.8V - 6.0V DC                                  |
| Torsi (Stall Torque) | 1.8 kg-cm (pada 4.8V)                           |
| Kecepatan Operasi    | 0.12 detik / 60 derajat (pada 4.8V tanpa beban) |
| Sudut Putaran        | 0 - 180 derajat                                 |
| Sinyal Kontrol       | PWM (Periode: 20 ms / 50 Hz)                    |
| Rentang Pulsa        | 1 ms (0°) hingga 2 ms (180°)                    |

### 5. Stepper Motor (28BYJ-48) & Driver ULN2003

| Parameter                  | Spesifikasi                                          |
| -------------------------- | ---------------------------------------------------- |
| Tegangan Kerja             | 5V DC                                                |
| Tipe / Fase                | Unipolar / 4 Fase                                    |
| Rasio Gear Reduksi         | 1 / 64                                               |
| Langkah per Revolusi       | 2048 (Full-step) / 4096 (Half-step)                  |
| Sudut per Langkah (Stride) | 5.625° / 64 (sekitar 0.088° per langkah _half-step_) |
| IC Driver                  | ULN2003 (Darlington Transistor Array)                |

---

## D. Koneksi dengan Fitur Mikrokontroler

Agar sistem berjalan optimal, antarmuka ke mikrokontroler memanfaatkan berbagai fitur perangkat keras khusus Arduino Mega:

1. **Timer/Counter (Timer2):** Digunakan untuk melakukan _tick_ internal (menghitung waktu tanpa _blocking_) melalui fitur **Interrupt (ISR)**. Timer2 diatur pada mode CTC dengan prescaler 64 untuk memicu _interrupt_ secara presisi setiap 1 milidetik (`ISR(TIMER2_COMPA_vect)`).
2. **External Interrupt (INT4 pada Pin 2):** Pin sinyal FC-51 dihubungkan ke fitur External Interrupt Arduino (`attachInterrupt`). Mode diatur ke `FALLING` agar mikrokontroler segera merespons seketika saat sinyal berubah dari HIGH ke LOW (objek terdeteksi) tanpa perlu melakukan _polling_ secara terus-menerus di fungsi `loop()`.
3. **GPIO Digital (I/O):** Digunakan untuk dua hal:
   - **Membaca jarak:** Pin 4 Arduino difungsikan secara bergantian sebagai _OUTPUT_ (untuk trigger pulsa 5µs) dan sebagai _INPUT_ untuk membaca durasi gema pantulan via fungsi `pulseIn()`.
   - **Menggerakkan Stepper:** Menggunakan 4 pin GPIO sebagai _OUTPUT_ digital untuk mengaktifkan koil stepper motor secara berurutan.
4. **Hardware PWM / Timer1:** Library `Servo.h` secara internal menggunakan Timer mikrokontroler untuk membangkitkan sinyal PWM dengan frekuensi spesifik (50 Hz) di Pin 9 untuk menahan dan menggerakkan Servo pada sudut yang diinginkan.
5. **UART / Serial Communication:** Menggunakan hardware UART (melalui kabel USB ke PC) pada _baud rate_ 115200 (untuk sistem gate) dan 9600 (untuk eksperimen stepper) untuk keperluan _debugging_, input bilangan via Serial Monitor, dan laporan status.

---

## E. Rangkaian Antarmuka

### 1. Smart Gate System (Utama)

| Arduino Mega | Modul       | Tersambung ke Pin   | Keterangan                       |
| ------------ | ----------- | ------------------- | -------------------------------- |
| 5V           | PING)))     | VCC                 | Catu daya sensor jarak           |
| 5V           | FC-51       | VCC                 | Catu daya sensor IR              |
| 5V           | Servo SG90  | Kabel Merah         | Catu daya motor servo            |
| GND          | Semua Modul | GND / Kabel Coklat  | _Common Ground_                  |
| D2           | FC-51       | OUT                 | External Interrupt (FALLING)     |
| D4           | PING)))     | SIG                 | GPIO (Trigger/Echo bergantian)   |
| D9           | Servo SG90  | Kabel Kuning/Oranye | Sinyal Kontrol (PWM via Servo.h) |

### 2. Eksperimen Stepper Motor

| Arduino Mega | Modul (Driver ULN2003) | Keterangan           |
| ------------ | ---------------------- | -------------------- |
| 5V           | VCC                    | Catu daya stepper    |
| GND          | GND                    | _Common Ground_      |
| D8           | IN1                    | GPIO Output (Coil 1) |
| D9           | IN2                    | GPIO Output (Coil 2) |
| D10          | IN3                    | GPIO Output (Coil 3) |
| D11          | IN4                    | GPIO Output (Coil 4) |

---

## F. Program Dasar

### 1. Program Integrasi Smart Gate (Ultrasonic + IR + Servo)

Program ini mengimplementasikan _State Machine_ (Mesin Status) dan berbasis waktu mandiri (_non-blocking timer_).

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

---

## G. Eksperimen Karakterisasi dan Analisis

### 1. Karakterisasi Sensor: PIR vs IR Obstacle

Dalam rancangan awal, sistem direncanakan menggunakan ultrasonik sebagai penentu jarak dan PIR sebagai konfirmator. Namun dalam proses eksperimen ditemukan karakteristik fundamental yang membedakan kinerja keduanya pada skenario _Smart Gate_:

- **PIR (HC-SR501):** Mendeteksi gerakan secara inframerah pasif (panas tubuh). Kelemahan utamanya adalah sensor ini **dapat mendeteksi semua objek yang memancarkan inframerah**. Sehingga jika ada sumber panas lain di sekitar (misal: matahari, lampu, atau kendaraan lain), sensor ini akan memberikan sinyal HIGH yang menandakan adanya objek bergerak. Selain itu, PIR memiliki _warm-up delay_ 30-60 detik saat pertama kali dihidupkan, sehingga tidak dapat merespons secara instan.
- **IR Obstacle (FC-51):** Beroperasi secara aktif memancarkan dan menerima cahaya. Sensor ini **hanya merespons objek yang berada di depannya dalam jarak pendek**. Outputnya LOW saat ada objek, dan HIGH saat normal. Sensor ini sangat reaktif dan dapat digunakan untuk konfirmasi cepat (via _Interrupt_) tanpa jeda pemanasan.

**Kendala Eksperimental:** Modul PIR yang diuji coba mengalami cacat produksi (output secara konstan _latch_ di posisi HIGH), meskipun pengaturan sensitivitas maupun mode jumbper (L/H) telah dikalibrasi. Hal ini menyebabkan sistem merespons bahwa selalu ada objek.

**Solusi:** Berdasarkan analisa kebutuhan state `TUNGGU_IR` (menunggu konfirmasi sebelum gerbang benar-benar terbuka) yang mensyaratkan deteksi objek yang sedang diam (menunggu), sistem difinalisasi dengan memadukan **PING))) Ultrasonik + FC-51 IR Obstacle**. IR FC-51 bertindak sangat reaktif (dihubungkan via _Interrupt_) yang merespons seketika tanpa jeda pemanasan (warm-up delay 30-60 detik yang biasanya dialami PIR).

### 2. Karakterisasi Aktuator: Servo vs Stepper

- **Servo SG90:** Sangat ideal untuk palang gerbang prototipe berskala kecil. Sifat kendalinya yang _closed-loop_ dan pemanfaatan PWM langsung membuatnya dapat bergerak dari sudut 0° ke 90° dengan satu baris perintah `gerbang.write()`.
- **Stepper 28BYJ-48:** Menawarkan kestabilan torsi saat diam (_holding torque_) namun memerlukan manajemen langkah (step) yang spesifik. Pada eksperimen serial, sistem membuktikan bahwa pergerakan bergantung pada pulsa `runToPosition()` yang harus selalu dipanggil untuk mencapai posisi yang ditargetkan tanpa _feedback_ posisi asli.

### 3. Kesimpulan

Untuk sistem palang gerbang cepat (_boom gate_), aktuator servo jauh lebih efisien dalam hal penulisan algoritma karena tidak membutuhkan sensor mekanis limit-switch tambahan untuk kalibrasi (homing) lokasi 0° seperti halnya Stepper. Kombinasi sensor Ultrasonik dan sensor IR Obstacle terbukti paling ideal dan tangguh (_robust_) untuk menciptakan integrasi _Smart Gate_ yang aman, karena meminimalisir status deteksi yang salah (_false positive_) saat tidak ada kendaraan.
