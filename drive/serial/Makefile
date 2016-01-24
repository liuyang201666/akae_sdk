######################################################
#*Function:driver makefile
#*Author:asmcos@gmail.com
#*Date:2005-08-24
#*$Id:Makefile,v 1.3 2006/05/17 12:37:19 asmcos Exp $
######################################################

ifneq ($(KERNELRELEASE),)
#kbuild part of makefile
obj-m:=serial.o
else


#KERNELSRC :=/usr/src/linux-headers-3.0.0-12-generic
KERNELSRC :=/home/lenovo/2440/linux-2.6.27

modules:
	make -C $(KERNELSRC) SUBDIRS=$(PWD) $@
	arm-linux-gcc app.c -o app -g -Wall

clean:
	rm -f app *.o *.ko *.mod.c *~

endif
