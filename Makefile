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

# H1
CC_OBJ_1=image.o h1.o

PROGRAM_NAME_1=h1

$(PROGRAM_NAME_1): $(CC_OBJ_1)
	g++ $(C++FLAG) -o $(EXEC_DIR)/$@ $(CC_OBJ_1) $(INCLUDES) $(LIBS_ALL)

# H2
CC_OBJ_2=image.o h2.o

PROGRAM_NAME_2=h2

$(PROGRAM_NAME_2): $(CC_OBJ_2)
	g++ $(C++FLAG) -o $(EXEC_DIR)/$@ $(CC_OBJ_2) $(INCLUDES) $(LIBS_ALL)

# H3
CC_OBJ_3=image.o h3.o

PROGRAM_NAME_3=h3

$(PROGRAM_NAME_3): $(CC_OBJ_3)
	g++ $(C++FLAG) -o $(EXEC_DIR)/$@ $(CC_OBJ_3) $(INCLUDES) $(LIBS_ALL)

# H4
CC_OBJ_4=image.o DisjSets.o h4.o

PROGRAM_NAME_4=h4

$(PROGRAM_NAME_4): $(CC_OBJ_4)
	g++ $(C++FLAG) -o $(EXEC_DIR)/$@ $(CC_OBJ_4) $(INCLUDES) $(LIBS_ALL)

CC_OBJ_5 = image.o s1.o DisjSets.o

PROGRAM_NAME_5 = s1

$(PROGRAM_NAME_5): $(CC_OBJ_5)
	g++ $(C++FLAG) -o $(EXEC_DIR)/$@ $(CC_OBJ_5) $(INCLUDES) $(LIBS_ALL)

CC_OBJ_6 = image.o s2.o 

PROGRAM_NAME_6 = s2

$(PROGRAM_NAME_6): $(CC_OBJ_6)
	g++ $(C++FLAG) -o $(EXEC_DIR)/$@ $(CC_OBJ_6) $(INCLUDES) $(LIBS_ALL)

CC_OBJ_7 = image.o s3.o

PROGRAM_NAME_7 = s3

$(PROGRAM_NAME_7): $(CC_OBJ_7)
	g++ $(C++FLAG) -o $(EXEC_DIR)/$@ $(CC_OBJ_7) $(INCLUDES) $(LIBS_ALL)

all:
# make $(PROGRAM_NAME_1)
# make $(PROGRAM_NAME_2)
# make $(PROGRAM_NAME_3) 
# make $(PROGRAM_NAME_4) 	
	make $(PROGRAM_NAME_5)
	make $(PROGRAM_NAME_6)
	make $(PROGRAM_NAME_7)

clean:
	(rm -f *.o; rm h1; rm h2; rm h3; rm h4; rm s1; rm s2; rm s3)

(:
