SHELL = C:/msys64/usr/bin/sh.exe
CC = C:/msys64/ucrt64/bin/gcc.exe
CFLAGS = -Wall -Wextra -pedantic -std=c11 -O2 -Iinclude
MKDIR_P = C:/msys64/usr/bin/mkdir.exe -p
RM = C:/msys64/usr/bin/rm.exe -f
CAT = C:/msys64/usr/bin/cat.exe
TARGET = build/iot_report.exe
SRC_DIR = src
BUILD_DIR = build
DATA_DIR = data
REPORT_DIR = reports
REPORT_FILE = $(REPORT_DIR)/iot_report.txt
SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/sensor.c
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

.PHONY: all help run report clean rebuild

all: $(TARGET)

help:
	@echo "Objetivos disponibles:"
	@echo "  make all      -> compila el ejecutable"
	@echo "  make run      -> compila, ejecuta y genera el reporte"
	@echo "  make report   -> muestra el reporte generado"
	@echo "  make clean    -> elimina archivos generados"
	@echo "  make rebuild  -> limpia y compila de nuevo"

$(BUILD_DIR):
	$(MKDIR_P) $(BUILD_DIR)

$(REPORT_DIR):
	$(MKDIR_P) $(REPORT_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c include/sensor.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(REPORT_FILE): $(TARGET) $(DATA_DIR)/sensor_data.csv | $(REPORT_DIR)
	./$(TARGET) $(DATA_DIR)/sensor_data.csv $(REPORT_FILE)

run: $(REPORT_FILE)

report: $(REPORT_FILE)
	$(CAT) $(REPORT_FILE)

clean:
	$(RM) $(BUILD_DIR)/*.o $(TARGET) $(REPORT_FILE)

rebuild: clean all
