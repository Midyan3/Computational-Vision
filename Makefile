########################################
##
## Makefile
## LINUX compilation 
##
##############################################


#FLAGS
C++FLAG = -g -std=c++14

MATH_LIBS = -lm

EXEC_DIR=.


.cc.o:
	g++ $(C++FLAG) $(INCLUDES)  -c $< -o $@


#Including
INCLUDES=  -I. 

#-->All libraries 
LIBS_ALL =  -L/usr/lib -L/usr/local/lib 

# P1
CC_OBJ_1=image.o p1.o

PROGRAM_NAME_1=p1

$(PROGRAM_NAME_1): $(CC_OBJ_1)
	g++ $(C++FLAG) -o $(EXEC_DIR)/$@ $(CC_OBJ_1) $(INCLUDES) $(LIBS_ALL)

# P2
CC_OBJ_2=image.o DisjSets.o p2.o

PROGRAM_NAME_2=p2

$(PROGRAM_NAME_2): $(CC_OBJ_2)
	g++ $(C++FLAG) -o $(EXEC_DIR)/$@ $(CC_OBJ_2) $(INCLUDES) $(LIBS_ALL)

# P3
CC_OBJ_3=image.o p3.o

PROGRAM_NAME_3=p3

$(PROGRAM_NAME_3): $(CC_OBJ_3)
	g++ $(C++FLAG) -o $(EXEC_DIR)/$@ $(CC_OBJ_3) $(INCLUDES) $(LIBS_ALL)

# P4
CC_OBJ_4=image.o p4.o

PROGRAM_NAME_4=p4

$(PROGRAM_NAME_4): $(CC_OBJ_4)
	g++ $(C++FLAG) -o $(EXEC_DIR)/$@ $(CC_OBJ_4) $(INCLUDES) $(LIBS_ALL)


CC_OBJ_5=image.o h1.o

PROGRAM_NAME_5=p5

$(PROGRAM_NAME_5): $(CC_OBJ_5)
	g++ $(C++FLAG) -o $(EXEC_DIR)/$@ $(CC_OBJ_5) $(INCLUDES) $(LIBS_ALL)

CC_OBJ_6=image.o h2.o

PROGRAM_NAME_6=p6

$(PROGRAM_NAME_6): $(CC_OBJ_6)
	g++ $(C++FLAG) -o $(EXEC_DIR)/$@ $(CC_OBJ_6) $(INCLUDES) $(LIBS_ALL)

all:
	make $(PROGRAM_NAME_1)
	make $(PROGRAM_NAME_2)
	make $(PROGRAM_NAME_3) 
	make $(PROGRAM_NAME_4)
	make $(PROGRAM_NAME_5)
	make $(PROGRAM_NAME_6) 
clean:
	(rm -f *.o; rm p1; rm p2; rm p3; rm p4; rm p5; rm p6) # Update this line
