#!/usr/bin/python3
import sys

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

argc = len(sys.argv)
if (argc != 2):
    print("Usage: calc_bin.py <size>")
else:
    print(sys.argv[1], bin(int(sys.argv[1])))
