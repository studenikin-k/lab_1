CC=g++
# Добавляем -I для указания директорий, где искать заголовочные файлы
# src/bmp_h - для bmp.h
# src/methods - для methods.h
CFLAGS=-c -Wall -std=c++20 -pthread -Isrc/bmp_h -Isrc/methods
LDFLAGS=-pthread

# Обновляем пути к исходным файлам
SOURCES=src/main.cpp src/methods/methods.cpp
HEADERS= include/bmp_h/bmp.h include/methods/methods.h
OBJECTS=$(patsubst src/%.cpp, obj/%.o, $(SOURCES)) # Изменяем путь к объектным файлам

EXECUTABLE=main

# Директория для объектных файлов
OBJDIR=obj

all: $(OBJDIR) $(EXECUTABLE)

$(OBJDIR):
	@mkdir -p $(OBJDIR)/methods # Создаем поддиректорию для объектных файлов методов

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

# Правило для .cpp файлов, чтобы они компилировались в obj/ директорию
# и создавали необходимые поддиректории
obj/%.o: src/%.cpp
	$(CC) $(CFLAGS) $< -o $@

# Специальное правило для methods.cpp, чтобы он попадал в obj/methods/
obj/methods/%.o: src/methods/%.cpp
	$(CC) $(CFLAGS) $< -o $@


clean:
	rm -rf $(OBJDIR) $(EXECUTABLE)