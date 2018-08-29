import bridge_controller as bc
import time

device = 'RTUa'

bc.set_pin(device, pin, value)
bc.read_all(device)
bc.digital_read_all(device)
bc.digital_read_pin(device, pin)
bc.digital_write_pin(device, pin, value)
bc.analog_read_all(device)
bc.analog_read_pin(device, pin)
bc.analog_write_pin(device, pin, value)
bc.send_pulse(device, pin, value)
