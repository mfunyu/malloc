#!/usr/bin/python3
import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

def bin(size):
    if size >> 9 <= 34:
        return ((size >> 9) + -3)
    if size >> 10 <= 22:
        return ((size >> 10) + 14)
    if size >> 11 <= 16:
        return ((size >> 11) + 25)
    if size >> 12 <= 11:
        return ((size >> 12) + 33)
    if size >> 13 <= 9:
        return ((size >> 13) + 39)
    if size >> 14 <= 8:
        return ((size >> 14) + 44)
    if size >> 15 <= 7:
        return ((size >> 15) + 48)
    if size >> 16 <= 7:
        return ((size >> 16) + 52)
    if size >> 17 <= 6:
        return ((size >> 17) + 56)
    return (63)

def exponential(x):
    return np.exp(0.199 * x) + 511

def plot_exponential_function():
    # Generate x values
    x_values = np.linspace(0, 64, 100)  # Adjust the range as needed

    # Calculate corresponding y values
    y_values = exponential(x_values)

    # Plot the function
    plt.plot(x_values, y_values, label='$e^{0.19x}$')

def exponential_function(x, a, b, c):
    return a * np.exp(b * x) + c

def plot_exponential_approximation(x_values, y_values, label):
    # Initial guess for parameters
    initial_guess = [1.0, 0.0, 1.0]

    # Fit the exponential curve
    params, covariance = curve_fit(exponential_function, x_values, y_values, p0=initial_guess, maxfev=10000)

    # Generate x values for the curve
    x_curve = np.linspace(min(x_values), max(x_values), 300)

    # Plot the original data points
    plt.plot(x_values, y_values, '. ', label=label)

    # Plot the exponential approximation curve
    
    plt.plot(x_curve, exponential_function(x_curve, *params), label=f'{label} (exponential approximation)', linestyle='-')

def show():
    plt.xlabel('index')
    plt.ylabel('bin width')
    plt.xlim(0)
    plt.ylim(0)
    plt.title('Exponential Approximation Plot')

    plt.legend()
    plt.show()

def plot(x_values, y_values):
    # Example data
    x_data = np.array(x_values)
    y_data = np.array(y_values)

    # Label
    label = 'bin width'

    # Exponential approximation curve plot function call
    plot_exponential_function()
    plot_exponential_approximation(x_data, y_data, label)


def func():
    prev_index = -1
    prev_value = 1024
    x_values = []
    y_values = []

    for size in range(1024, 1040385):
        index = bin(size)
        if (index != prev_index):
            width = size - prev_value
            x_values.append(index)
            y_values.append(width)
            print(f"[{index}] width: {width} value: {size} ~")
            prev_index = index
            prev_value = size

    plot(x_values, y_values)

func()
show()
