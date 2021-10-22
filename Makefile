#############################################################################
# Makefile
#############################################################################

default:kernel_for_qemu

all: kernel_for_qemu kernel_for_sdcard

kernel_for_qemu: build/kernel.elf build/kernel.list

kernel_for_sdcard: kernel_for_qemu build/kernel.img 

remake: clean all

# options à passer au compilateur C
CFLAGS=-Wall -Werror -nostdlib -nostartfiles -ffreestanding -std=gnu99 -g -fomit-frame-pointer -nostartfiles -O0 -fdiagnostics-show-option

# options à passer à la fois au compilateur C et à l'assembleur
COMMON_FLAGS=-mcpu=cortex-a7

# Object files (excluding kmain)
OBJECTS=$(addsuffix .o,  $(addprefix build/, $(basename $(notdir $(wildcard src/*.[cs])))))

# non-default usage: "make KMAIN=test/my-kmain.c" 
KMAIN ?= ./kmain.c

# check whether kmain does exists. typically this would get triggered
# by a command like "make KMAIN=" (i.e. with no value for KMAIN)
ifneq "$(shell test -f ''${KMAIN} && echo yes || echo no)" "yes"
    $(error no such file: "${KMAIN}")
endif

# check whether we're dealing with a different kmain.c than last time:
# either updated/modified, or maybe another file, located somewhere else
ifneq "$(shell md5sum ${KMAIN})" "$(shell test -f build/kmain.md5 && cat build/kmain.md5)"
  # if so, then our kmain.o has to be recompiled
  .PHONY: build/kmain.o
endif

build:
	mkdir -p build

# compilation C vers ELF
build/%.o: src/%.c $(wildcard src/*.h) | build
	arm-none-eabi-gcc $(COMMON_FLAGS) $(CFLAGS) $< -c -o $@

# compilation du point d'entrée 
build/kmain.o: $(KMAIN) $(wildcard src/*.h)
	arm-none-eabi-gcc $(COMMON_FLAGS) $(CFLAGS) -I src $< -c -o $@
	@#"let's silently remember the checksum of the most recent kmain.c we compiled"
	@md5sum $(KMAIN) > build/kmain.md5

# assemblage .s vers ELF
build/%.o: src/%.s | build
	arm-none-eabi-as -g $< -o $@
#	arm-none-eabi-as -g $(COMMON_FLAGS) $< -c -o $@

# édition de liens
build/kernel.elf: $(OBJECTS) build/kmain.o
	arm-none-eabi-ld $^ -o $@ -T src/barepyOS.ld -Map build/mapfile.map

# conversion de l'image pour transfert sur carte SD
build/kernel.img: build/kernel.elf
	arm-none-eabi-objcopy $^ -O binary $@

# désassemblage
build/kernel.list: build/kernel.elf
	arm-none-eabi-objdump -d -j .text -j .bss -j .stack -j .kernel_heap -j .user_stacks $< > $@

# nettoyage: effacer tous les fichiers générés
.PHONY:clean
clean:
	rm -rf build

.PHONY:test
test:
	cd tools;\
	echo "8 tests from q4.5 to q5.15";\
	(./run-test.sh   ../test/kmain-reboot.c        ../test/sys-reboot-does-reboot.gdb 		| grep OK) &&\
	(./run-test.sh   ../test/kmain-nop-reboot.c    ../test/sys-nop-does-return.gdb 			| grep OK) &&\
	(./run-test.sh   ../test/kmain-nop-reboot.c    ../test/swi-handler-preserves-SP.gdb 	| grep OK) &&\
	(./run-test.sh   ../test/kmain-settime.c       ../test/sys-settime-passes-argument.gdb	| grep OK) &&\
	(./run-test.sh   ../test/kmain-gettime.c       ../test/sys-gettime-returns-value.gdb    | grep OK) &&\
	(./run-test.sh   ../test/kmain-yieldto.c       ../test/sys-yieldto-jumps-to-dest.gdb    | grep OK) &&\
	(./run-test.sh   ../test/kmain-yieldto.c       ../test/sys-yieldto-preserves-locals.gdb | grep OK) &&\
	(./run-test.sh   ../test/kmain-yieldto.c       ../test/sys-yieldto-preserves-status-register.gdb | grep OK) &&\
	echo "\033[;32mAll tests SUCCEEDED\033[0m" || echo "\033[;31mOne or more tests FAILED\033[0m"
