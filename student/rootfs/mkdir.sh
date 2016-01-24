#!/bin/sh

# By Panda.Guo at 2011-11-03 create
# 创建根文件系统的基本目录结构
# 
mkdir rootfs
cd rootfs
mkdir  bin dev 
mkdir -p etc/init.d etc/network/if-down.d etc/network/if-post-down.d 
mkdir -p etc/network/if-pre-up.d etc/network/if-up.d
mkdir lib mnt opt proc sbin srv sys tmp 
mkdir -p usr/bin usr/lib usr/sbin var/run
