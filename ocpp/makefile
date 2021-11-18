CC = gcc
# CC = arm-none-eabi-gcc

INC_DIR = include

C_FLAGS := -Wall -pedantic -I$(INC_DIR)


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
