.PHONY: all clean
all: stage2.bin
stage2.bin: stage2.asm
	$(NASM) -f bin -o $@ $<

clean:
	$(RM) *.bin
