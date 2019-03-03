# PARROT
Particle Accelerator Observation Tool

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
![](https://github.com/paubric/parrot/blob/master/Parrot.png)


