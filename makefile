CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
OC = arm-none-eabi-objcopy
OD = arm-none-eabi-objdump
OS = arm-none-eabi-size

MCU_SPEC  = cortex-m3

AS_FLAGS := -c -mcpu=$(MCU_SPEC) -mthumb -Wall

LSCRIPT = $(LD_SCRIPT)
L_FLAGS = -mcpu=$(MCU_SPEC) -mthumb -Wall -nostdlib -lgcc -T$(LSCRIPT)

LD_SCRIPT = stm32f103.ld


TARGET = supplyzoid

SRC_DIR = src
OBJ_DIR = obj

ASM_SRC = $(wildcard $(SRC_DIR)/*s)
OBJS    = $(patsubst $(SRC_DIR)/%.s, $(OBJ_DIR)/%.o, $(ASM_SRC))

.PHONY: all
all: clean $(TARGET).bin 

$(OBJS): $(ASM_SRC)
	$(CC) -x assembler-with-cpp $(AS_FLAGS) $^ -o $@


$(TARGET).elf: $(OBJS)
	$(CC) $^ $(L_FLAGS) -o $@

$(TARGET).bin: $(TARGET).elf
	$(OC) -S -O binary $< $@
	$(OS) $<


.PHONY: clean
clean:
	-rm -f $(OBJ_DIR)/*
	-rm -f $(TARGET).elf
	-rm -f $(TARGET).bin
