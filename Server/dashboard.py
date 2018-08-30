from flask import Flask
import bridge_controller as bc
import time
import threading
import json

app = Flask(__name__)
device = 'rtua'
sensor_data = []
visible_item_count = 30

@app.route('/dummy_sensors')
def send_sensors():
    dict = {
        'data': sensor_data[-visible_item_count:]
    }
    return json.dumps(dict)

@app.route('/dummy_sensors')
def send_dummy_sensors():
    dict = {
        'data': [[123, 125, 142, 124], [254, 257, 254, 261]]
    }
    return json.dumps(dict)

def regular_get_sensors():
    while True:
        print('[*] Updating sensor_data...')
        new_data_array = bc.analog_read_all(device)['analog']
        new_data = [new_data_array[i]['value'] for i in range(1, 5)]
        sensor_data.append(new_data)
        print('[*] Updated sensor_data:', sensor_data)
        time.sleep(1)

#threading.Thread(target=regular_get_sensors).start()

if __name__ == '__main__':
    app.run()
