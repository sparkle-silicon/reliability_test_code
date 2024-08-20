#obj-m+=host_ec.o
#obj-m+=Reference_host_ec.o
#obj-m+=misc_host_ec.o
obj-m+=super_io_ec.o
#obj-m+=platform_host_ec.o
#obj-m+=power_supply_host_ec.o
all:
	make -C /usr/src/kernels/3.10.84-23.fc21.loongson.8.mips64el M=$(PWD) modules
#	gcc system_user_dev.c 
#	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean
auto:
	gcc ./auto_test.* -o auto_test -lpthread
