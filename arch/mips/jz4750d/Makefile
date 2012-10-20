#
# Makefile for the Ingenic JZ4750D.
#

# Object file lists.

obj-y += prom.o irq.o time.o reset.o setup.o
obj-y += clock.o platform.o timer.o serial.o

obj-$(CONFIG_DEBUG_FS) += clock-debugfs.o

# board specific support
obj-$(CONFIG_JZ4750D_RZX50)	+= board-rzx50.o
