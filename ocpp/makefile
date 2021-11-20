CC = gcc
# CC = arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb

INC_DIR = -Iinclude

C_FLAGS := -Wall -pedantic $(INC_DIR)


SRC_DIR = src

TARGET  = supplyzoid
SRCS    = $(wildcard $(SRC_DIR)/*.c)

.PHONY: all
all: clean $(TARGET) run 

$(TARGET): $(SRCS)
	$(CC) $^ $(C_FLAGS) -o $@


run: 
	./$(TARGET)

.PHONY: clean
clean:
	-rm -f $(TARGET)
