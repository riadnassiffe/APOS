############variaveis################
CC:= gcc
nasm := nasm
bin := ../bin/
kernel := kernel/
modulos :=modulos/
lib := libs/
ld := ld

FLAGS :=  -Wall -W  -nostdinc -fno-builtin  -I..\include -c -o
all:compilando
	

###########compilando###############
compilando:
	$(nasm) -f aout $(kernel)boot/Start.asm -o $(bin)start.o
	$(CC) $(FLAGS) $(bin)main.o $(kernel)Main.c
	$(CC) $(FLAGS) $(bin)gdt.o $(kernel)Gdt.c
	$(CC) $(FLAGS) $(bin)gmem.o $(modulos)Gmem.c
	$(CC) $(FLAGS) $(bin)gpro.o $(modulos)Gpro.c
	$(CC) $(FLAGS) $(bin)ide.o $(kernel)Ide.c
	$(CC) $(FLAGS) $(bin)idt.o $(kernel)Idt.c
	$(CC) $(FLAGS) $(bin)if.o If.c
	$(CC) $(FLAGS) $(bin)io.o $(kernel)IO.c
	$(CC) $(FLAGS) $(bin)fat.o $(kernel)Fat.c
	$(CC) $(FLAGS) $(bin)irq.o $(kernel)Irq.c
	$(CC) $(FLAGS) $(bin)isrs.o $(kernel)Isrs.c
	$(CC) $(FLAGS) $(bin)kb.o $(kernel)Kb.c
	$(CC) $(FLAGS) $(bin)scrn.o $(kernel)Scrn.c
	$(CC) $(FLAGS) $(bin)slib.o $(lib)Slib.c
	$(CC) $(FLAGS) $(bin)tim.o $(modulos)Tim.c
	$(CC) $(FLAGS) $(bin)memTable.o $(modulos)MemTable.c	
	
	$(ld)  -T link.ld -o $(bin)apos.bin $(bin)start.o $(bin)main.o $(bin)scrn.o $(bin)gdt.o $(bin)idt.o $(bin)isrs.o $(bin)irq.o $(bin)tim.o $(bin)kb.o $(bin)gpro.o $(bin)memTable.o $(bin)gmem.o  $(bin)slib.o $(bin)if.o $(bin)io.o $(bin)fat.o $(bin)ide.o 

run:
	../../../Bochs-2.2.6/bochs.exe -qf ../../../Bochs-2.2.6/bochs.txt    


teste:
	$(nasm)  -f  win32 $(kernel)boot/Start.asm -o $(bin)start.o
	$(CC) $(FLAGS) $(bin)main.o $(kernel)Main.c
	$(CC) $(FLAGS) $(bin)afs.o $(modulos)Afs.c
	$(CC) $(FLAGS) $(bin)gdt.o $(kernel)Gdt.c
	$(CC) $(FLAGS) $(bin)gmem.o $(modulos)Gmem.c
	$(CC) $(FLAGS) $(bin)gpro.o $(modulos)Gpro.c
	$(CC) $(FLAGS) $(bin)ide.o $(kernel)Ide.c
	$(CC) $(FLAGS) $(bin)idt.o $(kernel)Idt.c
	$(CC) $(FLAGS) $(bin)if.o If.c
	$(CC) $(FLAGS) $(bin)io.o $(kernel)IO.c
	$(CC) $(FLAGS) $(bin)irq.o $(kernel)Irq.c
	$(CC) $(FLAGS) $(bin)isrs.o $(kernel)Isrs.c
	$(CC) $(FLAGS) $(bin)kb.o $(kernel)Kb.c
	$(CC) $(FLAGS) $(bin)scrn.o $(kernel)Scrn.c
	$(CC) $(FLAGS) $(bin)slib.o $(lib)Slib.c
	$(CC) $(FLAGS) $(bin)tim.o $(modulos)Tim.c	
	
clean:
	rm $(bin)*.o
	rm $(bin)*.bin 
	rm $(bin)*.pe
###########removendo################

