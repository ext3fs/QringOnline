
.SUFFIXES : .c .o

CC = gcc

INCL	= -I/home/db/include/mysql
LIBS	= -L/home/db/lib/mysql -lmysqlclient -lz

OBJS	= main.o

TARGET	= test

.c.o	:
	$(CC) -c -Wall $< $(INCL)

$(TARGET) : $(OBJS)
	$(CC) -g -o $(TARGET) $(OBJS) $(LIBS) 

clean :
	rm -rf $(OBJS) $(TARGET) core

new :
	$(MAKE) clean
	$(MAKE)

