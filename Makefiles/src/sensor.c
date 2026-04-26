#include "sensor.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_BUFFER_SIZE 256
#define TEMPERATURE_LIMIT 30.0
#define CO2_LIMIT 900.0

static int write_report(const char *output_path, const SensorStats *stats) {
    FILE *output = fopen(output_path, "w");

    if (output == NULL) {
        perror("No se pudo crear el archivo de reporte");
        return 1;
    }

    fprintf(output, "Reporte de monitoreo IoT\n");
    fprintf(output, "========================\n");
    fprintf(output, "Muestras procesadas: %zu\n", stats->total_samples);
    fprintf(output, "Temperatura promedio: %.2f C\n", stats->average_temperature);
    fprintf(output, "Humedad promedio: %.2f %%\n", stats->average_humidity);
    fprintf(output, "CO2 promedio: %.2f ppm\n", stats->average_co2);
    fprintf(output, "Temperatura maxima: %.2f C\n", stats->max_temperature);
    fprintf(output, "Alertas por temperatura (> %.1f C): %u\n",
            TEMPERATURE_LIMIT,
            stats->temperature_alerts);
    fprintf(output, "Alertas por CO2 (> %.1f ppm): %u\n",
            CO2_LIMIT,
            stats->co2_alerts);

    fclose(output);
    return 0;
}

int process_sensor_file(const char *input_path, const char *output_path, SensorStats *stats) {
    FILE *input = fopen(input_path, "r");
    char line[LINE_BUFFER_SIZE];
    double temperature_sum = 0.0;
    double humidity_sum = 0.0;
    double co2_sum = 0.0;
    int is_first_sample = 1;

    if (input == NULL) {
        perror("No se pudo abrir el archivo de datos");
        return 1;
    }

    memset(stats, 0, sizeof(*stats));

    if (fgets(line, sizeof(line), input) == NULL) {
        fclose(input);
        fprintf(stderr, "El archivo no contiene encabezado ni datos.\n");
        return 1;
    }

    while (fgets(line, sizeof(line), input) != NULL) {
        char timestamp[64];
        double temperature = 0.0;
        double humidity = 0.0;
        double co2 = 0.0;
        int fields = sscanf(line, "%63[^,],%lf,%lf,%lf", timestamp, &temperature, &humidity, &co2);

        if (fields != 4) {
            fprintf(stderr, "Linea ignorada por formato invalido: %s", line);
            continue;
        }

        temperature_sum += temperature;
        humidity_sum += humidity;
        co2_sum += co2;
        stats->total_samples++;

        if (is_first_sample || temperature > stats->max_temperature) {
            stats->max_temperature = temperature;
            is_first_sample = 0;
        }

        if (temperature > TEMPERATURE_LIMIT) {
            stats->temperature_alerts++;
        }

        if (co2 > CO2_LIMIT) {
            stats->co2_alerts++;
        }
    }

    fclose(input);

    if (stats->total_samples == 0U) {
        fprintf(stderr, "No se encontraron muestras validas en el archivo.\n");
        return 1;
    }

    stats->average_temperature = temperature_sum / (double)stats->total_samples;
    stats->average_humidity = humidity_sum / (double)stats->total_samples;
    stats->average_co2 = co2_sum / (double)stats->total_samples;

    return write_report(output_path, stats);
}
