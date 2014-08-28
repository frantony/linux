#
# Makefile for the Ingenic JZ4750D.
#

CFLAGS_platform.o = -I$(src)/../../../scripts/dtc/libfdt
CFLAGS_setup.o = -I$(src)/../../../scripts/dtc/libfdt

# Object file lists.

obj-y += prom.o irq.o time.o reset.o setup.o
obj-y += platform.o timer.o

# board specific support
obj-$(CONFIG_JZ4750D_RZX50)	+= board-rzx50.o

obj-y += dts/
