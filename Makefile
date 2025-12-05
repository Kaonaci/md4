# Компилятор и флаги
CC = gcc
CFLAGS_DEBUG = -O0 -g -Wall -Wextra -std=c99
CFLAGS_RELEASE = -O3 -march=native -flto -DNDEBUG -std=c99
CFLAGS_PROFILE = -O2 -pg -std=c99

# Имена выходных файлов
TARGET = md4
TARGET_DEBUG = md4_debug
TARGET_PROFILE = md4_profile

# Исходный файл
SRC = md4.c

# Phony targets (цели, которые не создают файлы с таким именем)
.PHONY: all debug release profile clean test help

# Цель по умолчанию - сборка релизной версии
all: release

# Отладочная сборка
debug: $(TARGET_DEBUG)

$(TARGET_DEBUG): $(SRC)
	$(CC) $(CFLAGS_DEBUG) $< -o $@

# Релизная сборка
release: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS_RELEASE) $< -o $@

# Профилировочная сборка
profile: $(TARGET_PROFILE)

$(TARGET_PROFILE): $(SRC)
	$(CC) $(CFLAGS_PROFILE) $< -o $@

# Очистка скомпилированных файлов
clean:
	rm -f $(TARGET) $(TARGET_DEBUG) $(TARGET_PROFILE) *.o gmon.out

# Тестирование
test: debug
	@echo "Running basic tests..."
	./$(TARGET_DEBUG) "test" | grep -q "db346d691d7acc4dc2625db19f9e3f52" && echo "Test 1 passed" || echo "Test 1 failed"
	./$(TARGET_DEBUG) "hello world" | grep -q "aa010fbc1d14c795d86ef98c95479d17" && echo "Test 2 passed" || echo "Test 2 failed"

# Справка
help:
	@echo "Available targets:"
	@echo "  all     - Build release version (default)"
	@echo "  debug   - Build debug version"
	@echo "  release - Build optimized release version"
	@echo "  profile - Build version for profiling"
	@echo "  test    - Run basic tests"
	@echo "  clean   - Remove compiled files"
	@echo "  help    - Show this help"