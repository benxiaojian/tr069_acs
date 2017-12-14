DIR_INC = .
DIR_SRC = .
DIR_OBJ = .
DIR_BIN = .

SRC = $(wildcard $(DIR_SRC)/*.c)
OBJ = $(patsubst %.c,$(DIR_OBJ)/*.o,$(notdir $(SRC)))

TARGET = libt.a
BIN_TARGET = $(DIR_BIN)/$(TARGET)

object = HttpPostRequest.o RpcMethod.o HnbClientFsm.o

CC = g++
CFLAGS = -I ../libs/bin/libxml2-2.7.2/generic/i686-linux/include/libxml2/ -L ../libs/bin/libxml2-2.7.2/generic/i686-linux/lib/ -lxml2 -std=c++11 -I ../libs/bin/csoap/generic/i686-linux/include/nanohttp-1.1/ -L ../libs/bin/csoap/generic/i686-linux/lib/ -lnanohttp -I ../libs/bin/csoap/generic/i686-linux/include/libcsoap-1.1  -L ../libs/bin/csoap/generic/i686-linux/lib/ -lcsoap -lpthread -g

$(TARGET):$(object)
	ar rcs $@ $^

%.o:%.cpp
	$(CC) -c $< -o $@  $(CFLAGS) 



clean:
	rm $(wildcard $(DIR_OBJ)/*.o) $(BIN_TARGET)

test:
	$(CC) HttpConnect.cpp $(object) $(CFLAGS) -o hms
