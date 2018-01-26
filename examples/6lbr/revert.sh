#!/bin/sh

echo  tiktok007 |sudo -S rm /usr/bin/gcc 
sudo ln -s /usr/bin/gcc-4.8 /usr/bin/gcc

sudo rm /usr/bin/ld
sudo ln -s /usr/bin/ld.bfd /usr/bin/ld
