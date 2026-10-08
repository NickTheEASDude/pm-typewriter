%_c.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%_asm.o: %.asm
	$(NASM) $(NASMFLAGS) -o $@ $<

main.o: main_c.o main_asm.o
	$(LD) -m elf_i386 -r -o $@ $^

.PHONY: clean
clean:
	$(RM) *.a *.o
