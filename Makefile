CC = gcc
CFLAGS = -Iinclude -Iexternal/unity/src
LDFLAGS =

# Source and object files for the main application
SRC_DIR = src
OBJ_DIR = obj
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

# Exclude main.o for test builds
OBJ_FILES_NO_MAIN = $(filter-out $(OBJ_DIR)/main.o, $(OBJ_FILES))

# Test files
TEST_SRC_DIR = tests
TEST_FILES = $(wildcard $(TEST_SRC_DIR)/*.c)

# Unity files
UNITY_ROOT = external/unity
UNITY_SRC = $(UNITY_ROOT)/src/unity.c

# Application binary
APP = myapp

# Test binary
TEST = test_runner

.PHONY: all clean test runtest

all: $(APP)

$(APP): $(OBJ_FILES)
	$(CC) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

test: $(TEST)

$(TEST): $(TEST_FILES) $(UNITY_SRC) $(OBJ_FILES_NO_MAIN)
	$(CC) $(CFLAGS) -o $@ $^

runtest:
	$(CC) $(CFLAGS) -o $(TEST) $(TEST_SRC_DIR)/$(TESTNAME).c $(UNITY_SRC) $(OBJ_FILES_NO_MAIN)
	./$(TEST)

clean:
	rm -f $(OBJ_DIR)/*.o $(APP) $(TEST)
