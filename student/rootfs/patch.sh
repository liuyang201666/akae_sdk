#!/bin/sh
# By Panda.Guo at 2011-11-03
# 对busybox 1.19.3 打补丁

patch -p1 < ../busybox-1.19.3-getty.patch 
patch -p1 < ../busybox-1.19.3-modinfo.patch 

