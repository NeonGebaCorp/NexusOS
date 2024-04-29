#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define DIRECTORY "uniteriver"
#define MAX_FILENAME_LENGTH 256
#define DRIVER_EXTENSIONS ".usrkrnl .syskrnl .osrnl"

typedef struct {
    const char *path;
    pid_t pid;
    int loaded;
} driver_t;

int is_driver_loaded(const char *driver_name) {
    // Implement logic to check if the driver is loaded
    // Return 1 if loaded, 0 otherwise
    return 0;
}

void load_driver_internal(driver_t *driver) {
    if (driver->loaded) {
        printf("Driver %s is already loaded.\n", driver->path);
        return;
    }

    printf("Driver found [%s] starting...\n", driver->path);
    driver->pid = fork();

    if (driver->pid == 0) {
        execl(driver->path, driver->path, NULL);
        perror("Error executing driver");
        exit(1);
    } else if (driver->pid > 0) {
        driver->loaded = 1;
        printf("[ OK ]\n");
    } else {
        perror("Error forking process");
    }
}

void unload_driver_internal(driver_t *driver) {
    if (!driver->loaded) {
        printf("Driver %s is not loaded.\n", driver->path);
        return;
    }

    printf("Stopping driver [%s]...\n", driver->path);
    kill(driver->pid, SIGTERM);
    driver->loaded = 0;
    printf("[ OK ]\n");
}

void handle_drivers(const char *directory) {
    struct dirent *entry;
    DIR *dir = opendir(directory);
    if (dir == NULL) {
        perror("Error opening directory");
        return;
    }

    driver_t drivers[1024];
    int num_drivers = 0;

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            char *filename = entry->d_name;
            if (strstr(filename, DRIVER_EXTENSIONS) != NULL) {
                char driver_path[MAX_FILENAME_LENGTH];
                snprintf(driver_path, MAX_FILENAME_LENGTH, "%s/%s", directory, filename);
                drivers[num_drivers].path = driver_path;
                drivers[num_drivers].pid = 0;
                drivers[num_drivers].loaded = is_driver_loaded(filename);
                num_drivers++;
            }
        }
    }

    for (int i = 0; i < num_drivers; i++) {
        if (!drivers[i].loaded) {
            load_driver_internal(&drivers[i]);
        }
    }

    for (int i = 0; i < num_drivers; i++) {
        if (drivers[i].loaded) {
            unload_driver_internal(&drivers[i]);
        }
    }

    closedir(dir);
}

int main() {
    handle_drivers(DIRECTORY);
    printf("All drivers handled successfully.\n");
    return 0;
}
