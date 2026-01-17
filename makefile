CC = clang
LIB_BASE = C:/Users/i5lot/scoop/apps/glfw/current


CFLAGS = -Wall -Wextra -std=c99 -I"$(LIB_BASE)/include" -I"src/include" -I"src/cores"

LDFLAGS = -L"$(LIB_BASE)/lib-vc2022" -lglfw3 -lopengl32 -lgdi32 -luser32 -lshell32 \
          -Xlinker /NODEFAULTLIB:libcmt -lmsvcrt

TARGET = out
SRC = src/main.c src/glad.c
OUT_DIR = build
EXEC = $(OUT_DIR)/$(TARGET).exe

.SILENT:

all: $(OUT_DIR) $(EXEC)

$(OUT_DIR):
	if not exist $(OUT_DIR) mkdir $(OUT_DIR)

$(EXEC): $(SRC)
	$(CC) $(SRC) -o $(EXEC) $(CFLAGS) $(LDFLAGS)

clean:
	if exist $(OUT_DIR) rmdir /s /q $(OUT_DIR)

run: all
	./$(EXEC)
