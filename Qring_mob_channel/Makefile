#___________________________________________________________________________
#
#       	project name : MobServer
#		filename     : Makefile
#		date         : 2004-11-16
#_____________________________________________________________________________
#

CC 		= gcc

TARGET		= mobd

OBJECTS 	= main.o util.o sp.o mymath.o tick.o send.o recv.o proc.o db.o
OFLAGS 		= -o 
CFLAGS		=  -g -c -Wall -D_REENTRANT 
#CFLAGS		= -O2 -g -c -Wall -D_REENTRANT 

INCL    	= 
LIBS		= -lpthread -lm

$(TARGET)	: $(OBJECTS)
		$(CC) $(OFLAGS) $(TARGET) $(OBJECTS) $(LIBS)
clean :
		rm -rf $(OBJECTS) $(TARGET) core core.*
cleanobject :
		rm -rf $(OBJECTS) core core.*

.c.o	:
		$(CC) $(CFLAGS) $< $(INCL)
