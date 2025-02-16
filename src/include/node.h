#ifndef NODE_H
#define NODE_H

#include <stdbool.h>
#include <stdio.h>

// Definición opaca de la estructura nodeStruct
typedef struct nodeStruct Node;

// Enumeración para distinguir entre archivos y directorios
typedef enum {
    FILE_TYPE,
    DIR_TYPE
} NodeType;

Node* find_node(Node *root, const char *name, NodeType type);

// Funciones para manipulación de nodos
Node* create_node(const char *name, NodeType type, Node *parent);
void add_child(Node *parent, Node *child);
void remove_node(Node *node);
void free_tree(Node *root);

// Funciones para obtener información del nodo
const char* get_node_name(const Node *node);
NodeType get_node_type(const Node *node);
Node* get_parent(const Node *node);
Node* get_first_child(const Node *node);
Node* get_next_sibling(const Node *node);

// Función auxiliar que busca entre los hijos inmediatos de 'parent'
Node* find_immediate_child(Node *parent, const char *name);
// Función auxiliar que recorre el árbol en preorden y escribe cada nodo.
void write_preorder(FILE *file, const Node *node, const char *parent_path);
    
// Función para imprimir la estructura del árbol (para depuración) esto se puede borrar no lo he implementado
void print_tree(const Node *root, int depth);

#endif
