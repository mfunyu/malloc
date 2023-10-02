#!/bin/bash

echo run $1

make correction FILENAME="test$1.c"
