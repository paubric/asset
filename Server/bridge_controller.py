import requests

def read_all(device):
    r = requests.get('http://'+device+'.local'+'/arduino')
    return r.json()

def digital_read_all(device):
    r = requests.get('http://'+device+'.local'+'/arduino'+'/digital')
    return r.json()

def digital_read_pin(device, pin):
    r = requests.get('http://'+device+'.local'+'/arduino'+'/digital/'+str(pin))
    return r.json()

def digital_write_pin(device, pin, value):
    r = requests.get('http://'+device+'.local'+'/arduino'+'/digital/'+str(pin)+'/'+str(value))
    print(r.text)
    return r.json()

def analog_read_all(device):
    r = requests.get('http://'+device+'.local'+'/arduino'+'/analog/')
    return r.json()

def analog_read_pin(device, pin):
    r = requests.get('http://'+device+'.local'+'/arduino'+'/analog/'+str(pin))
    return r.json()

def analog_write_pin(device, pin, value):
    r = requests.get('http://'+device+'.local'+'/arduino'+'/analog/'+str(pin)+'/'+str(value))
    return r.json()

def send_pulse(device, pin, value):
    r = requests.get('http://'+device+'.local'+'/arduino'+'/pulse/'+str(pin)+'/'+str(value))
    return r.json()

def set_pin(device, pin, value):
    r = requests.get('http://'+device+'.local'+'/arduino'+'/mode/'+str(pin)+'/'+str(value))
    return r.json()
