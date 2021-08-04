#__________________________________________________________
#
#       project name : Qring Online
#	filename     : Makefile
#	description  : Make file
#	date         : February 14 2005
#__________________________________________________________
#

CC	= gcc
LIBS	= -lpthread 		
OBJECTS	= main.o wrap.o util.o db.o mk_packet.o recv.o proc.o buff.o send.o tick.o tick_char.o tick_mob.o tick_item.o crypt.o
TARGET	= gamed


.SUFFIXES : .c .o

.c.o :
	$(CC) -c -g -Wall $<

$(TARGET)	: $(OBJECTS) 
	$(CC) -o $(TARGET) $(OBJECTS) $(LIBS)
	
clean	:
	rm -rf $(TARGET) $(OBJECTS) core.*
