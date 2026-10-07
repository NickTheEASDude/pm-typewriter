%_c.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%_asm.o: %.asm
	$(NASM) $(NASMFLAGS) -o $@ $<

misc.a: io_c.o uart_asm.o
	$(AR) scrT $@ $^

.PHONY: clean
clean:
	$(RM) *.a *.o
