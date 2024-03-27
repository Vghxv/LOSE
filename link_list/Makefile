obj-m += link_list.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

install:
	sudo insmod link_list.ko

uninstall:
	sudo rmmod link_list

mls:
	lsmod | grep link_list

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
