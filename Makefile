CC = gcc

CFLAGS = -Wall -Wextra -std=c11

TARGET = washing_machine

SRC = main.c \
      machine.c \
      input.c \
      display.c \
      timer.c \
      power.c


$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)


clean:
	rm -f $(TARGET)