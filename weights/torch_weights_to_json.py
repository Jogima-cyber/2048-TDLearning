#!/usr/bin/env python
# coding: utf-8

import torch
import json

torch_weights = torch.load("TDL2048_weights.zip")
python_weights = torch_weights.tolist()

with open('TDL2048_weights.json', 'w') as outfile:
    json.dump(python_weights, outfile)