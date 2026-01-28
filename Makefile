CC = gcc
CFLAGS = -Wall -Wextra -g -I.

TARGET = server

SRC = main.c \
      Server.c \
      HttpRequest.c \
      HttpServer.c \
      DataStructures/Common/Node.c \
      DataStructures/Dictionary/Dictionary.c \
      DataStructures/Dictionary/Entry.c \
      DataStructures/Lists/LinkedList.c \
      DataStructures/Lists/Queue.c \
      DataStructures/Trees/BinarySearchTree.c

OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

re: clean all