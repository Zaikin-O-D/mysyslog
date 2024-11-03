#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include "libmysyslog.h"

#define CONFIG_PATH "/etc/mysyslog/mysyslog.cfg"

static volatile int keepRunning = 1;

void intHandler(int dummy) {
    keepRunning = 0;
}

void read_config(const char* path, int* level, int* driver, int* format, char* log_path) {
    FILE* file = fopen(path, "r");
    if (!file) {
        fprintf(stderr, "Could not open config file: %s\n", path);
        exit(EXIT_FAILURE);
    }

    fscanf(file, "level=%d\n", level);
    fscanf(file, "driver=%d\n", driver);
    fscanf(file, "format=%d\n", format);
    fscanf(file, "path=%s\n", log_path);
    fclose(file);
}

int main() {
    signal(SIGINT, intHandler);
    signal(SIGTERM, intHandler);

    int level, driver, format;
    char log_path[256];

    read_config(CONFIG_PATH, &level, &driver, &format, log_path);

    while (keepRunning) {
        mysyslog("Daemon log message", level, driver, format, log_path);
        sleep(5); // Log every 5 seconds
    }

    return 0;
}
