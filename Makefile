CC ?= gcc
CFLAGS ?= -Wall -Wextra -std=c99 -I.
LDFLAGS ?= -lm -nostdlib -T linker.ld

SRCS = neonos.c
OBJS = $(SRCS:.c=.o)
KERNEL = neonos.kernel
EXEC = driver_loader
GRUB_CFG = grub.cfg
ISO_DIR = uniteriver
ISO = neonos.iso
ISO_PATH = $(ISO_DIR)/$(ISO)

.PHONY: all clean iso

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

iso: $(KERNEL) $(GRUB_CFG)
	mkdir -p $(ISO_DIR)
	grub-mkrescue -o $(ISO_PATH) --no-grub-mkrescue-target-file .

	# Copy required files into the uniteriver directory
	mkdir -p $(ISO_DIR)/.usrkrnl $(ISO_DIR)/.syskrnl $(ISO_DIR)/.osrnl

	# Limit ISO size to 500MB
	truncate -s 500M $(ISO_PATH)

	# Append the uniteriver directory to the original ISO file
	mkisofs -r -b $(KERNEL) -no-emul-boot -c boot.catalog -boot-load-size 4 -boot-info-table -hide-rr-moved -J -R -V "NEONOS" $(ISO_PATH) $(ISO_DIR)
	rm -f boot.catalog

clean:
	rm -f $(OBJS) $(EXEC) $(KERNEL) $(GRUB_CFG) $(ISO_PATH)



ENTRY(kernel_entry)

SECTIONS
{
	. = 0x100000;
	.text : { *(.text) }
	.data : { *(.data) }
	.bss : { *(.bss) }
}

