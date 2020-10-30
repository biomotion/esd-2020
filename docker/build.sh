#!/bin/bash

wget https://github.com/biomotion/esd-2020/releases/download/v1.0/lab1_package.tar

docker build -t esd-2020:latest .

rm lab1_package.tar
