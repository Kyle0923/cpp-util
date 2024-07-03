
SRC := src/lambda1.cpp
OBJ := $(SRC:.cpp=.o)

ARTIFACT := main

all: $(ARTIFACT)

%.o: %.cpp
	g++ $(INC) -c $< -o $@

$(ARTIFACT): $(OBJ)
	g++ $^ -o $@

clean:
	rm -fr $(ARTIFACT) $(OBJ)
