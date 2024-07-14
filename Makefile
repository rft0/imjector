CC = clang
FLAGS = -std=c++20 -Wall

TARGET_X86 = ./bin/imjector_x86.exe
TARGET_X64 = ./bin/imjector_x64.exe

LIBS = -ld3d9 -luser32 -lcomdlg32 -lShlwapi

SRC_FILES = $(wildcard src/*.cpp)
IMGUI_FILES = $(wildcard src/imgui/*.cpp)

all: x86 x64

x86:
	$(CC) $(IMGUI_FILES) $(SRC_FILES) $(FLAGS) -m32 $(LIBS) -o $(TARGET_X86)

x64:
	$(CC) $(IMGUI_FILES) $(SRC_FILES) $(FLAGS) $(LIBS) -o $(TARGET_X64)