CC = clang

GLFW_BASE = C:/Users/i5lot/scoop/apps/glfw/current
CGLM_BASE = D:/WDownloads/cglm-0.9.6/cglm-0.9.6

CFLAGS = -Wall -Wextra -std=c99 -I"$(GLFW_BASE)/include" -I"$(CGLM_BASE)/include" -I"include" -I"src/cores" -D_CRT_SECURE_NO_WARNINGS

LDFLAGS = -L"$(GLFW_BASE)/lib-vc2022" -lglfw3 -lopengl32 -lgdi32 -luser32 -lshell32 \
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
