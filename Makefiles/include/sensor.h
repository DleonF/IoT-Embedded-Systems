#ifndef SENSOR_H
#define SENSOR_H

#include <stddef.h>

typedef struct {
    size_t total_samples;
    double average_temperature;
    double average_humidity;
    double average_co2;
    double max_temperature;
    unsigned int temperature_alerts;
    unsigned int co2_alerts;
} SensorStats;

int process_sensor_file(const char *input_path, const char *output_path, SensorStats *stats);

#endif
