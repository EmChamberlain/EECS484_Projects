.PHONY: clean

CC = g++
LD = g++
CFLAGS = -c -g -std=c++17 -Wall -Werror -pedantic-errors
LFLAGS = -g

OBJS = p3main.o BTree.o TreeNode.o LeafNode.o InnerNode.o DataEntry.o Utilities.o
PROG = proj3exe

default: $(PROG)

compile: $(PROG)

$(PROG): $(OBJS)
	@$(LD) $(LFLAGS) $(OBJS) -o $(PROG)

p3main.o: p3main.cpp BTree.h DataEntry.h
	@$(CC) $(CFLAGS) p3main.cpp

BTree.o: BTree.cpp BTree.h Utilities.h DataEntry.h TreeNode.h LeafNode.h
	@$(CC) $(CFLAGS) BTree.cpp

TreeNode.o: TreeNode.cpp TreeNode.h DataEntry.h Utilities.h InnerNode.h
	@$(CC) $(CFLAGS) TreeNode.cpp

LeafNode.o: LeafNode.cpp LeafNode.h DataEntry.h TreeNode.h InnerNode.h Utilities.h
	@$(CC) $(CFLAGS) LeafNode.cpp

InnerNode.o: InnerNode.cpp InnerNode.h TreeNode.h DataEntry.h Utilities.h
	@$(CC) $(CFLAGS) InnerNode.cpp

DataEntry.o: DataEntry.cpp DataEntry.h Utilities.h
	@$(CC) $(CFLAGS) DataEntry.cpp

Utilities.o: Utilities.cpp Utilities.h
	@$(CC) $(CFLAGS) Utilities.cpp

clean:
	@rm -f $(PROG)
	@rm -f *.o
