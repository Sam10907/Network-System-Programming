BASEINCLUDE = /lib/modules/5.4.0-91-generic/build
mydriver-objs := driver.o

obj-m := mydriver.o
all:
	$(MAKE) -C $(BASEINCLUDE) M=$(PWD) modules;

 clean:
	$(MAKE) -C $(BASEINCLUDE) SUBDIRS=$(PWD) clean;
	rm -f *.ko;