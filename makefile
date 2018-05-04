HEADERS = includes/defs.h
OBJECTS = build/main.o build/sprs.o build/bg2.o build/bg1.o build/soundbank.o
SOURCES = data/sprs.c data/bg2.c data/bg1.o src/main.c rc/soundbank.s
ELFS = build/main.elf
CFLAGS = -MMD -MP -MF -Wall -Ofast -ffast-math -mlong-calls -mcpu=arm7tdmi -mtune=arm7tdmi -fomit-frame-pointer -marm
ARCH = -mthumb -mthumb-interwork
SPECS = -specs=gba.specs
PREFIX = C:\devkitPro\devkitARM\bin\arm-none-eabi-

default: mayhem.gba

build/%.o: src/%.s $(HEADERS)
	$(PREFIX)gcc $(CFLAGS) $(ARCH) -c $< -o $@
build/%.o: src/%.c $(HEADERS)
	$(PREFIX)gcc $(CFLAGS) $(ARCH) -c $< -o $@
build/%.o: data/%.c $(HEADERS)
	$(PREFIX)gcc $(CFLAGS) $(ARCH) -c $< -o $@
build/main.elf: $(OBJECTS) 
	$(PREFIX)gcc $(SPECS) $(ARCH) -LC:\devkitPro\devkitARM\lib $(OBJECTS) -lheart -lm -o build/main.elf
mayhem.gba: $(ELFS)
	$(PREFIX)objcopy -O binary $(ELFS) mayhem.gba
	C:\devkitPro\devkitARM\bin\gbafix mayhem.gba
clean:
	-rm -f build/*.o
	-rm -f build/main.elf
	-rm -f mayhem.gba