CXX=arm-926ejs-linux-gnueabi-g++
CC=arm-926ejs-linux-gnueabi-gcc

SOURCES = $(patsubst src/%, %, $(wildcard src/*.c))
SOURCES += device/main.c device/i_fb_video.c device/i_no_sound.c device/i_no_music.c
OBJECTS = $(patsubst %.c, %.o, $(SOURCES))
TARGET_OBJS = $(patsubst %, obj/%, $(OBJECTS))
CCFLAGS = -DNORMALUNIX -std=gnu99

.PHONY:
all: doom

.PHONY: sources
sources:
	@echo $(SOURCES)

.PHONY: objects
objects:
	@echo $(TARGET_OBJS)

obj:
	mkdir -p obj/device

obj/%.o: src/%.c
	@mkdir -p obj/device
	@echo "Compiling $<..."
	@$(CC) -c $< -I src $(CCFLAGS) -g -o $@

obj/device/%.o: src/device/%.c
	@echo "Compiling $<..."
	@$(CC) -c $< -I src $(CCFLAGS) -g -o $@

.PHONY:
link: $(TARGET_OBJS)
	@echo "Linking..."
	@mkdir -p bin
	@$(CC) -ggdb -o bin/doom $(TARGET_OBJS)

doom: link

.PHONY: clean
clean:
	rm -rf obj/
	rm -rf bin/
