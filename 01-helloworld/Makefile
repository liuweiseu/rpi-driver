obj-m +=helloworld.o

KDIR:=/home/wei/Projects/rpi-driver/linux

PWD?=${shell pwd}

all:
	make -C ${KDIR} M=${PWD} modules

clean:
	make -C ${KDIR} M=${PWD} clean
