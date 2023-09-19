##version1
##第一种makefile的写法
# hello:practice1.cpp add.cpp
# 	g++ -o hello practice1.cpp add.cpp

##version2
##第二种makefile的写法
##更专业，定义一些新变量
# CXX = g++
# TARGET = hello
# OBJECTS = practice1.o add.o

# $(TARGET):$(OBJECTS)
# 	$(CXX) -o $(TARGET) $(OBJECTS)

# practice1.o:practice1.cpp
# 	$(CXX) -c practice1.cpp

# add.o:add.cpp
# 	$(CXX) -c add.cpp

##version3
# CXX = g++
# TARGET = hello
# OBJECTS = practice1.o add.o

# ##显示所有的warning
# CXXFLAGS = -c -Wall

# $(TARGET):$(OBJECTS)
# 	$(CXX) -o $@ $^
# %.o:%.cpp
# 	$(CXX) $(CXXFLAGS) $< -o $@

# ##这一句是防止我们在当前目录下有一个clean文件，那我们执行make clean就会当成编译
# .PHONY:clean
# ##这里很神，就是当我们执行make clean的时候，就会执行下面的命令
# clean:
# 	rm -rf $(TARGET) $(OBJECTS)


##version4,最好的makefile
CXX = g++
TARGET = hello
SRC = $(wildcard *.cpp)
##下面这一句是将SRC中的.cpp替换成.o
OBJECTS = $(patsubst %.cpp,%.o,$(SRC))

CXXFLAGS = -c -Wall

$(TARGET):$(OBJECTS)
	$(CXX) -o $@ $^
%.o:%.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

.PHONY:clean
clean:
	rm -rf $(TARGET) $(OBJECTS)