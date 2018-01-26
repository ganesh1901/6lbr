#!/bin/sh

echo tiktok007 |sudo -S rm /usr/bin/gcc 
sudo ln -s /usr/bin/arm-linux-gnueabihf-gcc /usr/bin/gcc

sudo rm /usr/bin/ld
sudo ln -s /usr/bin/arm-linux-gnueabihf-ld.bfd /usr/bin/ld
