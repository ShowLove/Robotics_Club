#!/bin/bash
gcc -lm -O3 hydrosimp.c -o hydrosimp && ./hydrosimp > hydrosimp.log
