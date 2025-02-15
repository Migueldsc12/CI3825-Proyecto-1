#ifndef COMMANDS_H
#define COMMANDS_H

#include "node.h"
#include <stdbool.h>

// Estructura para representar el estado del sistema de archivos
typedef struct {
    Node *root;          // Nodo raíz del sistema de archivos
    Node *current_dir;   // Directorio actual
} FileSystem;

// Inicializa el sistema de archivos
FileSystem* init_filesystem();

// Comandos
bool touch(FileSystem *fs, const char *path);
bool mkdir(FileSystem *fs, const char *path);
bool rm(FileSystem *fs, const char *path);
bool rmdir(FileSystem *fs, const char *path);
void ls(const FileSystem *fs, const char *path, bool long_listing);
bool cd(FileSystem *fs, const char *path);
void pwd(const FileSystem *fs);
bool wrts(const FileSystem *fs, const char *output_file);
void help();
void exit_filesystem(FileSystem *fs);

#endif