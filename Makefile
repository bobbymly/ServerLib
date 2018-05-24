# MAINSOURCE代表含有main入口函数的cpp文件
MAINSOURCE :=  ServerTest.cpp
SOURCE  := $(wildcard  base/*.cpp  tests/*.cpp net/*.cpp )
override SOURCE := $(filter-out $(MAINSOURCE),$(SOURCE))
OBJS    := $(patsubst %.cpp,%.o,$(SOURCE))

TARGET  :=  ServerTest
CC      := g++
LIBS    := -lpthread -lrt 
INCLUDE:= -I./usr/local/lib
CFLAGS  := -std=c++11 -g -Wall -O3 -D_PTHREADS
CXXFLAGS:= $(CFLAGS)

# Test object

.PHONY : clean 
all : $(TARGET) $(SUBTARGET1) $(SUBTARGET2) clean
objs : $(OBJS)
rebuild: veryclean all
clean: 
	find . -name '*.o' | xargs rm -f

debug:
	@echo $(SOURCE)

$(TARGET) : $(OBJS) ServerTest.o
	$(CC) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)
# $@代表目标，这里是$(TARGET)

