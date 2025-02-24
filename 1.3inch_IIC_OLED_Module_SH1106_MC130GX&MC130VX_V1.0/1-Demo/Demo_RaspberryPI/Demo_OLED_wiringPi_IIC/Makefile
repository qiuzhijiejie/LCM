DIR_CUR = $(shell pwd)
DIR_SRC = $(DIR_CUR)/source/src
DIR_INC = $(DIR_CUR)/source/include
DIR_OUT = $(DIR_CUR)/output

OBJ_C = $(wildcard ${DIR_SRC}/*.c)
OBJ_O = $(patsubst %.c,${DIR_OUT}/%.o,$(notdir ${OBJ_C}))

TARGET = 1.3_IIC_OLED

CC = gcc

DEBUG = -g -O0
CFLAGS += $(DEBUG)

LIB = -lwiringPi

${TARGET}:${OBJ_O}
	$(CC) $(CFLAGS) $(OBJ_O) -o $@ $(LIB)

${DIR_OUT}/%.o : $(DIR_SRC)/%.c
	$(CC) $(CFLAGS) -c  $< -o $@ $(LIB) -I$(DIR_INC)  

clean :
	$(RM) $(DIR_OUT)/*.* $(TARGET)
