CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
OC = arm-none-eabi-objcopy
OD = arm-none-eabi-objdump
OS = arm-none-eabi-size

MCU_SPEC  = cortex-m3

AS_FLAGS := -c -mcpu=$(MCU_SPEC) -mthumb -Wall -x assembler-with-cpp

LSCRIPT = $(LD_SCRIPT)
L_FLAGS = -mcpu=$(MCU_SPEC) -mthumb -Wall -nostdlib -lgcc -T$(LSCRIPT)

LD_SCRIPT = stm32f103.ld


TARGET = supplyzoid

SRC_DIR = src
ASM_DIR = assembly
C_DIR   = c
OBJ_DIR = obj

ASM_SRC  = $(wildcard $(SRC_DIR)/$(ASM_DIR)/*S)
C_SRC    = $(wildcard $(SRC_DIR)/$(C_DIR)/*.c)
ASM_OBJS = $(patsubst $(SRC_DIR)/$(ASM_DIR)/%.S, $(OBJ_DIR)/%.o, $(ASM_SRC))
C_OBJS   = $(patsubst $(SRC_DIR)/$(C_DIR)/%.c, $(OBJ_DIR)/%.o, $(C_SRC))

.PHONY: all
all: clean $(TARGET).bin 

$(ASM_OBJS): $(ASM_SRC)
	$(CC) $(AS_FLAGS) $< -o $@


$(C_OBJS): $(C_SRC)
	$(CC) -c $< -o $@


$(TARGET).elf: $(ASM_OBJS) $(C_OBJS)
	$(CC) $^ $(L_FLAGS) -o $@

$(TARGET).bin: $(TARGET).elf
	$(OC) -S -O binary $< $@
	$(OS) $<


.PHONY: clean
clean:
	-rm -f $(OBJ_DIR)/*
	-rm -f $(TARGET).elf
	-rm -f $(TARGET).bin
