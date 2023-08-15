CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
TARGET = metric-trainer
SRCDIR = src
SOURCES = $(SRCDIR)/main.c $(SRCDIR)/questions.c
OBJECTS = $(SOURCES:.c=.o)

.PHONY: all clean debug

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

$(SRCDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

debug: CFLAGS += -g -DDEBUG
debug: $(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET)

install: $(TARGET)
	cp $(TARGET) /usr/local/bin/

uninstall:
	rm -f /usr/local/bin/$(TARGET)