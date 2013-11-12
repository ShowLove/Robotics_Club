#!/bin/bash
D=$(date +%Y%m%d%H%M)
./baseline_program_d | tee logs/baseline-$D.log
