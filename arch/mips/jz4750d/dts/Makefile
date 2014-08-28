DTS_FILES = rzx50.dts
DTB_FILES = $(patsubst %.dts, %.dtb, $(DTS_FILES))

obj-y += $(patsubst %.dts, %.dtb.o, $(DTS_FILES))

clean-files += $(DTB_FILES) $(patsubst %.dtb, %.dtb.S, $(DTB_FILES))

