boot.bin: boot.asm
	$(NASM) -f bin -o $@ $<

.PHONY: clean
clean:
	$(RM) *.bin *.o
