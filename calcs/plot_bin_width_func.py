#!/usr/bin/python3
import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

def bin(size):
    if size >> 9 <= 34:
        return ((size >> 9) + -3)
    if size >> 10 <= 23:
        return ((size >> 10) + 14)
    if size >> 11 <= 16:
        return ((size >> 11) + 26)
    if size >> 12 <= 12:
        return ((size >> 12) + 34)
    if size >> 13 <= 9:
        return ((size >> 13) + 40)
    if size >> 14 <= 8:
        return ((size >> 14) + 45)
    if size >> 15 <= 8:
        return ((size >> 15) + 49)
    if size >> 16 <= 7:
        return ((size >> 16) + 53)
    if size >> 17 <= 7:
        return ((size >> 17) + 57)
    return (0)

def exponential(x):
    return np.exp(0.19 * x) + 511

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
    prev = -1
    val = 1024
    x_values = []
    y_values = []

    for i in range(1024, 1040385):
        index = bin(i)
        if (index != prev):
            width = i - val
            x_values.append(index)
            y_values.append(width)
            print(f"[{index}] width: {width} value: ~ {i - 1}")
            prev = index
            val = i

    plot(x_values, y_values)

func()
show()
