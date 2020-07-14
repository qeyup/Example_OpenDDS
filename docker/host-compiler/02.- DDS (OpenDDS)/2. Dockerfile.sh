#!/bin/bash

# Fix missing
apt update --fix-missing
apt upgrade -y


# Download requiremsts
PACKAGES=()
PACKAGES+=(google-mock)
PACKAGES+=(libgtest-dev)
PACKAGES+=(libxerces-c-dev)
PACKAGES+=(libssl-dev)
PACKAGES+=(perl-base)
PACKAGES+=(perl-modules)
apt install -y ${PACKAGES[@]}



# Download and extract
wget https://github.com/objectcomputing/OpenDDS/archive/DDS-3.13.3.tar.gz
tar xf DDS-3.13.3.tar.gz
cd OpenDDS-DDS-3.13.3



# Configure and build
OPTIONS+=()
OPTIONS+=(--std=c++11)
OPTIONS+=(--no-tests)
OPTIONS+=(--static)
OPTIONS+=(--prefix=${OPENDDS_PATH})
OPTIONS+=(-no-debug)
OPTIONS+=(--optimize)
#OPTIONS+=(--doc-group)
./configure ${OPTIONS[@]}
make


# Install
make install


# Symbolic link install
MakeSymbolicInstall "/usr/" true $OPENDDS_PATH lib include bin
MakeSymbolicInstall "/usr/" true $OPENDDS_PATH/share/ace bin
MakeSymbolicInstall "/usr/" true $OPENDDS_PATH/share/dds bin