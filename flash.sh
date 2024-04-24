#!/bin/bash

printf "\n--- FLASHING ---\n" &&
./build/picotool/picotool load build/main.uf2 -f