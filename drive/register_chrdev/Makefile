######################################################
#*Function:driver makefile
#*Author:asmcos@gmail.com
#*Date:2005-08-24
#*$Id:Makefile,v 1.3 2006/05/17 12:37:19 asmcos Exp $
######################################################

ifneq ($(KERNELRELEASE),)
#kbuild part of makefile
obj-m:=register_dev.o register_dev_new.o
else


#KERNELSRC	:=/usr/src/linux-headers-3.0.0-12-generic
#KERNELSRC :=/home/akaedu/Board/student/kernel/linux-2.6.27
KERNELSRC :=/home/lenovo/linux-kernel/linux-3.10.90
#KERNELSRC :=/home/lenovo/linux-kernel/tmp/linux-3.10.90

#KERNELSRC :=/home/bryan/akaedu/工作相关/18期驱动实验相关/秦老师资料/kernel-2.6.27-android_ok

modules:
	gcc app.c -o app
	make -C $(KERNELSRC) SUBDIRS=$(PWD) $@

clean:
	rm -f *.o *.ko *.mod.c *~ app

endif
