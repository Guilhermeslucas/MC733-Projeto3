# ####################################################
# Hello World
# ####################################################

#changed as this option was given in path
CC = mips-newlib-elf-gcc
CFLAGS = -msoft-float -specs=archc
LDFLAGS = -lm -L${HOME}/gmp-mips-build/lib -lgmp

TARGET = paillierMulticore.mips
INC_DIR := -I. 

SRCS := paillierMulticore.c
OBJS := $(SRCS:.c=.o)

#------------------------------------------------------
.SILENT:

#------------------------------------------------------
.SUFFIXES: .o .c .h

#------------------------------------------------------
all: $(OBJS)
	$(CC) $(CFLAGS) -I${HOME}/gmp-mips-build/include $(OBJS) -o $(TARGET) $(LDFLAGS)
#------------------------------------------------------
clean:
	rm -f *.o *~ $(TARGET)
#------------------------------------------------------
distclean: clean
#------------------------------------------------------
.c.o:
	$(CC) -c $(CFLAGS) $(INC_DIR) -c $<
	



