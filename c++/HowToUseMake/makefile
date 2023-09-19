##version1
##第一种makefile的写法
# hello:practice1.cpp add.cpp
# 	g++ -o hello practice1.cpp add.cpp

##version2
##第二种makefile的写法
##更专业，定义一些新变量
CXX = g++
TARGET = hello
OBJECTS = practice1.o add.o

$(TARGET):$(OBJECTS)
	$(CXX) -o $(TARGET) $(OBJECTS)

practice1.o:practice1.cpp
	$(CXX) -c practice1.cpp

add.o:add.cpp
	$(CXX) -c add.cpp

