#!/usr/bin/python3
import numpy as np

MAX_VALUE = 1040384

min_diff = MAX_VALUE
best_coeffs = 0

print("[Calculate the exponential function that best fit for the bin width]\n")

def exponential(coeffs, x):
    return np.exp(coeffs * x) + 511

def get_best_coeffs(coeffs):
    print("[Find the best coeffs]")

    while 1 :
        total = 1024 + 512
        shift = 8
        width = 0 
        for x in range(0, 64):
            y = exponential(coeffs, x)

            if (y >= (1 << (shift + 1))):
                shift += 1
                width = 1 << shift
                # print(f"[{x}] width: {width} (shift: {shift}) {total} ~ {total + width}, {y}")
            
            total += width
        
        print(f"coeffs: {coeffs}, total:{total}, diff:{total - MAX_VALUE}")
        
        if (total >= MAX_VALUE):
            break

        coeffs += 0.001

    return (coeffs)

coeffs = get_best_coeffs(0.16)

print("[Result]")
print(f"f(x) = 511 + e^({coeffs} * x)")