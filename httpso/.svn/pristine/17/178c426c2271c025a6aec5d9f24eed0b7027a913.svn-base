COM_PATH = ../../../common
VPATH = $(COM_PATH)/sim:$(COM_PATH):$(COM_PATH)/httpso/common:interface:protocol
INC_PATH = -I$(COM_PATH) -I$(COM_PATH)/httpso/include
LIB_PATH =

CPP = g++
CFLAGS       = -fPIC -Wall
CPPFLAGS     = -fPIC -g -O0 -Wall -std=c++11 $(INC_PATH)
DEBUGFLAGS   = -g -O0 -D_DEBUG -DDEBUG
RELEASEFLAGS = -O2 -DNDEBUG
LDFLAGS      = -shared
LIBS         = $(LIB_PATH) 

ADD_SRC = httpso.cpp 
TARGET  = libhttpso_test.so
SOURCES = $(shell echo *.cpp $(ADD_SRC))
HEADERS = $(shell echo *.h)
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CPP) $(FLAGS) $(CPPFLAGS) -shared -o $(TARGET) $(OBJECTS) $(LDFLAGS) $(LIBS)

clean:
	rm -f *.o $(TARGET)

