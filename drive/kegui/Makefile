#############################################
# *Function :driver makefile
# *Author
# *Date
# *$ld:
#############################################

ifneq ($(KERNELRELEASE),)
# Kbuild part of makefile
obj-m := kegui.o
else
#//   arm
#KERNELSRC :=/home/lenovo/2440/linux-2.6.27
#// linux
#KERNELSRC :=/usr/src/linux-headers-3.2.0-33-generic
KERNELSRC :=/home/akaedu/Board/student/kernel/linux-2.6.27



modules:
	make -C $(KERNELSRC) SUBDIRS=$(PWD) $@

clean:
	rm -f *.o *.ko *.mod.c *~

endif







