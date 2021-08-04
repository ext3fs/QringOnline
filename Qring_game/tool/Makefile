#____________________________________________________________________________
#
#	project name : Game Server  
#	filename     : Makefile
#	description  : Game Server Manager Tool Make file
#	date         : may 27 2004
#_____________________________________________________________________________
#

OBJECTS	= manager.o 
CC	= gcc
TARGET	= manager


.SUFFIXES : .c .o

.c.o :
	$(CC) -c -Wall $<

$(TARGET)	: $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS)
	
clean :
	rm -rf $(TARGET) $(OBJECTS) core
