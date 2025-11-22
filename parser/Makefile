.PHONY: all test clean
# Executable name
TARGET = parser

# Source files
LEXER_SRC = lexer.l
PARSER_SRC = parser.y
AST_SRC = ast.c

# Generated files
LEXER_C = lexer.c
PARSER_C = parser.tab.c
PARSER_H = parser.tab.h
AST_O = ast.o

# Compiler and flags
CC = gcc
CFLAGS = -lm

# Default rule: build everything
all: $(TARGET)

# Generate the lexer using Flex
$(LEXER_C): $(LEXER_SRC)
	flex -o $(LEXER_C) $(LEXER_SRC)

# Generate the parser using Bison
$(PARSER_C) $(PARSER_H): $(PARSER_SRC)
	bison -d -o $(PARSER_C) $(PARSER_SRC)

# Compile the AST module
$(AST_O): $(AST_SRC)
	$(CC) -c $(AST_SRC) -o $(AST_O)

# Compile the final executable
$(TARGET): $(PARSER_C) $(LEXER_C) $(AST_O)
	$(CC) -o $(TARGET) $(PARSER_C) $(LEXER_C) $(AST_O) $(CFLAGS)

# Rule to run the parser
run: $(TARGET)
	./$(TARGET)

# Clean up generated files
clean:
	rm -f $(TARGET) $(LEXER_C) $(PARSER_C) $(PARSER_H) $(AST_O)
