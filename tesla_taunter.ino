#include <ELECHOUSE_CC1101_SRC_DRV.h>
//-------------------------------------------------------------------------------
// Microcontroller: ESP32-DevKitC core board ESP32 V4 development board ESP32-WROOM-32D/U
//
// For more information:
//
// https://www.espressif.com/en/products/hardware/esp32/overview
// https://lastminuteengineers.com/esp32-arduino-ide-tutorial/#esp32-development-board-pinout
// https://europe1.discourse-cdn.com/arduino/original/4X/4/3/d/43d8b7d07e610d074a3a616821786e26aa7bc92f.jpeg
//
//                                                +---------------------------+
//                                                |      __    __    __ __    |
//                                                |     |  |__|  |__|  |  |   |
//                                                |     |      k       |  |   |
//                                 VDD 3.3 Volt/ -| [3V3] +-----------+ [GND] | -  / Ground (GND)
//                                           EN/ -| [EN]  |           |  [23] | 36 / GPIO23 / V_SPI_D / MOSI
//    SENSVP/INPUT ONLY/RTC_GPIO0/ADC1_0/GPIO36/ 5| [UP]  |           |  [22] | 39 / GPIO22 / V_SPI_WP / SCL / RTS 0
//    SENSVN/INPUT ONLY/RTC_GPIO3/ADC1_3/GPIO39/ 8| [UN]  |           |  [TX] | 41 / GPIO01 / TXD 0 / CLK3
//           INPUT ONLY/RTC_GPIO4/ADC1_6/GPIO34/10| [34]  |           |  [RX] | 40 / GPIO03 / RXD 0 / CLK2
//           INPUT ONLY/RTC_GPIO5/ADC1_7/GPIO35/11| [35]  |           |  [21] | 42 / GPIO23 / VSPI_HD / SDA
//       XTAL32P/TOUCH9/RTC_GPIO9/ADC1_4/GPIO32/12| [32]  |           | [GND] | -  / Ground (GND)
//       XTAL32N/TOUCH8/RTC_GPIO8/ADC1_5/GPIO33/13| [33]  |___________|  [19] | 38 / GPIO19 / V_SPI_Q / MISO / CRS 0
//                 DAC1/RTC_GPIO6/ADC2_8/GPIO25/14| [25]                 [18] | 35 / GPIO18 / V_SPI_CLK / SCK
//                 DAC2/RTC_GPIO7/ADC2_9/GPIO26/15| [26]                  [5] | 34 / GPIO05 / V_SPI_CS0 / SS
//              TOUCH7/RTC_GPIO17/AFC2_7/GPIO27/16| [27]                 [17] | 27 / GPIO17 / TXD 2
//     HSPI_CLK/TOUCH6/RTC_GPIO16/ADC2_6/GPIO14/17| [14]                 [16] | 25 / GPIO16 / TRXD 2
//       HSPI_Q/TOUCH5/RTC_GPIO15/ADC2_5/GPIO12/18| [12]                  [4] | 24 / GPIO4 / ADC2_0 / HSPI_HD / TOUCH 0 / RTC GPIO10
//                                 GROUND GND /  -| [GND]                 [C] | 23 / GPIO0 / ADC2_1 / CLK1 / TOUCH 1 / RTC GPIO11
//      HSPI_ID/TOUCH4/RTC GPIO14/ADC2_4/GPIO13/20| [13]                  [2] | 22 / GPIO2 / ADC2_2 / HSPI_WP0 / TOUCH 2 / RTC GPIO11
//   HSPI_DATA2/FLASH/D2/SD_DATA2/RXD1/GPIO09 / 28| [D2]                 [15] | 21 / GPIO15 / ADC2_3 / HSPI_CS0 / TOUCH 3 / RTC GPIO13
//   HSPI_DATA3/FLASH D3/SD DATA3/TXD1/GPIO10 / 29| [D3]                 [D1] | 33 / GPIO8 / CTS 2 / HSPI_DATA1 / SD_DATA1 / FLASH D1
//      HSPI_CMD/FLASH CMD/SD_CMD/RTS1/GPIO11 / 30| [CMD]                [DD] | 32 / GPIO7 / RTS 2 / HSPI_DATA0 / SD DATA0 / FLASH D0
//                                 VIN 5 Volt /  -| [5V]                [CLK] | 31 / GPIO6 / CTS 1 / HSPI_CLK / SD CLK / FLASH SCK
//                                                |                           |
//                                                |    O     | USB |     O    | 
//                                                |          -------          |
//                                                +---------------------------+
// ESP32 Power Pins
// ----------
// 
// VIN - VIN pin can be used to directly supply the ESP32 and its peripherals, if you have a regulated 5V voltage source.
// 3V3 - 3.3V pin is the output of an on-board voltage regulator. This pin can be used to supply power to external components.
// GND - Ground pin(s)                                    ___
//                                                       : _ : 
// Transmitter:     CC1101 8-pins                        |___|
//                                                +-----------------+
//                                                |      | | |      |
//                                                |   _         _   |
//                                                |  (_)       (_)  |
//                                                |                 |
//                                                |                 |
//                                                |   ____          |
//                                                |  |   ´|   __    |
//                                                |  |____|  |__|   |
//                                                |                 |
//                                                |    O  O  O  O   | 
//                                                |    O  O  O  O   |
//                                                +-----------------+
//                                                     2  4  6  8
//                                                     1  3  5  7
// CC1101       <->  Arduino
//------------------------------------
// 1. GND        -> GND top right 
// 2. Vdd 3.3V   -> VDD 3.3 Volt top left 
// 3. GDO0       -> [16] 25 GPIO16
// 4. CSN        -> [5]  34 GPIO5  (SS) 
// 5. SCK        -> [18] 35 GPIO18 (SCK)
// 6. MOSI       -> [23] 36 GPIO23 (MOSI)
// 7. MISO/GDO1  -> [19] 38 GPIO19 (MISO)
// 8 DGO2        -> [4]  24 GPIO4 
// ------------------------------------
//
// Light emitter diode
// ------------------------------------
// On ARduino pin [17] | 27 / GPIO17 solder a 220 Ohm resister to a LED and then from the LED to GROUND.
//
// Setup Arduino before compiling:
//
// 1. File -> Preferences -> Additional Board Manager URLs -> Add this: https://dl.espressif.com/dl/package_esp32_index.json
// 2. Tools -> Manage Libraries -> Search for "elechouse" and install SmartRC-CC1101-Driver-Lib by LSatan ( 2.5.7 or newer)
// 3. Tools -> Board MAnager -> Search for esp32 -> Install esp32 by Espressif Systems (3.0.5 or newer)
// 4. Tools -> Select esp32 -> ESP32 Dev Module
// 5. Tools -> Select CPU Frequency 80 MHz (WiFi/BT)
// 6. Tools -> Select Flash Frequency 40 MHz
// 7. Tools -> Select Flash Mode -> DIO
// 8. Select Sketch - Upload
//
//-------------------------------------------------------------------------------
// Definer CC1101 pins
//-------------------------------------------------------------------------------
#define LED_PIN 17                                   // Define LED pin
#define CC1101_GDO0_PIN   16                         // GDO0 -> GPIO16
#define CC1101_CS_PIN     5                          // CSN  -> GPIO5 (Chip Select)
#define CC1101_SCK_PIN    18                         // SCK  -> GPIO18
#define CC1101_MOSI_PIN   23                         // MOSI -> GPIO23
#define CC1101_MISO_PIN   19                         // MISO -> GPIO19
#define CC1101_GDO2_PIN   4                          // GDO2 -> GPIO4
//-------------------------------------------------------------------------------
// Define signal to transmit
//-------------------------------------------------------------------------------
const uint16_t pulseWidth = 400;                     // Mikroseconds
const uint16_t messageDistance = 23;                 // Milliseconds
const uint8_t transmissions = 5;                     // Repeat 5 tinmes
const uint8_t sequence[] = { 
  0x02,0xAA,0xAA,0xAA,                               // Preamble 26 bits by repeating 1010
  0x2B,                                              // Sync byte
  0x2C,0xCB,0x33,0x33,0x2D,0x34,0xB5,0x2B,0x4D,0x32,0xAD,0x2C,0x56,0x59,0x96,0x66,
  0x66,0x5A,0x69,0x6A,0x56,0x9A,0x65,0x5A,0x58,0xAC,0xB3,0x2C,0xCC,0xCC,0xB4,0xD2,
  0xD4,0xAD,0x34,0xCA,0xB4,0xA0
};
const uint8_t messageLength = sizeof(sequence);
//-------------------------------------------------------------------------------
// Setup
//-------------------------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("[+] Serial initialized");
  pinMode(LED_PIN, OUTPUT);                          // Initialize LED-pin
  digitalWrite(LED_PIN, LOW);                        // Turn off LED 
  pinMode(CC1101_GDO0_PIN, OUTPUT);                  // Initialze GDO0 as output to data
  digitalWrite(CC1101_GDO0_PIN, LOW);                // Set GDO0 to OFF
  ELECHOUSE_cc1101.setSpiPin(CC1101_SCK_PIN, CC1101_MISO_PIN, CC1101_MOSI_PIN, CC1101_CS_PIN);
  if (ELECHOUSE_cc1101.getCC1101()) {                // Detect CC1101
   Serial.println("[+] CC1101 detected");
   ELECHOUSE_cc1101.Init();                          // Initialize CC1101
   Serial.println("[+] CC1101 initialized");
  } else {
   Serial.println("[!] CC1101 connection ERROR");
   while (true) {
     yield();
   }
  }
  // Configure CC1101
  ELECHOUSE_cc1101.setModulation(2);                 // 2 = ASK/OOK modulation
  ELECHOUSE_cc1101.setMHZ(433.92);                   // Set frequency to 433.92 MHz
  ELECHOUSE_cc1101.setPA(10);                        // Set TxPower to max (10 dBm)
  ELECHOUSE_cc1101.setDRate(2.5);                    // Set Data speed 2.5 kBaud
  ELECHOUSE_cc1101.setDeviation(0);                  // Set Deviation is 0 for OOK
  ELECHOUSE_cc1101.setPktFormat(3);                  // Select Async seriel mode
  ELECHOUSE_cc1101.setSyncMode(0);                   // Select No preamble/sync
  ELECHOUSE_cc1101.SpiWriteReg(CC1101_IOCFG0, 0x2D); // Set GDO0 to data input mode
  Serial.println("[+] CC1101 configured for asynchronous serial mode");
  Serial.println("[+] Transmitter is ready");
}
//-------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------
void sendSequence() {
  for (uint8_t i = 0; i < messageLength; i++) {
    sendByte(sequence[i]);
  }
  
}
//-------------------------------------------------------------------------------
void sendByte(uint8_t dataByte) {
  for (int8_t bit = 7; bit >= 0; bit--) {            // MSB first
    digitalWrite(CC1101_GDO0_PIN, (dataByte & (1 << bit)) ? HIGH : LOW);
    delayMicroseconds(pulseWidth);
  }
}
//-------------------------------------------------------------------------------
// Loop
//-------------------------------------------------------------------------------
void loop() {
  // 433.92 MHz
  ELECHOUSE_cc1101.setMHZ(433.92);                   // Set frequency to 433.92 MHz
  Serial.println(F("[+] Transmitting 433.92 MHz"));
  digitalWrite(LED_PIN, HIGH);                       // Turn on LED
    ELECHOUSE_cc1101.SpiStrobe(CC1101_STX);          // Start TX mode
  for (uint8_t t = 0; t < transmissions; t++) {
    sendSequence();
    delay(messageDistance);
  }
  ELECHOUSE_cc1101.SpiStrobe(CC1101_SIDLE);          // Stop TX mode
  digitalWrite(LED_PIN, LOW);                        // Turn iff LED
  Serial.println(F("[+] Transmission completed"));
  // 315.00 Mhz
  ELECHOUSE_cc1101.setMHZ(315.00);                   // Set frequency to 315.00 MHz
  Serial.println(F("[+] Transmitting 315.00 MHz"));
  digitalWrite(LED_PIN, HIGH);                       // Turn on LED
    ELECHOUSE_cc1101.SpiStrobe(CC1101_STX);          // Start TX mode
  for (uint8_t t = 0; t < transmissions; t++) {
    sendSequence();
    delay(messageDistance);
  }
  ELECHOUSE_cc1101.SpiStrobe(CC1101_SIDLE);          // Stop TX mode
  digitalWrite(LED_PIN, LOW);                        // Turn iff LED
  Serial.println(F("[+] Transmission completed"));
  delay(1000);                                       // Sleep for 1 Second
}