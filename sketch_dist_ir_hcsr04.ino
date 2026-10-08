#include <avr/interrupt.h>
#include <Servo.h>

// ---------- pin ----------
const byte pin_ir    = 2;   // OUT FC-51 (LOW = objek terdeteksi)
const byte pin_echo  = 4;   // ECHO HC-SR04
const byte pin_trig  = 5;   // TRIG HC-SR04
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
  // 1. kirim pulsa trigger 10 us
  // HC-SR04 butuh minimal 10 us (PING pakai 5 us)
  digitalWrite(pin_trig, LOW);
  delayMicroseconds(2);
  digitalWrite(pin_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pin_trig, LOW);

  // 2. ukur pulsa echo di pin terpisah
  unsigned long durasi = pulseIn(pin_echo, HIGH, 25000); // timeout 25 ms
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
  pinMode(pin_trig, OUTPUT);
  pinMode(pin_echo, INPUT);

  Serial.begin(115200);
  gerbang.attach(pin_servo);

  attachInterrupt(digitalPinToInterrupt(pin_ir), gerakan_terdeteksi, FALLING);
  atur_timer();

  tutup_gerbang();
  Serial.println("siap");
}

void loop() {
  uint32_t sekarang = baca_tick();

  // 1. ukur jarak secara berkala (bukan tiap loop)
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
      if (!mobil_dekat) { // mobil pergi sebelum ir aktif
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
      if (mobil_dekat) { // ada mobil lagi, jangan tutup
        status = TERBUKA;
      } else if (sekarang - waktu_jeda_mulai >= jeda_tutup_ms) {
        tutup_gerbang();
        status = TUTUP;
      }
      break;
  }
}