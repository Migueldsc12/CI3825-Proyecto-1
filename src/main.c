#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/commands.h"

#define MAX_CMD 1024

// Función para cargar el sistema de archivos a partir de un archivo de entrada.
// Se asume que cada línea tiene el formato: <camino> <espacio o tab> <tipo>
// Ejemplo de línea: /home/lear/a.pdf F
void load_filesystem_from_file(FileSystem *fs, const char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (!fp)
    {
        perror("Error al abrir el archivo de sistema de archivos");
        return;
    }

    char line[MAX_CMD];
    while (fgets(line, sizeof(line), fp))
    {
        // Eliminar el salto de línea
        line[strcspn(line, "\n")] = '\0';
        if (strlen(line) == 0)
            continue; // saltar líneas vacías

        // Extraer el camino y el tipo
        char *token = strtok(line, " \t");
        if (!token)
            continue;
        char path[1024];
        strcpy(path, token);

        token = strtok(NULL, " \t");
        if (!token)
            continue;
        char typeChar = token[0];
        NodeType type = (typeChar == 'D') ? DIR_TYPE : FILE_TYPE;

        // Procesar el camino: es absoluto por lo tanto comienza con '/'
        char *path_copy = strdup(path);
        if (!path_copy) 
            continue;

        // Procesar la ruta componente por componente
        Node *current = fs->root; // Comenzamos en la raíz
        char *saveptr;
        char *part = strtok_r(path_copy, "/", &saveptr);
        char *next_part = strtok_r(NULL, "/", &saveptr);

        while (part != NULL) 
        {
            if (next_part == NULL) 
            {
                // Última parte: se crea el nodo con el tipo especificado (archivo o directorio)
                if (!find_immediate_child(current, part)) 
                {
                    Node *new_node = create_node(part, type, current);
                    add_child(current, new_node);
                }
            } 
            else 
            {
                // Parte intermedia: debe ser directorio
                Node *next_dir = find_immediate_child(current, part);
                if (!next_dir) 
                {
                    next_dir = create_node(part, DIR_TYPE, current);
                    add_child(current, next_dir);
                }
                current = next_dir;
            }
            part = next_part;
            next_part = strtok_r(NULL, "/", &saveptr);
        }
        free(path_copy);
    }
    fclose(fp);
}

//Funcion principal del programa
int main(int argc, char *argv[])
{
    FileSystem *fs = init_filesystem();
    if (!fs)
    {
        fprintf(stderr, "Error al inicializar el sistema de archivos.\n");
        return 1;
    }

    // Si se pasó un argumento (archivo de sistema de archivos), cargarlo
    if (argc == 2)
    {
        load_filesystem_from_file(fs, argv[1]);
    }
    else if (argc > 2)
    {
        fprintf(stderr, "Uso: %s [archivo_con_unix_fs]\n", argv[0]);
        exit_filesystem(fs);
        return 1;
    }

    // Ubicar al usuario en la raíz
    fs->current_dir = fs->root;

    char input[MAX_CMD];
    printf("> ");
    while (fgets(input, sizeof(input), stdin))
    {
        // Eliminar el salto de línea
        input[strcspn(input, "\n")] = '\0';
        if (strlen(input) == 0)
        {
            printf("> ");
            continue;
        }

        // Tokenizar la entrada
        char *command = strtok(input, " ");
        if (!command)
        {
            printf("> ");
            continue;
        }

        // Despacho de comandos
        if (strcmp(command, "touch") == 0)
        {
            char *arg = strtok(NULL, " ");
            if (!arg)
            {
                printf("Uso: touch <nombre_archivo>\n");
            }
            else
            {
                if (!touch(fs, arg))
                {
                    // El error ya se imprime dentro de la función
                }
            }
        }
        else if (strcmp(command, "rm") == 0)
        {
            char *arg = strtok(NULL, " ");
            if (!arg)
            {
                printf("Uso: rm <nombre_archivo>\n");
            }
            else
            {
                rm(fs, arg);
            }
        }
        else if (strcmp(command, "mkdir") == 0)
        {
            char *arg = strtok(NULL, " ");
            if (!arg)
            {
                printf("Uso: mkdir <nombre_directorio>\n");
            }
            else
            {
                mkdir(fs, arg);
            }
        }
        else if (strcmp(command, "rmdir") == 0)
        {
            char *arg = strtok(NULL, " ");
            if (!arg)
            {
                printf("Uso: rmdir <nombre_directorio>\n");
            }
            else
            {
                rmdir(fs, arg);
            }
        }
        else if (strcmp(command, "ls") == 0)
        {
            char *arg = strtok(NULL, " ");
            bool long_listing = false;
            if (arg && strcmp(arg, "-l") == 0)
            {
                long_listing = true;
                arg = strtok(NULL, " ");
            }
            ls(fs, arg, long_listing);
        }
        else if (strcmp(command, "cd") == 0)
        {
            char *arg = strtok(NULL, " ");
            if (!arg)
            {
                printf("Uso: cd <nombre_directorio>\n");
            }
            else
            {
                cd(fs, arg);
            }
        }
        else if (strcmp(command, "pwd") == 0)
        {
            pwd(fs);
        }
        else if (strcmp(command, "wrts") == 0)
        {
            char *arg = strtok(NULL, " ");
            if (!arg)
            {
                printf("Uso: wrts <nombre_del_archivo>\n");
            }
            else
            {
                if (!wrts(fs, arg))
                {
                    printf("Error al escribir el sistema de archivos.\n");
                }
            }
        }
        else if (strcmp(command, "help") == 0)
        {
            help();
        }
        else if (strcmp(command, "exit") == 0)
        {
            break;
        }
        else
        {
            printf("Comando desconocido. Escriba 'help' para ver los comandos disponibles.\n");
        }

        printf("> ");
    }

    exit_filesystem(fs);
    return 0;
}
