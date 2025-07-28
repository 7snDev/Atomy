# Works only on Linux, for Windows use g++ main.cpp -o main.exe or similar command

GCC=g++
CFLAGS=-std=c++17 -Wall -Wextra -O3
LDFLAGS=-lX11 -lpthread -lXtst
SOURCES=main.cpp
OBJDIR=bin
OBJECTS=$(addprefix $(OBJDIR)/,$(SOURCES:.cpp=.o))
TARGET=main

all: clean build postbuild

build: $(OBJDIR) $(TARGET)

postbuild:
	rm -rf $(OBJDIR)
	clear
	./$(TARGET)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(TARGET): $(OBJECTS)
	$(GCC) $(OBJECTS) -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	$(GCC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(TARGET)
