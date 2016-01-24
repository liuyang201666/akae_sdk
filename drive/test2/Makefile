#############################################
# *Function :driver makefile
# *Author
# *Date
# *$ld:
#############################################

ifneq ($(KERNELRELEASE),)
# Kbuild part of makefile
obj-m := driver.o
else

KERNELSRC :=/home/lenovo/2440/linux-2.6.27 
#KERNELSRC :=/usr/src/linux-headers-2.6.32-30-generic


modules:
	make -C $(KERNELSRC) SUBDIRS=$(PWD) $@

clean:
	rm -f *.o *.ko *.mod.c *~

endif







