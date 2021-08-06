CC       = gcc
CFLAGS   = -Wall
LDFLAGS  =
OBJFILES = console.o main.o
TARGET   = phpg

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

clean:
	rm -f $(OBJFILES) $(TARGET) *~
