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
## Read All
```
GET http://rtu-name.local/arduino
```
## Digital Read All
```
GET http://rtu-name.local/arduino/digital
```
## Digital Read Pin
```
GET http://rtu-name.local/arduino/digital/pin
```
## Digital Write Pin
```
GET http://rtu-name.local/arduino/digital/pin/value
```
## Analog Read All
```
GET http://rtu-name.local/arduino/analog
```
## Analog Read Pin
```
GET http://rtu-name.local/arduino/analog/pin
```
## Analog Write Pin
```
GET http://rtu-name.local/arduino/analog/pin/value
```
## Send Pulse
```
GET http://rtu-name.local/arduino/pulse/pin/value
```
## Set Pin
```
GET http://rtu-name.local/arduino/mode/pin/value
```
