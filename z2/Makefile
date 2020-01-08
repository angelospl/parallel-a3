CC = gcc
CFLAGS = -Wall -Wextra -pthread -O0

## Remove some warnings.
CFLAGS += -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function

all: linked_list test ttas ttas_test

## Which lock to use?
LOCK_FILE = nosync_lock.c

## Additional source files
SRC_FILES = ../common/aff.c

PTHREAD = pthread_lock.c

TTAS = ttas_lock.c

linked_list: main.c $(LOCK_FILE) $(SRC_FILES)
	$(CC) $(CFLAGS) $^ -o $@

test: test.c $(LOCK_FILE) $(SRC_FILES)
	$(CC) $(CFLAGS) $^ -o $@

pthread_test: test.c $(PTHREAD) $(SRC_FILES)
	$(CC) $(CFLAGS) $^ -o $@

pthread: main.c $(PTHREAD) $(SRC_FILES)
	$(CC) $(CFLAGS) $^ -o $@

ttas_test: test.c $(TTAS) $(SRC_FILES)
	$(CC) $(CFLAGS) $^ -o $@

ttas: main.c $(TTAS) $(SRC_FILES)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f linked_list test
