
VPATH = ../sim:../

CPP = g++
CFLAGS       = -fPIC -Wall
CPPFLAGS     = -fPIC -g -O0 -Wall -std=c++11 -I../ -I./include -I../sim
DEBUGFLAGS   = -g -O0 -D_DEBUG -DDEBUG
RELEASEFLAGS = -O2 -DNDEBUG
LDFLAGS      = -lsqlite3 -llog4cplus -L/usr/lib64/mysql -lmysqlclient

TARGET  = libmysqlroute.so
SOURCES = $(shell echo *.cpp)
HEADERS = $(shell echo *.h)
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CPP) $(FLAGS) $(CPPFLAGS) -shared -o $(TARGET) $(OBJECTS) $(LDFLAGS) $(LIBS)

clean:
	rm -f *.o $(TARGET) ../../sim/*.o ../*.o

