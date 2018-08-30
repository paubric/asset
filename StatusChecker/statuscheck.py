import os,time

hostnames = [
    "rtua.local"
]

while True:
    for device in hostnames:
        response = os.system("ping -c 1 "+device+" > /dev/null 2>&1")
        print(device,"is online") if response == 0 else print(device,"is offline")
    time.sleep(15)