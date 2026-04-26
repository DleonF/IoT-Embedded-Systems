# Proyecto IoT con Makefile

## Descripcion del proyecto

En este proyecto desarrollé un programa en C que analiza lecturas de un sistema IoT almacenadas en un archivo CSV. A partir de esos datos, puede calcular estadisticas basicas de temperatura, humedad y CO2, y generar un reporte de texto con los resultados.

La idea principal fue aplicar el uso de un `Makefile` para automatizar el flujo completo del proyecto: compilacion, ejecucion, generacion del reporte y limpieza de archivos temporales.

## Estructura del repositorio

```text
Makefiles/
├── Makefile
├── README.md
├── .gitignore
├── data/
│   └── sensor_data.csv
├── include/
│   └── sensor.h
├── reports/
│   └── .gitkeep
└── src/
    ├── main.c
    └── sensor.c
```

## Archivos principales

- `Makefile`: automatiza compilacion, ejecucion, visualizacion del reporte y limpieza.
- `src/main.c`: recibe argumentos, llama al analisis y muestra un resumen en consola.
- `src/sensor.c`: procesa el archivo CSV y genera el reporte.
- `include/sensor.h`: define la estructura `SensorStats` y la funcion `process_sensor_file`.
- `data/sensor_data.csv`: archivo de entrada con lecturas del sistema IoT.
- `reports/`: carpeta donde se genera el reporte final.

## Herramientas necesarias

- `gcc`
- `make`

En mi equipo utilice:

- `C:\msys64\ucrt64\bin\gcc.exe`
- `C:\msys64\usr\bin\make.exe`

En mi caso, `make` se ejecuto usando su ruta completa en Windows.

## Funcionalidad del programa

El programa lee cinco muestras de prueba desde `data/sensor_data.csv` y produce un reporte con:

- numero de muestras procesadas
- temperatura promedio
- humedad promedio
- promedio de CO2
- temperatura maxima registrada
- numero de alertas por temperatura alta
- numero de alertas por CO2 alto

## Explicacion del Makefile

```make
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
```

### Funcion de cada seccion

**1. Variables**

Las variables centralizan nombres de archivos, carpetas, comandos y banderas. Con esto el `Makefile` queda mas facil de mantener, porque si cambia el compilador, el nombre del ejecutable o la ubicacion del reporte, solo tengo que modificar una linea.

**2. Objetivos `.PHONY`**

Los objetivos `all`, `help`, `run`, `report`, `clean` y `rebuild` son tareas. No dependen de archivos con el mismo nombre.

**3. Directorios**

Las reglas:

```make
$(BUILD_DIR):
	$(MKDIR_P) $(BUILD_DIR)

$(REPORT_DIR):
	$(MKDIR_P) $(REPORT_DIR)
```

crean automaticamente las carpetas necesarias antes de compilar o generar reportes.

**4. Regla patron**

```make
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c include/sensor.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
```

Esta regla transforma cualquier archivo `.c` dentro de `src/` en su correspondiente archivo `.o` dentro de `build/`.

- `$<` representa la primera dependencia.
- `$@` representa el nombre del objetivo.

**5. Enlace**

```make
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@
```

Une los objetos compilados para crear `build/iot_report.exe`.

**6. Generacion del reporte**

```make
$(REPORT_FILE): $(TARGET) $(DATA_DIR)/sensor_data.csv | $(REPORT_DIR)
	./$(TARGET) $(DATA_DIR)/sensor_data.csv $(REPORT_FILE)
```

Esta es la parte mas importante de la automatizacion: el reporte depende del ejecutable y del archivo de datos. Si alguno cambia, `make` vuelve a ejecutar el programa y actualiza el reporte.

**7. Limpieza y reconstruccion**

`make clean` elimina objetos, ejecutable y reporte generado.

`make rebuild` fuerza una compilacion desde cero.

## Instrucciones de ejecucion

### Opcion 1: desde PowerShell en Windows

Ubicarse en la carpeta del proyecto:

```powershell
cd C:\DiscoD\ESCOM-ISC\8voSem\IoT\Makefiles
```

Mostrar ayuda:

```powershell
C:\msys64\usr\bin\make.exe help
```

Compilar:

```powershell
C:\msys64\usr\bin\make.exe all
```

Compilar, ejecutar y generar el reporte:

```powershell
C:\msys64\usr\bin\make.exe run
```

Mostrar el reporte:

```powershell
C:\msys64\usr\bin\make.exe report
```

Limpiar archivos generados:

```powershell
C:\msys64\usr\bin\make.exe clean
```

Reconstruir:

```powershell
C:\msys64\usr\bin\make.exe rebuild
```

### Opcion 2: si `make` ya esta en el PATH

```bash
make help
make all
make run
make report
make clean
make rebuild
```

## Resultados obtenidos

### Salida de `make help`

```text
Objetivos disponibles:
  make all      -> compila el ejecutable
  make run      -> compila, ejecuta y genera el reporte
  make report   -> muestra el reporte generado
  make clean    -> elimina archivos generados
  make rebuild  -> limpia y compila de nuevo
```

### Salida de `make all`

```text
C:/msys64/ucrt64/bin/gcc.exe -Wall -Wextra -pedantic -std=c11 -O2 -Iinclude -c src/main.c -o build/main.o
C:/msys64/ucrt64/bin/gcc.exe -Wall -Wextra -pedantic -std=c11 -O2 -Iinclude -c src/sensor.c -o build/sensor.o
C:/msys64/ucrt64/bin/gcc.exe -Wall -Wextra -pedantic -std=c11 -O2 -Iinclude build/main.o build/sensor.o -o build/iot_report.exe
```

### Salida de `make run`

```text
./build/iot_report.exe data/sensor_data.csv reports/iot_report.txt
Analisis completado correctamente.
Muestras procesadas: 5
Temperatura promedio: 29.10 C
Humedad promedio: 40.60 %
CO2 promedio: 824.00 ppm
Reporte generado en: reports/iot_report.txt
```

### Contenido de `make report`

```text
Reporte de monitoreo IoT
========================
Muestras procesadas: 5
Temperatura promedio: 29.10 C
Humedad promedio: 40.60 %
CO2 promedio: 824.00 ppm
Temperatura maxima: 32.60 C
Alertas por temperatura (> 30.0 C): 2
Alertas por CO2 (> 900.0 ppm): 2
```

## Como se logra la automatizacion

La automatizacion se logra porque `make` evalua dependencias y ejecuta solo lo necesario:

- Si cambia un archivo fuente `.c`, recompila su `.o`.
- Si cambia algun objeto, vuelve a enlazar el ejecutable.
- Si cambia el ejecutable o el archivo `sensor_data.csv`, regenera `iot_report.txt`.
- Si no hubo cambios, evita trabajo innecesario.

Con esto el proceso queda estandarizado y se puede compilar y ejecutar el proyecto con comandos cortos y consistentes.

## Archivo .gitignore

Inclui un `.gitignore` para excluir archivos temporales o generados automaticamente:

- `build/`
- `reports/*.txt`
- `*.o`
- `*.exe`
- `~$*.docx`
- archivos del sistema como `Thumbs.db` y `.DS_Store`

Esto mantiene el repositorio limpio y facilita su revision.

## Organizacion del repositorio

Organice el proyecto para que cada tipo de archivo tenga una funcion clara:

- `src/` contiene el codigo fuente.
- `include/` contiene las declaraciones compartidas.
- `data/` guarda los datos de entrada.
- `reports/` almacena los resultados generados.
- `Makefile` centraliza la automatizacion del flujo.
- `README.md` documenta el funcionamiento del proyecto y su uso.

Esta organizacion hace que el repositorio sea mas facil de entender, ejecutar y revisar.

## Conclusion

Con este proyecto cumplo con el objetivo de construir un repositorio propio con `Makefile`, documentacion y control de archivos generados. La automatizacion no se limita a compilar el programa, sino que tambien ejecuta el analisis de datos y produce un reporte final de manera ordenada y repetible.
