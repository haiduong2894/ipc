include ./Makefile.inc
# the compiler: gcc for C program, define as g++ for C++
CC = gcc

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall 
LIBS = -lrt
INCLUDES = -I../include -I./lib
LFLAGS = -L../lib 

# define the C source files
SRCS = error_functions.c curr_time.c get_num.c

OBJS = $(SRCS:.c=.o)

# the build target executable:
TARGET = pmsg_getattr pmsg_create
 
#.c.o:
#	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

all:	
	@ for output in ${TARGET} ; do \
	#echo $(CC) $(CFLAGS) $(INCLUDES) $(SRCS) $$output.c  $(LFLAGS) $(LIBS) -o  $$output; \
	$(CC) $(CFLAGS) $(INCLUDES) $(SRCS) $$output.c  $(LFLAGS) $(LIBS) -o  $$output; \
	done

clean:
	$(RM) *.o *~ $(TARGET) 

depend: $(SRCS) 
	makedepend $(INCLUDES) $^


# DO NOT DELETE THIS LINE -- make depend needs it
