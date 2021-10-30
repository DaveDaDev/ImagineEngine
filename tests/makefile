CC = gcc
CFLAGS = -Wall -std=c11 -g 
DFLAGS = IE_CORE_DEBUG_MEMORY_STATUS_ON 

PPATH = .. 
LIBS = -lpthread -lxcb 
BIN = test
SRC = test.c\
      ../core/debug_memory/iedebugmemory.c\
      ../core/iecore.c\
      ../core/loop/ieloop.c\
      ../core/window/iewindow.c\
      ../core/log/ielog.c\
      ../core/log/ielog_quick.c\
      ../core/event/ieevent.c\
      ../core/draw/ieprimitive.c\
      ../core/config/ieconfig.c\
      ../core/array_dynamic/iearraydynamic.c 

all: $(SRC)
	$(CC) $(CFLAGS) -o $(BIN) $^ $(LIBS) -D $(DFLAGS) -I$(PPATH)	

run:
	./$(BIN)	
