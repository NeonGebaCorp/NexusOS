CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -lm

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
	grub-mkrescue -o $(ISO_PATH) .

	# Copy required files into the uniteriver directory
	mkdir -p $(ISO_DIR)/.usrkrnl $(ISO_DIR)/.syskrnl $(ISO_DIR)/.osrnl

	# Limit ISO size to 500MB
	truncate -s 500M $(ISO_PATH)

	# Append the uniteriver directory to the original ISO file
	mkisofs -r -o temp.iso $(ISO_DIR)
	dd if=temp.iso of=$(ISO_PATH) seek=$(shell stat --format="%s" $(ISO_PATH)) conv=notrunc
	rm -f temp.iso

clean:
	rm -f $(OBJS) $(EXEC) $(KERNEL) $(GRUB_CFG) $(ISO_PATH)
