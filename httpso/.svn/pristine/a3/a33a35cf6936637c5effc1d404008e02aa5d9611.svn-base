COM_PATH = ../..
ADD_SRC = simini.cpp so_handler.cpp
ADD_INC = -I../include -I$(COM_PATH) -I$(COM_PATH)/sim 
LIBS = -ldl

VPATH = $(COM_PATH):$(COM_PATH)/sim

CPP = g++
CFLAGS       = -fPIC -Wall
CPPFLAGS     = -fPIC -g -O0 -Wall -std=c++11 $(ADD_INC)
DEBUGFLAGS   = -g -O0 -D_DEBUG -DDEBUG
RELEASEFLAGS = -O2 -DNDEBUG
LDFLAGS      = 

TARGET  = libhttpentry.so
SOURCES = $(shell echo *.cpp $(ADD_SRC))
HEADERS = $(shell echo *.h)
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CPP) $(FLAGS) $(CPPFLAGS) -shared -o $(TARGET) $(OBJECTS) $(LDFLAGS) $(LIBS)

clean:
	rm -f *.o $(TARGET) ../../sim/*.o ../*.o

