#############################################
# *Function :driver makefile
# *Author
# *Date
# *$ld:
#############################################

ifneq ($(KERNELRELEASE),)
# Kbuild part of makefile
obj-m := register_dev_new.o
else
#//   arm
KERNELSRC :=/home/lenovo/crosstool/linux-2.6.27
#// linux
#KERNELSRC :=/usr/src/linux-headers-3.2.0-23-generic



modules:
	make -C $(KERNELSRC) SUBDIRS=$(PWD) $@

clean:
	rm -f *.o *.ko *.mod.c *~

endif







