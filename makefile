DIR_INC = ./inc
DIR_SRC = ./src
DIR_OBJ = ./obj
DIR_BIN = ./bin

SRC = $(wildcard $(DIR_SRC)/*.cpp)
OBJ := $(patsubst %.cpp,$(DIR_OBJ)/%.o,$(notdir $(SRC)))

#$(info $(DIR_SRC))
#$(info $(SRC))
#$(info $(OBJ))

TARGET = hms
BIN_TARGET = $(DIR_BIN)/$(TARGET)

CC = g++

INCLUDE := -I./inc -I./libs/bin/libxml2-2.7.2/generic/i686-linux/include/libxml2/ -I./libs/bin/csoap/generic/i686-linux/include/nanohttp-1.1/ -I./libs/bin/csoap/generic/i686-linux/include/libcsoap-1.1

LDFLAGS := -L./libs/bin/libxml2-2.7.2/generic/i686-linux/lib/ -std=c++11  -L./libs/bin/csoap/generic/i686-linux/lib/ -L./libs/bin/csoap/generic/i686-linux/lib/ 

LIBS := -lxml2 -lnanohttp -lcsoap -lpthread

CFLAGS := -std=c++11 $(INCLUDE) $(LDFLAGS) $(LIBS)

$(DIR_OBJ)/%.o: $(DIR_SRC)/%.cpp
	$(CC) -c $< -o $@  $(CFLAGS) 

$(BIN_TARGET) : $(OBJ)
	$(CC) -o $@ $(OBJ) $(CFLAGS) 

clean:
	rm $(wildcard $(DIR_OBJ)/*.o) $(BIN_TARGET)


clean_library:
	rm ./libraries/csoap ./libraries/libxml2-2.7.2  ./libs -rf
