CFLAGS=-Wall -g
LDFLAGS=-Wall -g -pthread

COMPILE=g++ $(CFLAGS) -c
LINK=g++ $(LDFLAGS) -o

SRCDIR=./src
OBJDIR=./obj
OBJ=$(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(wildcard $(SRCDIR)/*.cpp) -lssl -lcrypto)
BINDIR=./bin
BIN=$(BINDIR)/apns-client

all: $(BIN)

$(BIN): $(OBJ)
	$(LINK) $(BIN) $(OBJ)

$(OBJDIR)/%.o:	$(SRCDIR)/%.cpp
	$(COMPILE) $< -o $@

