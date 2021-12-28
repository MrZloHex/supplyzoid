CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
OC = arm-none-eabi-objcopy
OD = arm-none-eabi-objdump
OS = arm-none-eabi-size

MCU_SPEC  = cortex-m0

INC_DIR = include

AS_FLAGS := -c -mcpu=$(MCU_SPEC) -mthumb -Wall -ggdb -x assembler-with-cpp -I$(INC_DIR)

C_FLAGS := -c -Wall -pedantic -I$(INC_DIR)

LSCRIPT = $(LD_SCRIPT)
L_FLAGS = -mcpu=$(MCU_SPEC) -mthumb -Wall -nostdlib -lgcc -T$(LSCRIPT)

LD_SCRIPT = stm32g070.ld


TARGET = supplyzoid

SRC_DIR = src
ASM_DIR = assembly
C_DIR   = c
OBJ_DIR = obj

ASM_SRC  = $(wildcard $(SRC_DIR)/$(ASM_DIR)/*S)
C_SRC    = $(wildcard $(SRC_DIR)/$(C_DIR)/*.c)
ASM_OBJS = $(patsubst $(SRC_DIR)/$(ASM_DIR)/%.S, $(OBJ_DIR)/%.asm.o, $(ASM_SRC))
C_OBJS   = $(patsubst $(SRC_DIR)/$(C_DIR)/%.c, $(OBJ_DIR)/%.c.o, $(C_SRC))

.PHONY: all
all: clean $(TARGET).bin 

$(ASM_OBJS): $(ASM_SRC)
	$(CC) $(AS_FLAGS) $(patsubst $(OBJ_DIR)/%.asm.o, $(SRC_DIR)/$(ASM_DIR)/%.S, $@) -o $@


$(C_OBJS): $(C_SRC)
	$(CC) $(C_FLAGS) $(patsubst $(OBJ_DIR)/%.c.o, $(SRC_DIR)/$(C_DIR)/%.c, $@) -o $@


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
