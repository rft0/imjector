CC = clang
FLAGS = -std=c++20 -Wall

TARGET_DIR = ./bin
TARGET_NAME = out.exe

LLIBS = -ld3d9 -luser32 -lcomdlg32

SRC_FILES = $(wildcard src/*.cpp)
IMGUI_FILES = $(wildcard imgui/*.cpp)

all: build

build:
	$(CC) $(FLAGS) $(IMGUI_FILES) $(SRC_FILES) -o $(TARGET_DIR)/$(TARGET_NAME) $(LLIBS)

run:
	$(TARGET_DIR)/$(TARGET_NAME)

clean:
	rm -rf ./bin/*