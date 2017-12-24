HEADERS = includes/defs.h
OBJECTS = build/main.o build/sprs.o build/bg2.o build/bg1.o build/bgm.o
SOURCES = data/sprs.c data/bg2.c data/bg1.o src/main.c data/bgm.c
ELFS = build/main.elf
CFLAGS = -g -MMD -MP -MF -Wall -Og -O1 -O2 -O3 -O -Os -ffast-math -mlong-calls -mcpu=arm7tdmi -mtune=arm7tdmi -fomit-frame-pointer -marm
ARCH = -mthumb -mthumb-interwork
SPECS = -specs=gba.specs
PREFIX = C:\devkitPro\devkitARM\bin\arm-none-eabi-

default: mayhem.gba

build/%.o: src/%.c $(HEADERS)
	$(PREFIX)gcc $(CFLAGS) $(ARCH) -c $< -o $@
build/%.o: data/%.c $(HEADERS)
	$(PREFIX)gcc $(CFLAGS) $(ARCH) -c $< -o $@
build/main.elf: $(OBJECTS) 
	$(PREFIX)gcc $(SPECS) $(ARCH) $(OBJECTS) -o build/main.elf
mayhem.gba: $(ELFS)
	$(PREFIX)objcopy -O binary $(ELFS) mayhem.gba
	C:\devkitPro\devkitARM\bin\gbafix mayhem.gba
clean:
	-rm -f build/*.o
	-rm -f build/main.elf
	-rm -f mayhem.gba