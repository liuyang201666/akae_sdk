######################################################
#*Function:driver makefile
#*Author:asmcos@gmail.com
#*Date:2005-08-24
#*$Id:Makefile,v 1.3 2006/05/17 12:37:19 asmcos Exp $
######################################################

ifneq ($(KERNELRELEASE),)
#kbuild part of makefile
obj-m:=digitr.o
else
########--PC---------------------------------------------------------

#KERNELSRC	:=/usr/src/linux-headers-3.2.0-38-generic
########--arm board --------------------------------------------------
KERNELSRC :=/home/akaedu/Board/student/kernel/linux-2.6.27

modules:
	make -C $(KERNELSRC) SUBDIRS=$(PWD) $@
	arm-linux-gcc app.c -o app
	arm-linux-gcc led_write.c -o led_write

clean:
	rm -f *.o *.ko *.mod.c *~
distclean:
	rm -f *.o *.ko *.mod.c *~ app

endif
