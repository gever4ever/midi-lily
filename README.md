# MIDI controller based on ATmega32U4 MCU

From concept to product - documentation and learning process

Documentating all the way of the supply chain from proof of concept to prototyping with custom breakout boards and producing the final PCB including design, layout, assembly and testing.

(Planned) Features:
- Sends standard MIDI messages 
- Rotary Encoders with pushbuttons
- Cherry MX switch matrix
- 45mm slider potentiometers
- RGB LED fader feedback

Specifications:
- ATmega32U4 MCU
- Native USB device and MIDI HID compliant 
- Type-C plug with over-current protection 
- MCP23S17 SPI GPIO expander for rotary encoders
- CD4067 16-channels multiplexer faders
- Shift registers for switch matrix scanning

Libraries used:
- FastLED
- ResponsiveAnalogRead
- MCP23X17
- CD4067


## 08.03.2026 - POD & Prototype

Prototype board was ordered through JLCPCB containing four "modules" of a rotary encoder, fader and three MX switches.  
Using a Teensy 2.0 and breakout boards of CD4067 multiplexer and SPI controlled MCP23S17 GPIO expander all functions and MIDI interface were confirmed working without noticeable delay using MIDI-OX on Windows monitoring incoming MIDI messages.

The setup was meant to have sub-optimal routing of signals and grounding to get a somewhat of an idea on how robust the signal integrity is.

Using 22AWG solid core wires for interconnects and a "prema-board" with 2.54mm socket headers soldered on.

### Prototype Photos

![Test Setup](photo/IMG20260302224214_01.jpg)

![Side view](photo/IMG20260302223935.jpg)
