%_c.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%_asm.o: %.asm
	$(NASM) $(NASMFLAGS) -o $@ $<

interrupts.a: interrupts_c.o interrupts_asm.o
	$(AR) scrT $@ $^

.PHONY: clean
clean:
	$(RM) *.a *.o
