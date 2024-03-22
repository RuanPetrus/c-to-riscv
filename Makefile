# Directories
BUILD_DIR=build
OBJ_DIR=$(BUILD_DIR)/obj
ELF_DIR=$(BUILD_DIR)/elf
DUMP_DIR=$(BUILD_DIR)/dump

PLATFORM_DIR=platform

# Toolchain
LINKER_FILE=$(PLATFORM_DIR)/rv32-board.lds
CC=riscv64-unknown-elf-gcc
MARCH=rv32imf
MABI=ilp32f
MACH=-march=$(MARCH) -mabi=$(MABI)
LDFLAGS=-nostdlib -fno-exceptions -nostartfiles -ffreestanding -nodefaultlibs -T $(LINKER_FILE) -Wl,-Map=$(BUILD_DIR)/final.map -Wl,--no-relax

# C flags
CFLAGS= -Wall -Wextra -Wshadow -std=c99  -O0

# Elf executable name
TARGET=$(ELF_DIR)/music.elf

# Add your sources here
SOURCES=music.c \
		$(PLATFORM_DIR)/cr0.c

OBJ_NAMES=$(SOURCES:.c=.o)
OBJECTS=$(patsubst %,$(OBJ_DIR)/%,$(OBJ_NAMES))

$(TARGET): $(OBJECTS)
	@mkdir -p $(dir $@)
	$(CC) $(MACH) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(MACH) $(CFLAGS) -c -o $@ $^

# Dump rules
OBJCOPY_FLAGS=-R .comment -S --strip-unneeded -g --reverse-bytes=4 -O verilog --verilog-data-width 1

$(DUMP_DIR)/dump.bin: $(TARGET)
	@mkdir -p $(dir $@)
	riscv64-unknown-elf-objcopy $(OBJCOPY_FLAGS) $^ $@

$(DUMP_DIR)/dump_text.mif $(DUMP_DIR)/dump_data.mif: mif.in.intermediate;

.INTERMEDIATE: mif.in.intermediate
mif.in.intermediate: $(DUMP_DIR)/dump.bin
	@mkdir -p $(dir $@)
	python3 $(PLATFORM_DIR)/verilog_to_mif.py $^ $(DUMP_DIR)/dump

# Phonies
.PHONY: all clean dump

all: $(TARGET)

dump: $(DUMP_DIR)/dump_text.mif $(DUMP_DIR)/dump_data.mif

clean:
	rm -rf build
