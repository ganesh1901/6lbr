#!/bin/sh

export PATH=/opt/analogics/am335x_sk/toolchain/usr/bin:/opt/analogics/am335x_sk/toolchain/usr/sbin:$PATH

echo tiktok007 |sudo -S rm /usr/bin/gcc 
sudo ln -s /opt/analogics/am335x_sk/toolchain/usr/bin/armv7l-analogics-linux-gnueabi-gcc  /usr/bin/gcc

sudo rm /usr/bin/ld
sudo ln -s /opt/analogics/am335x_sk/toolchain/usr/bin/armv7l-analogics-linux-gnueabi-ld.bfd /usr/bin/ld
