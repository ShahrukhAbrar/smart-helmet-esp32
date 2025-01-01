#include <HardwareSerial.h>
#include <TinyGPSPlus.h>
HardwareSerial gpsSerial(1); // UART1 for GPS 

//GPS
double longtitude = 0.0;
double latitude = 0.0;
double Prevlongtitude = 0;
double Prevlatitude = 0;
unsigned long previousMillis = 0;
const unsigned long interval = 300000;
// Create an instance of the TinyGPSPlus library
TinyGPSPlus gps;

/*
void setup() {
  Serial.begin(115200);
  gpsSerial.begin(9600, SERIAL_8N1, 16, 17); // RX = GPIO16, TX = GPIO17

  disableUnnecessaryNMEASentences();
  enablePowerSavingMode();
  disableSBAS();
  setUltraLowUpdateRate();
}
*/
void disableUnnecessaryNMEASentences() {
  // Disable all NMEA sentences except RMC
  byte disableGGA[] = { 0xB5, 0x62, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x00, 0x00, 0xFA, 0x0F }; // GGA off
  byte disableGSA[] = { 0xB5, 0x62, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x02, 0x00, 0xFC, 0x13 }; // GSA off
  byte disableGSV[] = { 0xB5, 0x62, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x03, 0x00, 0xFD, 0x15 }; // GSV off
  byte disableGLL[] = { 0xB5, 0x62, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x01, 0x00, 0xFB, 0x11 }; // GLL off
  byte disableVTG[] = { 0xB5, 0x62, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x05, 0x00, 0xFF, 0x19 }; // VTG off

  gpsSerial.write(disableGGA, sizeof(disableGGA));
  gpsSerial.write(disableGSA, sizeof(disableGSA));
  gpsSerial.write(disableGSV, sizeof(disableGSV));
  gpsSerial.write(disableGLL, sizeof(disableGLL));
  gpsSerial.write(disableVTG, sizeof(disableVTG));

  Serial.println("Disabled unnecessary NMEA sentences.");
}

void setUltraLowUpdateRate() {
  byte setRate[] = {
    0xB5, 0x62, 0x06, 0x08, // UBX-CFG-RATE header
    0x06, 0x00,             // Payload length
    0x10, 0x27,             // Update interval = 10000 ms (0x2710)
    0x01, 0x00,             // Measurement rate (1)
    0x01, 0x00              // Navigation rate (1)
  };

  // Calculate checksum
  uint8_t CK_A = 0;
  uint8_t CK_B = 0;
  for (int i = 2; i < sizeof(setRate); i++) {  // Start from 2 to skip header
    CK_A += setRate[i];
    CK_B += CK_A;
  }

  byte checksum[] = {CK_A, CK_B};
  
  // Append checksum to the end of the packet
  gpsSerial.write(setRate, sizeof(setRate));
  gpsSerial.write(checksum, sizeof(checksum));
  Serial.println("Set update rate to 10 seconds.");
}


void enablePowerSavingMode() {
  byte psmCommand[] = {
    0xB5, 0x62, 0x06, 0x11, // UBX-CFG-RXM header
    0x02, 0x00,             // Payload length (2 bytes)
    0x01, 0x01,             // Power Save Mode (1=PSM, 1=autonomous)
    0x1F, 0x21              // Checksum
  };
  gpsSerial.write(psmCommand, sizeof(psmCommand));
  Serial.println("Power Saving Mode enabled.");
}

void disableSBAS() {
  byte sbasCommand[] = {
    0xB5, 0x62, 0x06, 0x16, // UBX-CFG-SBAS header
    0x08, 0x00,             // Payload length (8 bytes)
    0x00, 0x00, 0x00, 0x00, // SBAS disabled, Integrity disabled
    0x00, 0x00, 0x00, 0x00, // Reserved
    0x2A, 0x13              // Checksum
  };
  gpsSerial.write(sbasCommand, sizeof(sbasCommand));
  Serial.println("SBAS disabled.");
}

void readGPSDataOnce() {
  while (gpsSerial.available() > 0) {
    char c = gpsSerial.read();
    if (gps.encode(c)) {
      if (gps.location.isUpdated()) {
        longtitude = gps.location.lng();
        latitude = gps.location.lat();

        if (longtitude > 0 || latitude > 0) {
          Prevlongtitude = longtitude;
          Prevlatitude = latitude;
          Serial.println("[+] : Location Updated!");
          Serial.print("Latitude: ");
          Serial.println(latitude, 6);
          Serial.print("Longitude: ");
          Serial.println(longtitude, 6);
        } else {
          Serial.println("[-] : GPS has not found a Fix!");
        }
        return;
      }
    }
  }
}
