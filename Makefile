SRC = ./src
INC = ./includes
BIN = ./bin
LIB = ./lib

GCC_FLAGS = -Wall -g
LIB_FLAGS = -lescalona -L$(LIB)
INC_FLAGS = -I$(INC)

all: folders escalona
 
escalona: libescalona.a
	$(CC) $(GCC_FLAGS) $(INC_FLAGS) $(SRC)/escalona.c -o escalona $(LIB_FLAGS)

libescalona.a: conflict_detection.o equivalent_view.o
	$(AR) $(ARFLAGS) $(LIB)/libescalona.a $(BIN)/*.o

conflict_detection.o:
	$(CC) $(GCC_FLAGS) $(INC_FLAGS) -c $(SRC)/conflict_detection.c -o $(BIN)/conflict_detection.o

equivalent_view.o:
	$(CC) $(GCC_FLAGS) $(INC_FLAGS) -c $(SRC)/equivalent_view.c -o $(BIN)/equivalent_view.o

folders:
	@echo "Criando Pastas...\n"
	@if [ ! -d "$(BIN)" ]; then \
		mkdir $(BIN); \
	fi
	@if [ ! -d "$(LIB)" ]; then \
		mkdir $(LIB); \
	fi

.PHONY: clean
clean:
	@rm escalona & > /dev/null
	@echo "Limpando...\n"
	@if [ -d "$(BIN)" ]; then \
		rm -r $(BIN); \
	fi
	@if [ -d "$(LIB)" ]; then \
		rm -r $(LIB); \
	fi