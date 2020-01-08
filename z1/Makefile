CC = gcc
CFLAGS = -Wall -Wextra -pthread -O0

all: accounts accounts_pad

SRC_FILES= accounts.c ../common/aff.c

accounts_pad: accounts_pad.c ../common/aff.c
	$(CC) $(CFLAGS) $^ -o $@

accounts: $(SRC_FILES)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f accounts
