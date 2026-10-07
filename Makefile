export
GLOBAL_INC = $(abspath $(dir $(lastword $(MAKEFILE_LIST))))/include
QEMU_I386 := qemu-system-i386
NASMFLAGS := -I$(CURDIR)/include -f elf
TRUNCATE := truncate
CFLAGS = -I$(GLOBAL_INC) -m32 -ffreestanding -nostdlib -fno-pie -fno-pic
NASM := nasm
CAT := cat
DD := dd

DEPS := boot/boot.bin main/stage2.bin
.PHONY: all clean run FORCE

all: $(DEPS) disk.img

$(DEPS): FORCE
	$(MAKE) -C $(dir $@)

clean:
	@for dir in $(dir $(DEPS)); do \
		$(MAKE) -C $$dir clean; \
	\
	done
	$(RM) disk.img

disk.img: $(DEPS)
	$(RM) $@
	$(TRUNCATE) -s 100M $@
	$(CAT) $(DEPS) | $(DD) of=$@ conv=notrunc

run: disk.img
	$(QEMU_I386) -m 100M -drive if=ide,format=raw,file=$< -boot c -nographic
	@reset
FORCE:
