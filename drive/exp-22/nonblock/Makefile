obj-m := block.o block_another.o
KDIR := ../linux
all:
	$(MAKE) -C $(KDIR) M=$(PWD) 
clean:
	rm -rf .*.cmd *.o *.mod.c *.ko .tmp_version *.sym*
app:
	gcc -o app app.c -static
