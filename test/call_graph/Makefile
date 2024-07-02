
# SRC := src/c1.cpp src/c2.cpp
SRC := src/c1.cpp src/c2.cpp
OBJ := $(SRC:.cpp=.o)
# INC := -Iinc1/ -Iinc2/

ARTIFACT := main

all: $(ARTIFACT)

%.o: %.cpp
	g++ $(INC) -c $< -o $@

$(ARTIFACT): $(OBJ)
	g++ $^ -o $@

clean:
	rm -fr $(ARTIFACT) $(OBJ)
