#include "sensor.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    SensorStats stats;

    if (argc != 3) {
        fprintf(stderr, "Uso: %s <archivo_entrada.csv> <archivo_reporte.txt>\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (process_sensor_file(argv[1], argv[2], &stats) != 0) {
        return EXIT_FAILURE;
    }

    printf("Analisis completado correctamente.\n");
    printf("Muestras procesadas: %zu\n", stats.total_samples);
    printf("Temperatura promedio: %.2f C\n", stats.average_temperature);
    printf("Humedad promedio: %.2f %%\n", stats.average_humidity);
    printf("CO2 promedio: %.2f ppm\n", stats.average_co2);
    printf("Reporte generado en: %s\n", argv[2]);

    return EXIT_SUCCESS;
}
