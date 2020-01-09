CC = gcc
CFLAGS = -Wall -Wextra -pthread

## Remove some warnings.
CFLAGS += -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function

all: linked_list fgl optim

## Which linked list implementation to use?
LL_FILE = ll_serial.c

FGL = ll_fgl.c

OPT= ll_opt.c

## Additional source files
SRC_FILES = ../common/aff.c

linked_list: main.c ${LL_FILE} ${SRC_FILES}
	$(CC) $(CFLAGS) $^ -o $@

fgl: main.c ${FGL} ${SRC_FILES}
	$(CC) $(CFLAGS) $^ -o $@

optim: main.c ${OPT} ${SRC_FILES}
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f linked_list fgl optim
