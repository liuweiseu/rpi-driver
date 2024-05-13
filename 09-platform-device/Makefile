obj-m +=device.o

KDIR:=../linux

PWD?=${shell pwd}

all:
	make -C ${KDIR} M=${PWD} modules

clean:
	make -C ${KDIR} M=${PWD} clean
