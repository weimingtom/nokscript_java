# cd /cygdrive/D/ugame_c/testnokscript
# make

CC := gcc
CPP := g++
LD := ld
RM := rm -rf

CPPFLAGS := -g
OBJS := CtrlFunc.o DxFunc.o HelpFunc.o
OBJS += Main.o MakeFunc.o Misc.o 
OBJS += NkScript.o OpFunc.o 

all : nokscript

nokscript : $(OBJS)
	$(CPP) $(CPPFLAGS) -o $@ $(OBJS)

%.o : %.cpp
	$(CPP) $(CPPFLAGS) -o $@ -c $<

clean :
	$(RM) nokscript *.o *.exe
