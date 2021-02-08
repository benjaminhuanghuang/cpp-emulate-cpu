

CXX       = gcc
EXEBIN    = cpu.exe
SOURCES   = main.cpp #p2funargs.cpp #data-type.cpp #arithmatic.cpp
FLAGS     = -std=c++11 -Wall

all: $(EXEBIN)

$(EXEBIN) : $(SOURCES)
	$(CXX) $(FLAGS) $(SOURCES) -o $(EXEBIN)

clean :
	rm -f $(EXEBIN)