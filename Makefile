CC=g++
CFLAGS=-c -Wall -std=c++20 -pthread -Iinclude/bmp_h -Iinclude/methods -Itest
LDFLAGS=-pthread

SOURCES=src/main.cpp src/methods/methods.cpp
HEADERS= include/bmp_h/bmp.h include/methods/methods.h

OBJECTS=$(patsubst src/%.cpp, obj/%.o, $(SOURCES))

EXECUTABLE=main # Имя основного исполняемого файла

OBJDIR=obj

all: $(OBJDIR) $(EXECUTABLE)
$(OBJDIR):
	@mkdir -p $(OBJDIR)/methods

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

obj/%.o: src/%.cpp
	$(CC) $(CFLAGS) $< -o $@

obj/methods/%.o: src/methods/%.cpp
	$(CC) $(CFLAGS) $< -o $@


clean:
	rm -rf $(OBJDIR) $(EXECUTABLE) output_*.bmp

.PHONY: all test clean