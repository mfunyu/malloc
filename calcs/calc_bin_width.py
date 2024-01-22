#!/usr/bin/python3
import numpy as np

def exponential(x):
    return np.exp(0.19 * x) + 511

shift = 9
width = 1 << shift * 1
total = 1024

func_py = ""
func_c = ""

def add_func_py(total, i, shift):
    max_value = total >> shift
    global func_py
    func_py += f"if size >> {shift} <= {max_value}:\n"
    func_py += f"    return ((size >> {shift}) + {i - max_value})\n"

def add_func_c(total, i, shift):
    max_value = total >> shift
    global func_c
    func_c += f"if (size >> {shift} <= {max_value})\n"
    func_c += f"    return ((size >> {shift}) + {i - max_value});\n"

for i in range(0, 67):
    y = exponential(i + 1)
    if (y >= (1 << (shift + 1))):
        print(f"[{i}] width: {width} (shift: {shift}) {total} ~ {total + width - 1}")
        add_func_py(total, i - 1, shift)
        add_func_c(total, i - 1, shift)
        shift += 1
        width = 1 << shift
    total += width

print(func_py)
print(func_c)