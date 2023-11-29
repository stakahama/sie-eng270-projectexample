CC=gcc
CFLAGS=-Wall -fPIC -O2
LIBS=-lm
DIRCODE=code
DIRBIN=bin
TARGET=$(DIRBIN)/clib.so
OBJ=$(DIRBIN)/cmain.o $(DIRBIN)/cfunctions.o
DEPS=$(DIRCODE)/cfunctions.h

$(DIRBIN)/%.o: $(DIRCODE)/%.c $(DEPS)
	$(CC) $(CFLAGS) -o $@ -c $< $(LIBS)

$(TARGET): $(OBJ)
	$(CC) -shared -o $@ $^

clean:
	rm -f $(DIRBIN)/*.o $(DIRBIN)/*.so

## create the directory if it does not exist
$(shell mkdir -p $(DIRBIN))
