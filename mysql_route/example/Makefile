COM_PATH =../..
VPATH    = $(COM_PATH)/sim:$(COM_PATH)/mysql_route/
INC_PATH = -I$(COM_PATH) -I$(COM_PATH)/sim -I$(COM_PATH)/mysql_route/include -I$(COM_PATH)/mysql_route
ADD_SRC  = simini.cpp simsql.cpp string_common.cpp mysql_route.cpp mysql_route_factory.cpp

CPP = g++
CFLAGS       = -fPIC -Wall
CPPFLAGS     = -fPIC -g -O0 -Wall -std=c++11 $(INC_PATH)
DEBUGFLAGS   = -g -O0 -D_DEBUG -DDEBUG
RELEASEFLAGS = -O2 -DNDEBUG
LDFLAGS      = -lsqlite3 -llog4cplus -L/usr/lib64/mysql -lmysqlclient -lboost_system

TARGET  = demo
SOURCES = $(shell echo *.cpp $(ADD_SRC))
HEADERS = $(shell echo *.h)
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CPP) $(FLAGS) $(CPPFLAGS) -o $(TARGET) $(OBJECTS) $(LDFLAGS) $(LIBS)

clean:
	rm -f *.o $(TARGET) ../../sim/*.o ../*.o

