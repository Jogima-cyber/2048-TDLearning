# 2048-TDLearning
## Introduction
Personal implementation in C++ of http://www.cs.put.poznan.pl/mszubert/pub/szubert2014cig.pdf. It's an algorithm that learns by itself to solve the 2048 game. It doesn't use deep learning (aka. neural networks). But it learns by itself using the Bellman equations.

## Installation
To clone this project type in command line the following :
'''
git clone jogyma-cyber/2048-TDLearning
'''

I've used pytorch for c++ to save the final trained weights in a format readible in python. To install libtorch type the following in command line in the root directory of this repository :
'''
wget https://download.pytorch.org/libtorch/nightly/cpu/libtorch-shared-with-deps-latest.zip
unzip libtorch-shared-with-deps-latest.zip
'''

## Building
To build the executable type in commnd line at the root of the repository :
'''
mkdir build
cd build
cmake -DCMAKE_PREFIX_PATH=/absolute/path/to/libtorch ..
cmake --build . --config Release
'''

## Launching train
Go in command line to the build folder created during building and launch TDL2048 (or TDL2048.exe if you're on windows)

## Issues
I've followed the guidelines of https://pytorch.org/cppdocs/installing.html#minimal-example in order to install the torch dependency, and if you are on windows, maybe you need to change little things according to this tutorial.
