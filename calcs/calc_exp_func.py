#!/usr/bin/python3
import numpy as np

MAX_VALUE = 1040384

min_diff = MAX_VALUE
best_coeffs = 0

print("[Calculate the exponential function that best fit for the bin width]\n")

coeffs = 0.16
while coeffs < 0.2:
    total = 0
    for x in range (0, 65):
        total = total + np.exp(coeffs * x) + 511

    diff = abs(total - MAX_VALUE)
    if (diff < min_diff):
        min_diff = diff
        best_coeffs = coeffs
        print(f"coeffs: {coeffs}, total:{total}, diff:{diff}")
    coeffs += 0.01


print("[Result]")
print(f"f(x) = 511 + e^({best_coeffs} * x)")