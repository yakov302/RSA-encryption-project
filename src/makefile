SRC_DIR = .
INC_DIR = ../inc

CXX = g++
CC = $(CXX)
WARNINGS = -pedantic
DEBUG = -g3
CXX_STD = c++17
LDLIBS  = -lpthread 

CXXFLAGS = $(WARNINGS) $(DEBUG) -std=$(CXX_STD) $(LDLIBS)
CPPFLAGS = -I$(INC_DIR)

TARGET = main
OBJS = ./main.o ./big_integer.o  ./rsa.o  ./app.o
$(TARGET):$(OBJS)

main.o: main.cpp $(INC_DIR)/big_integer.hpp $(INC_DIR)/rsa.hpp  $(INC_DIR)/time.hpp
	$(CC) $(CXXFLAGS) -c main.cpp

big_integer.o: big_integer.cpp $(INC_DIR)/big_integer.hpp 
	$(CC) $(CXXFLAGS) -c big_integer.cpp

rsa.o: rsa.cpp $(INC_DIR)/big_integer.hpp 
	$(CC) $(CXXFLAGS) -c rsa.cpp

app.o: app.cpp $(INC_DIR)/rsa.hpp 
	$(CC) $(CXXFLAGS) -c app.cpp

vala: $(TARGET)
	valgrind --log-file=V1.log ./$(TARGET)
	valgrind --log-file=V2.log --leak-check=full --track-origins=yes ./$(TARGET) 
   
check: $(TARGET)
	./$(TARGET) -v
	
clean:
	@$(RM) $(TARGET) $(OBJS)
	
.PHONY: check clean
