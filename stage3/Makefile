DEPS := main/main.o interrupts/interrupts.a misc/misc.a
.PHONY: all clean FORCE

all: stage3.bin

$(DEPS): FORCE
	$(MAKE) -C $(dir $@)

stage3.bin: stage3.elf
	$(OBJCOPY) -O binary $< $@

stage3.elf: $(DEPS) stage3.ld
	$(LD) --no-pie -T stage3.ld -Map=stage3.map -o $@ $(DEPS)

clean:
	@for dir in $(dir $(DEPS)); do \
		$(MAKE) -C $$dir clean; \
	\
	done
	$(RM) *.bin *.o *.map *.elf

FORCE:
