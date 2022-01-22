AC = nasm
CFLAGS = -felf64

LD = ld

target = chksum
args = '$$AB 00^'

all: clean compile link run

run:
	./$(target) $(args)

compile:
	$(AC) $(CFLAGS) $(target).asm -o $(target).o

link:
	$(LD) $(target).o -o $(target)

clean:
	-rm $(target)
	-rm $(target).o