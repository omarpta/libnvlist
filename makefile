.LIBNVLIST : clean

CC = gcc
CFLAGS= -fPIC -g
LDFLAGS= -shared

SOURCES = $(shell echo *.c)
HEADERS = $(shell echo *.h)
OBJECTS=$(SOURCES:.c=.o)

LIBDIR=/usr/lib
INCDIR=/usr/include

TARGET=$(LIBDIR)/libnvlist.so

all: $(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)
