obj-m += link_list_c.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

install:
	sudo insmod link_list_c.ko

uninstall:
	sudo rmmod link_list_c

mls:
	lsmod | grep link_list_c

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
