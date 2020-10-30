#!/bin/bash

REPO="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
export ESD_IMAGE_NAME=esd-2020:latest
export ESD_CONTAINER_NAME=esd
export PATH=$PATH:$REPO/scripts

