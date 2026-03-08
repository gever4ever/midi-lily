/*

This is for reading the encoders over the GPIO expander and the faders over the multiplexer.
For testing purposes this was all written by Gemini.
Future firmwares would be optimized and more elegant. Probably.

*/

#include <SPI.h>

// ==================================================
// ---------------- CD4067 SETUP --------------------
// ==================================================

#define MUX_SIG A8
#define MUX_S0  A4
#define MUX_S1  A5
#define MUX_S2  A6
#define MUX_S3  A7

// CD4067 Channel Mapping
#define FAD1_CH 15
#define FAD2_CH 14
#define FAD3_CH 13
#define FAD4_CH 12

int lastFaderValue[4];

// ==================================================
// ---------------- MCP23S17 SETUP ------------------
// ==================================================

#define MCP_CS   0   // P0
#define MCP_INT  5   // P5 (unused for now)

#define MCP_IODIRA 0x00
#define MCP_IODIRB 0x01
#define MCP_GPIOA  0x12
#define MCP_GPIOB  0x13

byte encoderState[4];
byte lastEncoderState[4];

// ==================================================
// -------------------- SETUP -----------------------
// ==================================================

void setup() {

  // ---- MUX Control Pins ----
  pinMode(MUX_S0, OUTPUT);
  pinMode(MUX_S1, OUTPUT);
  pinMode(MUX_S2, OUTPUT);
  pinMode(MUX_S3, OUTPUT);

  pinMode(MUX_SIG, INPUT_PULLUP);

  // ---- SPI + MCP23S17 ----
  SPI.begin();
  pinMode(MCP_CS, OUTPUT);
  digitalWrite(MCP_CS, HIGH);

  initMCP();

  // Init fader memory
  for (int i = 0; i < 4; i++) {
    lastFaderValue[i] = readMuxAnalog(FAD1_CH - i);
  }
}

// ==================================================
// -------------------- LOOP ------------------------
// ==================================================

void loop() {

  readFaders();
  readEncoders();

  while (usbMIDI.read()) {}
}

// ==================================================
// ---------------- MUX FUNCTIONS -------------------
// ==================================================

void selectMuxChannel(byte channel) {
  digitalWrite(MUX_S0, channel & 0x01);
  digitalWrite(MUX_S1, (channel >> 1) & 0x01);
  digitalWrite(MUX_S2, (channel >> 2) & 0x01);
  digitalWrite(MUX_S3, (channel >> 3) & 0x01);
  delayMicroseconds(5);
}

int readMuxAnalog(byte channel) {
  selectMuxChannel(channel);
  return analogRead(MUX_SIG);
}

bool readMuxDigital(byte channel) {
  selectMuxChannel(channel);
  return !digitalRead(MUX_SIG); // active LOW (pullups)
}

// ==================================================
// ---------------- FADERS --------------------------
// ==================================================

void readFaders() {

  byte faderChannels[4] = {FAD1_CH, FAD2_CH, FAD3_CH, FAD4_CH};

  for (int i = 0; i < 4; i++) {

    int value = readMuxAnalog(faderChannels[i]);

    if (abs(value - lastFaderValue[i]) > 4) {

      int midiValue = map(value, 0, 1023, 0, 127);
      usbMIDI.sendControlChange(20 + i, midiValue, 1);

      lastFaderValue[i] = value;
    }
  }
}

// ==================================================
// ---------------- MCP23S17 ------------------------
// ==================================================

void initMCP() {

  writeMCP(MCP_IODIRA, 0xFF); // Port A inputs
  writeMCP(MCP_IODIRB, 0xFF); // Port B inputs
}

void writeMCP(byte reg, byte value) {

  digitalWrite(MCP_CS, LOW);
  SPI.transfer(0x40); // Write opcode
  SPI.transfer(reg);
  SPI.transfer(value);
  digitalWrite(MCP_CS, HIGH);
}

byte readMCP(byte reg) {

  digitalWrite(MCP_CS, LOW);
  SPI.transfer(0x41); // Read opcode
  SPI.transfer(reg);
  byte value = SPI.transfer(0x00);
  digitalWrite(MCP_CS, HIGH);

  return value;
}

// ==================================================
// ---------------- ENCODERS ------------------------
// ==================================================

void readEncoders() {

  byte gpioA = readMCP(MCP_GPIOA);
  byte gpioB = readMCP(MCP_GPIOB);

  for (int i = 0; i < 4; i++) {

    byte A = (gpioB >> i) & 1;
    byte B = (gpioA >> i) & 1;

    byte currentState = (A << 1) | B;

    if (currentState != lastEncoderState[i]) {

      if ((lastEncoderState[i] == 0b00 && currentState == 0b01) ||
          (lastEncoderState[i] == 0b01 && currentState == 0b11) ||
          (lastEncoderState[i] == 0b11 && currentState == 0b10) ||
          (lastEncoderState[i] == 0b10 && currentState == 0b00)) {

        usbMIDI.sendControlChange(30 + i, 1, 1);
      } else {
        usbMIDI.sendControlChange(30 + i, 127, 1);
      }

      lastEncoderState[i] = currentState;
    }
  }
}
