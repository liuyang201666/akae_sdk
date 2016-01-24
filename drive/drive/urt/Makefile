######################################################
#*Function:driver makefile
#*Author:asmcos@gmail.com
#*Date:2005-08-24
#*$Id:Makefile,v 1.3 2006/05/17 12:37:19 asmcos Exp $
######################################################

ifneq ($(KERNELRELEASE),)
#kbuild part of makefile
obj-m:=urt.o
else


#KERNELSRC :=/home/bryan/akaedu/工作相关/18期驱动实验相关/秦老师资料/kernel-2.6.27-android_ok
#KERNELSRC :=/usr/src/linux-headers-2.6.32-40-generic
KERNELSRC :=/home/lenovo/2440/linux-2.6.27

modules:
	make -C $(KERNELSRC) SUBDIRS=$(PWD) $@
	arm-linux-gcc app.c -o app

clean:
	rm -f *.o *.ko *.mod.c *~

endif
