rm *.o
gcc -c shell.c interpreter.c shellmemory.c kernel.c cpu.c pcb.c ram.c memorymanager.c DISK_driver.c DISK_driver_problem1.c DISK_driver_problem2.c DISK_driver_problem3.c DISK_driver_problem4.c
gcc -o mykernel shell.o interpreter.o shellmemory.o kernel.o cpu.o pcb.o ram.o memorymanager.o DISK_driver.o DISK_driver_problem1.o DISK_driver_problem2.o DISK_driver_problem3.o DISK_driver_problem4.o
