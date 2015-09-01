all: vm_manager

main: vm_manager.o datatypes.o
	gcc -o vm_manager vm_manager.o datatypes.o

main.o: vm_manager.c
	gcc -c vm_manager.c

datatypes.o: datatypes.c
	gcc -c datatypes.c

clean:
	rm -rf *.o vm_manager