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

DTS_FILES = rzx50.dts
DTB_FILES = $(patsubst %.dts, %.dtb, $(DTS_FILES))

obj-y += $(patsubst %.dts, %.dtb.o, $(DTS_FILES))

$(obj)/%.dtb: $(src)/%.dts FORCE
	$(call if_changed_dep,dtc)

# Let's keep the .dtb files around in case we want to look at them.
.SECONDARY:  $(addprefix $(obj)/, $(DTB_FILES))

clean-files += $(DTB_FILES) $(patsubst %.dtb, %.dtb.S, $(DTB_FILES))
