#!/bin/bash

mkdir -p build &&
cd build &&
printf "\n--- CONFIGURING ---\n" &&
cmake .. &&
printf "\n--- BUILDING ---\n" &&
make -j14 &&
cd ..