CC       = g++
WSDIR    = $(CURDIR)/wsServer
INCLUDE  = -I $(WSDIR)/include
CFLAGS   =  -Wall -Wextra -g
CFLAGS  +=  $(INCLUDE) -std=c++11 -pthread
LIB      =  $(WSDIR)/libws.a

all: $(LIB) mystery

$(LIB):
	@$(MAKE) all -C $(WSDIR)/

canvas.o: canvas.h canvas.cpp $(LIB)
	$(CC) $(CFLAGS) -c canvas.cpp

mystery: mystery.cpp canvas.o $(LIB)
	$(CC) $(CFLAGS) $(LDFLAGS) mystery.cpp canvas.o -o mystery $(LIB)

# Clean
clean:
	rm *.o mystery
