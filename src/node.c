#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "node.h"

// Definición de la estructura de nodo (estructura opaca)
struct nodeStruct {
    char *name;
    NodeType type;
    Node *parent;
    Node *child;
    Node *sibling;
};

Node* create_node(const char *name, NodeType type, Node *parent) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (!new_node) {
        perror("Error al asignar memoria para el nodo");
        return NULL;
    }

    new_node->name = strdup(name);  // Copia el nombre del archivo/directorio
    if (!new_node->name) {
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

void add_child(Node *parent, Node *child) {
    if (!parent || !child) return;

    child->parent = parent;

    if (!parent->child) {
        parent->child = child;  // Primer hijo
    } else {
        Node *sibling = parent->child;
        while (sibling->sibling) {
            sibling = sibling->sibling;  // Encuentra el último hermano
        }
        sibling->sibling = child;
    }
}

void remove_node(Node *node) {
    if (!node) return;

    // Si el nodo tiene padre, reorganiza los hijos
    if (node->parent) {
        Node *sibling = node->parent->child;
        if (sibling == node) {
            node->parent->child = node->sibling;  // Elimina la referencia del padre
        } else {
            while (sibling && sibling->sibling != node) {
                sibling = sibling->sibling;
            }
            if (sibling) {
                sibling->sibling = node->sibling;
            }
        }
    }

    // Liberar memoria
    free(node->name);
    free(node);
}

// Función para liberar todo el árbol de nodos recursivamente
void free_tree(Node *root) {
    if (!root) return;

    // Liberar todos los hijos recursivamente
    free_tree(root->child);

    // Liberar todos los hermanos recursivamente
    free_tree(root->sibling);

    // Liberar el nodo actual
    free(root->name);
    free(root);
}

const char* get_node_name(const Node *node) {
    return node ? node->name : NULL;
}

NodeType get_node_type(const Node *node) {
    return node ? node->type : FILE_TYPE;
}

Node* get_parent(const Node *node) {
    return node ? node->parent : NULL;
}

Node* get_first_child(const Node *node) {
    return node ? node->child : NULL;
}

Node* get_next_sibling(const Node *node) {
    return node ? node->sibling : NULL;
}

// Función para imprimir la estructura del árbol (para depuración) esto se puede borrar luego
void print_tree(const Node *root, int depth) {
    if (!root) return;

    // Imprimir sangría según la profundidad
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }

    printf("%s (%s)\n", root->name, root->type == DIR_TYPE ? "DIR" : "FILE");

    // Recursión para los hijos y hermanos
    print_tree(root->child, depth + 1);
    print_tree(root->sibling, depth);
}
