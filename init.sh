#!/bin/bash

mkdir -p build/picotool &&
cd build/picotool &&
printf "\n--- CONFIGURING ---\n" &&
cmake -DPICO_SDK_PATH=../../pico-sdk ../../picotool &&
printf "\n--- BUILDING ---\n" &&
make -j14 &&
cd ../..