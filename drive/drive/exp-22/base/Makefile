obj-m := base.o
KDIR := ../linux
all:
	$(MAKE) -C $(KDIR) M=$(PWD) 
clean:
	rm -rf .*.cmd *.o *.mod.c *.ko .tmp_version *.sym*
app:
	gcc -o app app.c -static
	gcc -o app1 app1.c -static
