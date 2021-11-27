CC = gcc
# CC = arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb

INC_DIR = -Iinclude

C_FLAGS := -Wall -pedantic $(INC_DIR) -O3


SRC_DIR = src
SRC_R_DIR = src/requests

TARGET  = supplyzoid
SRCS    = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_R_DIR)/*.c)

.PHONY: all
all: clean $(TARGET) run 

$(TARGET): $(SRCS)
	$(CC) $^ $(C_FLAGS) -o $@


run: 
	./$(TARGET)

.PHONY: clean
clean:
	-rm -f $(TARGET)
