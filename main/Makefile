DEPS := stage2/stage2.o interrupts/interrupts.a misc/misc.a
LDFLAGS := --no-pie
.PHONY: all clean FORCE

all: stage2.bin

$(DEPS): FORCE
	$(MAKE) -C $(dir $@)

stage2.bin: $(DEPS) stage2.ld
	$(LD) --no-pie -T stage2.ld -Map=stage2.map -o $@ $(DEPS)

clean:
	@for dir in $(dir $(DEPS)); do \
		$(MAKE) -C $$dir clean; \
	\
	done
	$(RM) *.bin *.o *.map

FORCE:
