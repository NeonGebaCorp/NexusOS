#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

#define DIRECTORY "uniteriver"
#define MAX_FILENAME_LENGTH 256

int is_driver_loaded(const char *driver_name) {
    // Implement logic to check if the driver is loaded
    // Return 1 if loaded, 0 otherwise
    return 0;
}

int load_driver(const char *driver_path, const char *arch) {
    // Implement logic to load and run the driver for the specified architecture
    // Return 1 if successful, 0 otherwise
    printf("Driver found [%s] starting...\n", driver_path);
    // Simulate loading and running the driver
    sleep(1); // Simulate driver loading time
    return 0; // Simulate failure to load
}

void handle_drivers(const char *directory) {
    struct dirent *entry;
    DIR *dir = opendir(directory);
    if (dir == NULL) {
        perror("Error opening directory");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            char *filename = entry->d_name;
            if (strstr(filename, ".usrkrnl") != NULL || strstr(filename, ".syskrnl") != NULL || strstr(filename, ".osrnl") != NULL) {
                char driver_path[MAX_FILENAME_LENGTH];
                snprintf(driver_path, MAX_FILENAME_LENGTH, "%s/%s", directory, filename);
                if (!is_driver_loaded(filename)) {
                    char arch[10];
                    // Determine architecture based on filename or other method
                    // Assuming here that we use 'i386' or 'x86_64' as the architecture
                    strcpy(arch, "x86_64");
                    if (load_driver(driver_path, arch)) {
                        printf("[ OK ]\n");
                    } else {
                        printf("Error: Unable to load driver %s. Halting the CPU.\n", filename);
                        // Halt the CPU until reboot
                        while (1) {
                            sleep(1); // CPU halt
                        }
                    }
                }
            }
        }
    }

    closedir(dir);
}

int main() {
    handle_drivers(DIRECTORY);
    printf("All drivers loaded successfully.\n");
    return 0;
}
