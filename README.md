# PARROT
Particle Accelerator Observation Tool

# History
## Day 1 - Aug 27
- Brainstorming
## Day 2 - Aug 28
- RTU configuration
- RTU programming for bridge communication
- Master programming for bridge communication
- Testing Master-RTU communcation link
## Day 3 - Aug 29
- RTU Board porting (Yun -> Yun Mini)
- Dashboard Interface
- Create function that reads all analog ins
- Create function that sends N pulses
- Brand identity
- Motor and sensor interfacing
- Recive and send live camera input

# Bridge API Documentation
## set_pin(device, pin, value)
```
GET http://device.local/arduino/mode/pin/value
```
## read_all(device)
```
GET http://device.local/arduino
```
## digital_read_all(device)
```
GET http://device.local/arduino/digital
```
## digital_read_pin(device, pin)
```
GET http://device.local/arduino/digital/pin
```
## digital_write_pin(device, pin, value)
```
GET http://device.local/arduino/digital/pin/value
```
## analog_read_all(device)
```
GET http://device.local/arduino/analog
```
## analog_read_pin(device, pin)
```
GET http://device.local/arduino/analog/pin
```
## analog_write_pin(device, pin, value)
```
GET http://device.local/arduino/analog/pin/value
```
## send_pulse(device, pin, value)
```
GET http://device.local/arduino/pulse/pin/value
```
