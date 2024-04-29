CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -lm

SRCS = neonos.c
OBJS = $(SRCS:.c=.o)
KERNEL = neonos.kernel
EXEC = driver_loader
GRUB_CFG = grub.cfg
ISO = neonos.iso

.PHONY: all clean iso

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

iso: $(KERNEL) $(GRUB_CFG)
	grub-mkrescue -o $(ISO) .

$(KERNEL): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(GRUB_CFG):
	echo "default=0" > $(GRUB_CFG)
	echo "timeout=3" >> $(GRUB_CFG)
	echo "" >> $(GRUB_CFG)
	echo "menuentry \"Neon OS\" {" >> $(GRUB_CFG)
	echo "    set root=(hd0,1)" >> $(GRUB_CFG)
	echo "    multiboot /boot/$(KERNEL)" >> $(GRUB_CFG)
	echo "}" >> $(GRUB_CFG)

clean:
	rm -f $(OBJS) $(EXEC) $(KERNEL) $(GRUB_CFG) $(ISO)
