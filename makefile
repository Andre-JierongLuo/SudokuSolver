CXX = g++
CXXFLAGS = -g
OBJECTS = driver.o sudoku.o cell.o

EXEC = solver

all : ${EXEC}

${EXEC} : ${OBJECTS}
        ${CXX} ${CXXFLAGS} ${OBJECTS} -o $@

cell.o : cell.h

sudoku.o : sudoku.h

driver.o : driver.cpp

clean :
        rm -f ${EXEC} ${OBJECTS}
