from sklearn.neural_network import MLPRegressor
from sklearn.linear_model import LinearRegression
import numpy as np
import matplotlib.pyplot as plt
import os
import time
import pickle
from itertools import repeat

def analyze(filename):
    reg = None
    with open('model.pk', 'rb') as pickle_file:
        reg = pickle.load(pickle_file)
    file = open(filename, 'rb')
    lines = file.readlines()[12:1036]
    lines = [x for item in lines for x in repeat(item, 4)]
    lines = [np.log10(int(line.decode('utf-8')[:-2])) for line in lines]
    for i in range(len(lines)):
        if lines[i] == float('-inf'):
            lines[i] = 0
    pred = reg.predict(np.asarray(lines).reshape(1, -1))
    return lines, pred

def mae(a, b):
    a = np.asarray(a)
    b = np.asarray(b)
    mae = np.sum(np.absolute((a.astype("float") - b.astype("float"))))/(len(a)*len(a[0]))
    return mae

def train():
    files = sorted(os.listdir('./Data'))
    print(files)

    x = []
    for filename in files:
        file = open('./Data/' + filename, 'rb')
        lines = file.readlines()[16:4112]
        lines = [np.log10(int(line.decode('utf-8')[:-2])) for line in lines]

        #print(lines)
        #print(len(lines))
        x.append(lines)

        #plt.plot(range(0, 4096), lines)
        #plt.show()

    for i in range(len(x)):
        for j in range(len(x[i])):
            if x[i][j] == float('-inf'):
                x[i][j] = 0

    x = np.asarray(x)
    # SiO2, Na2O, CaO, Al2O3
    y = [[72, 14, 12, 2],
        [72, 14, 12, 2],
        [75, 5, 8, 6],
        [70, 12, 8, 4],
        [75, 9, 7, 4],
        [75, 8, 9, 4],
        [65, 20, 7, 2]]

    x_train = x
    x_test = x
    y_train = y

    mlp = MLPRegressor()
    lin = LinearRegression()
    reg = lin.fit(x_train, y_train)

    with open('model.pk', 'wb') as pickle_file:
        pickle.dump(reg, pickle_file)

    start = time.time()
    y_test = reg.predict(x_test)
    end = time.time()
    print(mae(y, y_test))
    print(end-start)
