######################################################
#*Function:driver makefile
#*Author:asmcos@gmail.com
#*Date:2005-08-24
#*$Id:Makefile,v 1.3 2006/05/17 12:37:19 asmcos Exp $
######################################################

ifneq ($(KERNELRELEASE),)
#kbuild part of makefile
obj-m:=sched.o kill_deepsleep.o
else


#KERNELSRC :=/usr/src/linux-headers-2.6.32-40-generic
KERNELSRC :=/usr/src/linux-headers-3.0.0-12-generic

modules:
	make -C $(KERNELSRC) SUBDIRS=$(PWD) $@

clean:
	rm -f *.o *.ko *.mod.c *~

endif
