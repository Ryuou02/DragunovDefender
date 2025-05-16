# Purpose
This project was made as part of Smart India Hackathon 2024. It made it past the preliminary rounds however did not clear further stages. 
This was meant to be for a system placed on an FPGA that detects the direction of the gunshot with data from an array of 6 microphones.

# Progress
Gunshot detection was tried with basic FFTs and Neural Networks. 
Neural Networks were considered due to the requirement to process continuous input and considering an FPGA's parallel processing capabilities (which could have potentially ran neurons in parallel). 
FFTs were not favoured over the neural networks due to hardware and dataset limitations of not having a audio that accurately represents gunshot sounds based on volume of the gunshot which led us to believing that FFTs are unfit for the purpose.
Neural Networks worked without much false positives and almost 0 false negatives.

# contents of this repository
This repository contains neural networks that were to be trained to detect gunshots. neural network.h contains a neural network made in C++ if it interests you.
