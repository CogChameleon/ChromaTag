#!/bin/bash
# This script runs the ChromaTag detector on the provided starter data using the provided config file
#
# This script makes two assumptions
#    1. you are running it from inside .../ChromaTag/Scripts
#    2. you build the source by making a directory called build
../build/bin/Run_ChromaTag_Detector -c ../Configs/chromatags_config.yml -F ../Data -x 2