%_c.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%_asm.o: %.asm
	$(NASM) $(NASMFLAGS) -o $@ $<

stage2.o: stage2_c.o stage2_asm.o
	$(LD) -m elf_i386 -r -o $@ $^

.PHONY: clean
clean:
	$(RM) *.a *.o
