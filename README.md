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
- RTU Board porting (Yun -> Yun Mini) (1/2)
- Dashboard Interface (1/3)
- Create function that reads all analog ins
- Brand identity
## Day 4 - Aug 30
- RTU Board porting (Yun -> Yun Mini) (2/2)
- Dashboard Interface (2/3)
- Motor and sensor interfacing
## Day 5 - Aug 31
- Dashboard Interface (3/3)
- Create function that sends N pulses
- Recive and send live camera input(2/2)
- Test graphs with sensors


# Bridge API Documentation
## set_pin(device, pin, value)
```
GET http://device.local/arduino/mode/pin/value
```
## read_all(device)
```
GET http://device.local/arduino/all/0
```
## digital_read_all(device)
```
GET http://device.local/arduino/digital/-1
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
GET http://device.local/arduino/analog/-1
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
# How to install
1. Get a particle accelerator
![](https://github.com/paubric/parrot/blob/master/Parrot.png)


