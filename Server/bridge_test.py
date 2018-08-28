import bridge_controller as bc
import time

start = time.time()
bc.digital_write('RTU1', 13, 1)
end = time.time()
print(end-start)
