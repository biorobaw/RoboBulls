#!/bin/bash
#RoboBulls installation script.
#Installs the SSL RefBox, grSim simulator, and downloads RoboBulls2
INSTALL_PATH=$HOME #Where to install the folders to

function echored() { RED='\033[0;31m'; NC='\033[0m'; printf "${RED}$1${NC}\n"; }

#All preliminary libraries
echored "Installing Preliminary libraries"
sudo apt-get install -y git g++ g++-4.8 make libprotobuf-dev protobuf-compiler libgtkmm-2.4-dev qt5-default qt5-qmake qtcreator libsdl2-dev minicom build-essential cmake libqt4-dev libgl1-mesa-dev libglu1-mesa-dev libprotobuf-dev libode-dev

#RefBox installation, if not downloaded
if [ ! -d $INSTALL_PATH/ssl-refbox ]; then
    echored "RefBox not installed; installing in $INSTALL_PATH"
    cd $INSTALL_PATH
    git clone https://github.com/Hawk777/ssl-refbox.git
    cd ssl-refbox && make
else
    echored "RefBox already installed (directory exists)"
fi

#grSim installation, if not downloaded
if [ ! -d $INSTALL_PATH/grSim ]; then
    echored "grSim not installed; installing in $INSTALL_PATH"
    cd /tmp
    wget http://vartypes.googlecode.com/files/vartypes-0.7.tar.gz
    tar xfz vartypes-0.7.tar.gz
    cd vartypes-0.7 && mkdir build && cd build && cmake .. && make && sudo make install
    rm -rf vartypes-0.7
    cd $INSTALL_PATH
    git clone https://github.com/mani-monaj/grSim
    cd grSim && mkdir build && cd build && cmake .. && make
else
    echored "grSim already installed (directory exists)"
fi

#Download and install RoboBulls
if [ ! -d $INSTALL_PATH/robobulls2 ]; then
    echored "Downloading and installing RoboBulls in $INSTALL_PATH"
    cd $INSTALL_PATH
    git clone https://github.com/mllofriu/robobulls2.git
    cd robobulls2
    #We need to set up the USB rules to recognize the Xbee
    sudo cp scripts/99-usb-serial.rules /etc/udev/rules.d/99-usb-serial.rules
else
    echored "RoboBulls2 already installed (directory exists)"
fi
