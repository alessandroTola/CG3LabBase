#!/bin/sh

sudo apt-get install qtcreator qt5-default -y
sudo apt-get install libboost-all-dev libcgal-dev libgmp-dev libqglviewer-dev -y
sudo apt-get install doxygen libeigen3-dev -y

read -p "Do you wish to install LibIGL? (Y/n)" yn
    case $yn in
        [Yy]* ) 
            cd /usr/include/
            sudo git clone --recursive https://github.com/libigl/libigl.git
            echo "export LIBIGL=/usr/include/libigl" >> ~/.profile
            cd ~
            break;;
        * ) exit;;
    esac
