NAME       = firmware
CSOURCES   = $(shell find -L . -type f -name '*.c')
CPPSOURCES = $(shell find -L . -type f -name '*.cpp')
INCLUDES   = $(shell find -L . -type f -name '*.h')
DEPENDS    = $(CSOURCES:%.c=%.dep) $(CPPSOURCES:%.cpp=%.dep)
OBJS       = $(CSOURCES:%.c=%.o) $(CPPSOURCES:%.cpp=%.o)

CC         = arm-none-eabi-gcc
CP         = arm-none-eabi-g++
LD         = arm-none-eabi-ld
OC         = arm-none-eabi-objcopy
CCFLAGS    = '-mcpu=cortex-m3' '-mthumb' -flto -ffunction-sections -fdata-sections
CPFLAGS    = '-mcpu=cortex-m3' '-mthumb' -flto -ffunction-sections -fdata-sections -fno-non-call-exceptions -fno-exceptions -fno-rtti
LDFLAGS    = '-Tlpc1833.ld' -nostartfiles -nostdlib -nodefaultlibs --gc-sections
OCFLAGS    = -Obinary --strip-unneeded

JLINK = jLinkExe
FLASH_START_ADDRESS = 0x1a000000


# all generates our target bin
all: $(NAME).bin

# generate bin from hex by setting the correct checksum
$(NAME).bin: $(NAME).hex
	cp $(NAME).hex $(NAME).bin
	checksum $(NAME).bin

# generate hex from obj by objcopying from linked obj file
$(NAME).hex: $(NAME).obj
	$(OC) $(OCFLAGS) $(NAME).obj $(NAME).hex

# generate obj by linking all .o files
$(NAME).obj: $(OBJS)
	$(LD) $(LDFLAGS) -o $(NAME).obj $(OBJS)

# compile .c to .o files. Generate .dep files on the fly.
%.o: %.c
	$(CC) $(CCFLAGS) -MD -o $@ -c $<
	@cp $*.d $*.dep; \
	sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $*.d >> $*.dep; \
	rm -f $*.d

# compile .cpp to .o files. Generate .dep files on the fly.
%.o: %.cpp
	$(CP) $(CPFLAGS) -MD -o $@ -c $<
	@cp $*.d $*.dep; \
	sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $*.d >> $*.dep; \
	rm -f $*.d

# include depencies are defined in .dep files. Include them as rules here.
-include $(DEPENDS)

## Program device
flash: rm-flash.jlink flash.jlink $(NAME).bin
	$(JLINK) flash.jlink

rm-flash.jlink:
	-rm -rf flash.jlink

flash.jlink:
	echo "device lpc1833\nr\nloadbin $(NAME).bin, $(FLASH_START_ADDRESS)\nr\ng\nexit\n" > flash.jlink

# clean all generated files
.PHONY: clean
clean:
	-rm $(NAME).bin $(NAME).hex $(NAME).obj $(OBJS) $(DEPENDS) flash.jlink JLink.log 2>/dev/null || true
