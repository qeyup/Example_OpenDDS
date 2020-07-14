#!/bin/bash


# Update
apt update --fix-missing
apt upgrade -y


# Install required
apt install -y \
               wget \
               xz-utils \
               make \
               m4 \
               build-essential \
               gawk \
               git \
               texinfo \
               bison \
               file \
