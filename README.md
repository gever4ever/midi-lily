# MIDI controller based on ATmega32U4 MCU

From concept to product - documentation and learning process

Going all the way in the supply chain from proof of concept to prototyping with custom breakout boards and producing the final PCB including design, layout, assembly and testing.
In this process ...

Features:
- Sends standard MIDI messages 
- Rotary Encoders with pushbuttons
- Cherry MX switch matrix
- 45mm slider potentiometers
- RGB LED fader feedback

Specifications:
- ATmega32U4 MCU
- Native USB device and HID compliant 
- Type-C plug with over-current protection 
- MCP23S17 SPI GPIO expander for rotary encoders
- CD4067 16-channels multiplexer for switch matrix and faders

Libraries used:
- FastLED
- ResponsiveAnalogRead
- MCP23X17
- CD4067a
