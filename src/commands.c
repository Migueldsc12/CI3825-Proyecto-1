#include "include/commands.h"
#include "include/node.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Inicializa el sistema de archivos con la raíz "/"
FileSystem *init_filesystem()
{
    FileSystem *fs = (FileSystem *)malloc(sizeof(FileSystem));
    if (!fs)
    {
        perror("Error al asignar memoria para el sistema de archivos");
        return NULL;
    }

    fs->root = create_node("/", DIR_TYPE, NULL);
    fs->current_dir = fs->root;

    if (!fs->root)
    {
        free(fs);
        return NULL;
    }

    return fs;
}

// Crea un archivo en la ruta especificada
bool touch(FileSystem *fs, const char *path)
{
    if (!fs || !path)
        return false;

    // Verifica si el archivo ya existe
    Node *existing_node = find_node(fs->current_dir, path, FILE_TYPE);
    if (existing_node)
    {
        fprintf(stderr, "Error: El archivo '%s' ya existe.\n", path);
        return false;
    }

    // Crea el archivo
    Node *new_file = create_node(path, FILE_TYPE, fs->current_dir);
    if (!new_file)
        return false;

    add_child(fs->current_dir, new_file);
    return true;
}

// Crea un directorio en la ruta especificada
bool mkdir(FileSystem *fs, const char *path)
{
    if (!fs || !path)
        return false;

    // Verifica si el directorio ya existe
    Node *existing_node = find_node(fs->current_dir, path, DIR_TYPE);
    if (existing_node)
    {
        fprintf(stderr, "Error: El directorio '%s' ya existe.\n", path);
        return false;
    }

    // Crea el directorio
    Node *new_dir = create_node(path, DIR_TYPE, fs->current_dir);
    if (!new_dir)
        return false;

    add_child(fs->current_dir, new_dir);
    return true;
}

// Elimina un archivo en la ruta especificada
bool rm(FileSystem *fs, const char *path)
{
    if (!fs || !path)
        return false;

    // Busca el archivo en el árbol
    Node *file_to_remove = find_node(fs->current_dir, path, FILE_TYPE);

    if (!file_to_remove || get_node_type(file_to_remove) != FILE_TYPE)
    {
        fprintf(stderr, "Error: El archivo no existe o es un directorio.\n");
        return false;
    }

    remove_node(file_to_remove);
    return true;
}

// Elimina un directorio vacío en la ruta especificada
bool rmdir(FileSystem *fs, const char *path)
{
    if (!fs || !path)
        return false;

    // Busca el directorio en el árbol
    Node *dir_to_remove = find_node(fs->current_dir, path, DIR_TYPE);

    if (!dir_to_remove || get_node_type(dir_to_remove) != DIR_TYPE)
    {
        fprintf(stderr, "Error: El directorio no existe o no está vacío.\n");
        return false;
    }

    // Verifica que el directorio esté vacío
    if (get_first_child(dir_to_remove))
    {
        fprintf(stderr, "Error: El directorio no está vacío.\n");
        return false;
    }

    remove_node(dir_to_remove);
    return true;
}

// Lista los archivos y directorios en la ruta especificada
void ls(const FileSystem *fs, const char *path, bool long_listing)
{
    if (!fs)
        return;

    Node *target_dir = fs->current_dir;

    // Si se especifica un path, buscar el directorio correspondiente
    if (path)
    {
        target_dir = find_node(fs->current_dir, path, DIR_TYPE);
        if (!target_dir)
        {
            fprintf(stderr, "Error: El directorio '%s' no existe.\n", path);
            return;
        }
    }

    Node *child = get_first_child(target_dir);
    while (child)
    {
        if (long_listing)
        {
            printf("%s (%s)\n", get_node_name(child),
                   get_node_type(child) == DIR_TYPE ? "DIR" : "FILE");
        }
        else
        {
            printf("%s\n", get_node_name(child));
        }
        child = get_next_sibling(child);
    }
}

// Cambia el directorio actual
bool cd(FileSystem *fs, const char *path)
{
    if (!fs || !path)
        return false;

        // Caso especial: "cd .." para retroceder al directorio padre
    if (strcmp(path, "..") == 0) 
    {
        Node *parent = get_parent(fs->current_dir);
        if (!parent) {
            fprintf(stderr, "Error: No hay directorio padre (ya estás en la raíz).\n");
            return false;
        }
        fs->current_dir = parent;
        return true;
    }

    // Busca el directorio
    Node *target_dir = find_node(fs->current_dir, path, DIR_TYPE);

    if (!target_dir || get_node_type(target_dir) != DIR_TYPE)
    {
        fprintf(stderr, "Error: El directorio no existe.\n");
        return false;
    }

    fs->current_dir = target_dir;
    return true;
}

// Imprime la ruta absoluta del directorio actual
void pwd(const FileSystem *fs) {
    if (!fs) return;

    // Construye la ruta absoluta recorriendo los padres
    char path[1024] = "";
    Node *current = fs->current_dir;

    while (current)
    {
        char temp[1024];

        if (get_parent(current) == NULL)
        {
            // Si es la raíz, solo agregamos "/"
            snprintf(temp, sizeof(temp), "/%s", path);
        } 
        else if (get_parent(get_parent(current)) == NULL) 
        {
            // Si es un hijo directo de la raíz, no agregamos "/" adicional
            snprintf(temp, sizeof(temp), "%s%s", get_node_name(current), path);
        } 
        else 
        {
            // Si no es la raíz ni un hijo directo de la raíz, agregamos "/<nombre>"
            snprintf(temp, sizeof(temp), "/%s%s", get_node_name(current), path);
        }
        strcpy(path, temp);
        current = get_parent(current);
    }

    printf("%s\n", path);
}

// Guarda el sistema de archivos en un archivo de texto
bool wrts(const FileSystem *fs, const char *output_file)
{
    if (!fs || !output_file)
        return false;

    FILE *file = fopen(output_file, "w");
    if (!file)
    {
        perror("Error al abrir el archivo");
        return false;
    }

    // Se recorre el árbol en preorden, iniciando en la raíz.
    write_preorder(file, fs->root, "");

    fclose(file);
    return true;
}

// Muestra una lista de comandos disponibles
void help()
{
    printf("Comandos disponibles:\n");
    printf("  touch <nombre_archivo> - Crea un archivo.\n");
    printf("  mkdir <nombre_directorio> - Crea un directorio.\n");
    printf("  rm <nombre_archivo> - Elimina un archivo.\n");
    printf("  rmdir <nombre_directorio> - Elimina un directorio vacío.\n");
    printf("  ls [<nombre_directorio>] - Lista archivos y directorios.\n");
    printf("  cd <nombre_directorio> - Cambia el directorio actual.\n");
    printf("  pwd - Muestra la ruta absoluta del directorio actual.\n");
    printf("  wrts <nombre_archivo> - Guarda el sistema de archivos en un archivo.\n");
    printf("  help - Muestra esta ayuda.\n");
    printf("  exit - Termina el programa.\n");
}

// Libera la memoria del sistema de archivos
void exit_filesystem(FileSystem *fs)
{
    if (!fs)
        return;

    free_tree(fs->root);
    free(fs);
}