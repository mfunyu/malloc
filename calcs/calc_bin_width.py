#!/usr/bin/python3
import numpy as np

def exponential(x):
    return np.exp(0.199 * x) + 511

shift = 9
width = 1 << shift
total = 1536

func_py = ""
func_c = ""

def add_func_py(total, shift, index):
    max_value = total >> shift
    global func_py
    func_py += f"    if size >> {shift} <= {max_value}:\n"
    func_py += f"        return ((size >> {shift}) + {index - max_value})\n"

def add_func_c(total, shift, index):
    max_value = total >> shift
    global func_c
    func_c += f"	if (size >> {shift} <= {max_value})\n"
    func_c += f"		return ((size >> {shift}) + {index - max_value});\n"

for i in range(0, 64):
    y = exponential(i)
    if (y >= (1 << (shift + 1))): # width is greater than next shift value
        shift += 1
        width = 1 << shift
        print(f"[{i}] width: {width} (shift: {shift}) {total} ~ {total + width}")
        add_func_py(total - 1, shift - 1, i - 1)
        add_func_c(total - 1, shift - 1, i - 1)
    else:
        print(f"  [{i}] width: {width} (shift: {shift}) {total} ~ {total + width}")

    total += width

print(func_py)
print(func_c)