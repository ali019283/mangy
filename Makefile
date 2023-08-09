SOURCES=mangy.c
EXECUTABLE=mangy
PREFIX=/usr/local
CFLAGS += -O2

all:
	$(CC) $(SOURCES) $(CFLAGS) -l X11 -o $(EXECUTABLE)

install: all
	cp $(EXECUTABLE) $(DESTDIR)$(PREFIX)/bin