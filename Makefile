
CXX      = clang++
CXXFLAGS = -g3 -Wall -Wextra -std=c++11 -O2
LDFLAGS  = -g3

gerp: main.o Database.o DirNode.o FSTree.o LinkedList.o
	${CXX} ${LDFLAGS} -o gerp main.o Database.o DirNode.o FSTree.o LinkedList.o

main.o: main.cpp Database.o

LinkedList.o: LinkedList.cpp LinkedList.h NodeType.h
	${CXX} ${CXXFLAGS} -c LinkedList.cpp NodeType.h

Database.o: Database.cpp Database.h Entry.h FSTree.h DirNode.h LinkedList.h LinkedList.cpp
	${CXX} ${CXXFLAGS} -c Database.cpp LinkedList.cpp

