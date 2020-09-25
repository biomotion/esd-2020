FROM ubuntu:14.04
LABEL maintainer="bensonpuppy1997@gmail.com"

RUN apt-get update && \
    apt-get install -y \
        tar \
        vim \
        git \
        build-essential \
        parted \
        dosfstools

COPY ./lab1_package.tar /data/
COPY ./arm-env /root/.arm-env

WORKDIR /data
RUN tar -xvf /data/lab1_package.tar && \
    tar -xvjf /data/lab1_package/gcc-linaro-5.3-20170216.tar.bz2 -C / && \
    echo "source /root/.arm-env" >> /root/.bashrc
