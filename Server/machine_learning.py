from sklearn.neural_network import MLPRegressor
import numpy as np

x = [[1, 1, 2, 1], [1, 2, 1, 1]]
y = [[0.5, 1.5], [1.5, 0.5]]

reg = MLPRegressor()

reg = reg.fit(x, y)

test_x = [[1, 1, 1.8, 1]]
test_y = reg.predict(test_x)

print(test_y)
