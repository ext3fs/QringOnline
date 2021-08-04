#___________________________________________________________________________
#
#       	project name : AgentServer
#		filename     : Makefile
#		date         : 2004-11-16
#_____________________________________________________________________________
#

CC 		= gcc

TARGET		= agentd

OBJECTS 	= main.o db.o util.o accept.o recv.o 
CFLAGS		= -g -c -Wall -D_REENTRANT
OFLAGS 		= -o 

LIBS		= -lpthread -L/home/db/lib/mysql -lmysqlclient 
INCL    	= -I/usr/include/mysql

$(TARGET)	: $(OBJECTS)
		$(CC) $(OFLAGS) $(TARGET) $(OBJECTS) $(LIBS)
.c.o	:
		$(CC) $(CFLAGS) $< $(INCL)

clean :
		rm -rf $(OBJECTS) $(TARGET) core core.*
cleanobject :
		rm -rf $(OBJECTS) core core.*
