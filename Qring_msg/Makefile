#__________________________________________________________
#
#       project name : Qring online
#	filename     : Makefile
#	description  : Make file
#	date         : February 14 2005
#__________________________________________________________
#

CC	= gcc
LIBS	= -lpthread 		
OBJECTS	= main.o wrap.o util.o db.o mk_packet.o recv.o tick.o proc.o buff.o send.o
TARGET	= msgd_qring


.SUFFIXES : .c .o

.c.o :
	$(CC) -c -Wall $<

$(TARGET)	: $(OBJECTS) 
	$(CC) -o $(TARGET) $(OBJECTS) $(LIBS)
	
clean	:
	rm -rf $(TARGET) $(OBJECTS) core
