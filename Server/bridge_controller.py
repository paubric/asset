import requests

name_map = {
    'RTU1': 'yun-mihai',
    'RTU2': 'arduinoYUN'
}

def digital_write(device, pin, value):
    r = requests.get('http://'+name_map[device]+'.local'+'/arduino'+'/digital/'+str(pin)+'/'+str(value))
    print(r.text)
    return r.json()

def digital_read(device, pin):
    r = requests.get('http://'+name_map[device]+'.local'+'/arduino'+'/digital/'+str(pin))
    return r.json()

def analog_write(device, pin, value):
    r = requests.get('http://'+name_map[device]+'.local'+'/arduino'+'/analog/'+str(pin)+'/'+str(value))
    return r.json()

def analog_read(device, pin):
    r = requests.get('http://'+name_map[device]+'.local'+'/arduino'+'/analog/'+str(pin))
    return r.json()

def set_pin(device, pin, value):
    r = requests.get('http://'+name_map[device]+'.local'+'/arduino'+'/mode/'+str(pin)+'/'+str(value))
    return r.json()

def send_pulse(device, pin):
    r = digital_write(device, pin, 1)
    r = digital_write(device, pin, 0)
    return r
