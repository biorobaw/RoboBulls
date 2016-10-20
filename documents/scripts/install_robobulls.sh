#!/bin/bash
#RoboBulls installation script.
#Installs the SSL RefBox, grSim simulator, and downloads RoboBulls2
INSTALL_PATH=$HOME #Where to install the folders to
INCLUDE_PATH="/usr/local/include" #Where to install library header files

function echogreen() { RED='\e[32m'; NC='\033[0m'; printf "${RED}$1${NC}\n"; }

#All preliminary libraries
echogreen "Installing Preliminary libraries"
sudo apt-get install -y git g++ make libprotobuf-dev protobuf-compiler libgtkmm-2.4-dev qt5-default qtbase5-private-dev qt5-qmake qtcreator libqt5serialport5-dev libsdl2-dev minicom build-essential cmake libqt4-dev libgl1-mesa-dev libglu1-mesa-dev libode-dev

#Download and install Kalman filter library
if [ ! -d $INCLUDE_PATH/kalman ]; then
	echo "Kalman library not installed; installing to $INCLUDE_PATH"
	cd /tmp
	wget "http://downloads.sourceforge.net/project/kalman/kalman/1.3/kalman-1.3.zip?r=&ts=1462934052&use_mirror=tenet" -O kalman.zip
	unzip -o -q kalman.zip
	sudo cp -R kalman/kalman $INCLUDE_PATH
	rm -rf kalman/ kalman.zip
else
    echogreen "Kalman Library already installed in $INCLUDE_PATH"
fi

#RefBox installation, if not downloaded
if [ ! -d $INSTALL_PATH/ssl-refbox ]; then
    echo "RefBox not installed; installing in $INSTALL_PATH"
    cd $INSTALL_PATH
    git clone https://github.com/Hawk777/ssl-refbox.git
    cd ssl-refbox && make
else
    echogreen "RefBox already installed (directory exists)"
fi

#grSim installation, if not downloaded
if [ ! -d $INSTALL_PATH/grSim ]; then
    echo "grSim not installed; installing in $INSTALL_PATH"
    cd /tmp
    wget http://vartypes.googlecode.com/files/vartypes-0.7.tar.gz
    tar xfz vartypes-0.7.tar.gz
    cd vartypes-0.7 && mkdir build && cd build && cmake .. && make && sudo make install
    rm -rf vartypes-0.7
    cd $INSTALL_PATH
    git clone https://github.com/mllofriu/grSim.git
    cd grSim && mkdir build && cd build && cmake .. && make
else
    echogreen "grSim already installed (directory exists)"
fi

#Download and install RoboBulls
if [ ! -d $INSTALL_PATH/robobulls2 ]; then
    echo "Downloading and installing RoboBulls in $INSTALL_PATH"
    cd $INSTALL_PATH
    git clone https://github.com/mllofriu/robobulls2.git
    cd robobulls2
else
    echogreen "RoboBulls2 already installed (directory exists)"
fi

#We need to set up the USB rules to recognize the Xbee
sudo cp 99-usb-serial.rules /etc/udev/rules.d/99-usb-serial.rules
sudo cp minirc.dfl /etc/minicom/minirc.dfl