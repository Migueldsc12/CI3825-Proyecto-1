#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/node.h"

// Definición de la estructura de nodo (estructura opaca)
struct nodeStruct
{
    char *name;
    NodeType type;
    Node *parent;
    Node *child;
    Node *sibling;
};

Node *create_node(const char *name, NodeType type, Node *parent)
{
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (!new_node)
    {
        perror("Error al asignar memoria para el nodo");
        return NULL;
    }

    new_node->name = strdup(name); // Copia el nombre del archivo/directorio
    if (!new_node->name)
    {
        perror("Error al asignar memoria para el nombre del nodo");
        free(new_node);
        return NULL;
    }

    new_node->type = type;
    new_node->parent = parent;
    new_node->child = NULL;
    new_node->sibling = NULL;

    return new_node;
}

// Busca un nodo por su nombre y tipo en el árbol
Node *find_node(Node *root, const char *name, NodeType type)
{
    if (!root || !name)
        return NULL;

    // Verifica si el nodo actual coincide con el nombre y tipo buscado
    if (strcmp(root->name, name) == 0 && root->type == type)
    {
        return root;
    }

    // Busca en los hijos del nodo actual
    Node *found = find_node(root->child, name, type);
    if (found)
        return found;

    // Busca en los hermanos del nodo actual
    return find_node(root->sibling, name, type);
}

void add_child(Node *parent, Node *child)
{
    if (!parent || !child)
        return;

    child->parent = parent;

    if (!parent->child)
    {
        parent->child = child; // Primer hijo
    }
    else
    {
        Node *sibling = parent->child;
        while (sibling->sibling)
        {
            sibling = sibling->sibling; // Encuentra el último hermano
        }
        sibling->sibling = child;
    }
}

void remove_node(Node *node)
{
    if (!node)
        return;

    // Si el nodo tiene padre, reorganiza los hijos
    if (node->parent)
    {
        Node *sibling = node->parent->child;
        if (sibling == node)
        {
            node->parent->child = node->sibling; // Elimina la referencia del padre
        }
        else
        {
            while (sibling && sibling->sibling != node)
            {
                sibling = sibling->sibling;
            }
            if (sibling)
            {
                sibling->sibling = node->sibling;
            }
        }
    }

    // Liberar memoria
    free(node->name);
    free(node);
}

// Función para liberar todo el árbol de nodos recursivamente
void free_tree(Node *root)
{
    if (!root)
        return;

    // Liberar todos los hijos recursivamente
    free_tree(root->child);

    // Liberar todos los hermanos recursivamente
    free_tree(root->sibling);

    // Liberar el nodo actual
    free(root->name);
    free(root);
}

const char *get_node_name(const Node *node)
{
    return node ? node->name : NULL;
}

NodeType get_node_type(const Node *node)
{
    return node ? node->type : FILE_TYPE;
}

Node *get_parent(const Node *node)
{
    return node ? node->parent : NULL;
}

Node *get_first_child(const Node *node)
{
    return node ? node->child : NULL;
}

Node *get_next_sibling(const Node *node)
{
    return node ? node->sibling : NULL;
}

// Función auxiliar que busca entre los hijos inmediatos de 'parent'
Node *find_immediate_child(Node *parent, const char *name)
{
    if (!parent)
        return NULL;
    Node *child = get_first_child(parent);
    while (child)
    {
        if (strcmp(get_node_name(child), name) == 0)
        {
            return child;
        }
        child = get_next_sibling(child);
    }
    return NULL;
}

// Función auxiliar que recorre el árbol en preorden y escribe cada nodo.
// parent_path: camino absoluto del nodo padre. Para la raíz se pasa cadena vacía.
void write_preorder(FILE *file, const Node *node, const char *parent_path)
{
    if (!node)
        return;

    char abs_path[1024];
    // Si el nodo es la raíz (no tiene padre), su camino absoluto es "/"
    if (node->parent == NULL)
    {
        snprintf(abs_path, sizeof(abs_path), "/");
    }
    else
    {
        // Si el padre es la raíz, concatenamos "/<nombre>"
        if (strcmp(parent_path, "/") == 0)
            snprintf(abs_path, sizeof(abs_path), "/%s", node->name);
        else
            snprintf(abs_path, sizeof(abs_path), "%s/%s", parent_path, node->name);
    }

    // Fecha de creación ficticia hay que agregar a la estructura del nodo, pero esto es para pruebas
    const char *creation_date = "00:00-00/00/0000";
    // Tipo: 'D' para directorio, 'F' para archivo.
    char type_letter = (node->type == DIR_TYPE) ? 'D' : 'F';

    // Se escribe en el archivo con campos separados por tabuladores:
    // nombre, fecha de creación, tipo y camino absoluto.
    fprintf(file, "%s\t%s\t%c\t%s\n", node->name, creation_date, type_letter, abs_path);

    // Recorrido preorden: primero el hijo, luego el siguiente hermano.
    write_preorder(file, node->child, abs_path);
    write_preorder(file, node->sibling, parent_path);
}

// Función para imprimir la estructura del árbol (para depuración) esto se puede borrar luego
void print_tree(const Node *root, int depth)
{
    if (!root)
        return;

    // Imprimir sangría según la profundidad
    for (int i = 0; i < depth; i++)
    {
        printf("  ");
    }

    printf("%s (%s)\n", root->name, root->type == DIR_TYPE ? "DIR" : "FILE");

    // Recursión para los hijos y hermanos
    print_tree(root->child, depth + 1);
    print_tree(root->sibling, depth);
}
