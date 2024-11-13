#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"
#include <sys/stat.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <libgen.h>

void listDir() {
    DIR *dir = opendir(".");
    struct dirent *entry;

    if (dir == NULL) {
        write(1, "Error! Unable to list directory\n", strlen("Error! Unable to list directory\n"));
        closedir(dir);
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        write(1, entry->d_name, strlen(entry->d_name));
        write(1, " ", strlen(" "));
    }

    write(1, "\n", strlen("\n"));
    closedir(dir);
}

void showCurrentDir() {
    char cwd[1024];
    
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        write(1, "Error! Unable to show current directory\n", strlen("Error! Unable to show current directory\n"));
    } else {
        write(1, cwd, strlen(cwd));
        write(1, "\n", strlen("\n"));
    }
}

void makeDir(char *dirName) {

    int success = mkdir(dirName, 0777);

    if (success == -1) {
        write(1, "Error! cannot make directory\n", strlen("Error! cannot make directory\n"));
    }
}

void changeDir(char *dirName) {
    int success = chdir(dirName);

    if (success == -1) {
        write(2, "Error! cannot change directory\n", strlen("Error! cannot change directory\n"));
    }
}

void copyFile(char *sourcePath, char *destinationPath) {
    int src;
    int dest;
    size_t bytes_read;
    char buffer[4096];
    struct stat dest_stat;
    char* fullPath = malloc(strlen(destinationPath) + strlen(sourcePath) + 2);

    src = open(sourcePath, O_RDONLY);
    if (src == -1) {
        write(1, "Error! Cannot copy \n", strlen("Error! Cannot copy \n"));
        free(fullPath);
        return;
    }

    if (stat(destinationPath, &dest_stat) == 0 && S_ISDIR(dest_stat.st_mode)) {
        strcpy(fullPath, destinationPath);
        strcat(fullPath, "/");
        strcat(fullPath, basename(sourcePath));
        fullPath[strlen(destinationPath) + strlen(sourcePath) + 1] = '\0';
        dest = open(fullPath, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    } else {
        dest = open(destinationPath, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    }

    if (dest == -1) {
        write(1, "Error! Cannot copy file\n", strlen("Error! Cannot copy file\n"));
        free(fullPath);
        return;
    }

    while((bytes_read = read(src, buffer, sizeof(buffer))) > 0) {
        write(dest, buffer, bytes_read);
    }

    free(fullPath);
    close(src);
    close(dest);
}

void moveFile(char *sourcePath, char *destinationPath) { 
    copyFile(sourcePath, destinationPath);
    deleteFile(sourcePath);
}

void deleteFile(char *filename) {
    int success = unlink(filename);

    if (success == -1) {
        write(1, "Error! Cannot delete file\n", strlen("Error! Cannot delete file\n"));
    }
}

void displayFile(char *filename) {
    int fd = open(filename, O_RDONLY);
    char buffer[4096];

    ssize_t bytes_read;

    while((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
        write(1, buffer, bytes_read);
    }
}
