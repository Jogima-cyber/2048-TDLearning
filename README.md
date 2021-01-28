# 2048-TDLearning
## Introduction
Personal implementation in C++ of http://www.cs.put.poznan.pl/mszubert/pub/szubert2014cig.pdf. It's an algorithm that learns by itself to solve the 2048 game. It doesn't use deep learning (aka. neural networks). But it learns by itself using the Bellman equations.
The authors of this paper already gave away an opensource implementation in java : https://github.com/mszubert/2048. Mines is ten times slower but goes directly to the point and thus I believe is more understandable.

## Installation
To clone this project type in command line the following :
```
git clone jogyma-cyber/2048-TDLearning
```

I've used pytorch for c++ to save the final trained weights in a format readible in python. To install libtorch type the following in command line in the root directory of this repository :
```
wget https://download.pytorch.org/libtorch/nightly/cpu/libtorch-shared-with-deps-latest.zip
unzip libtorch-shared-with-deps-latest.zip
```

## Building
To build the executable type in commnd line at the root of the repository :
```
mkdir build
cd build
cmake -DCMAKE_PREFIX_PATH=/absolute/path/to/libtorch ..
cmake --build . --config Release
```

## Launching train
Go in command line to the build folder created during building and launch TDL2048 (or TDL2048.exe if you're on windows)

## Issues
I've followed the guidelines of https://pytorch.org/cppdocs/installing.html#minimal-example in order to install the torch dependency, and if you are on windows, maybe you need to change little things according to this tutorial.

## Results
So I was able to reproduce the results of the paper, my training time was about 12 hours, when training time of paper was around 2 hours. So there is need for optimization. If you have ideas, don't hesitate to share them with me in the issues/comments. Agent trained on 100 000 games with a learning rate of 0.01. Thanks to github pages, this repository is also accessible as a website ! You can watch the agent performance at https://jogima-cyber.github.io/2048-TDLearning/ 

## Load weights in python
When training is done, a file called "TDL2048_weights.zip" is created in the build folder. It countains the trained weights in a format readable by torch in python. If you wish to read them in Python, just use :
```
import torch
weights = torch.load(file);
```
## Other interesting work on the subject
 - An pure Monte Carlo Tree Search (MCTS) AI solving 2048 (there is no learning, unlike in MCTS algorithms of DeepMind) : https://jupiter.xtrp.io/
