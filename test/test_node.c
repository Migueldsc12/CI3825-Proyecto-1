#include "../src/include/node.h"
#include <assert.h>
#include <stdio.h>

// Prueba para create_node
void test_create_node() {
    Node *root = create_node("root", DIR_TYPE, NULL);
    assert(root != NULL); 
    assert(strcmp(get_node_name(root), "root") == 0); 
    assert(get_node_type(root) == DIR_TYPE); 
    assert(get_parent(root) == NULL);  

    free_tree(root); 
    printf("test_create_node: OK\n");
}

// Prueba para add_child
void test_add_child() {
    Node *parent = create_node("parent", DIR_TYPE, NULL);
    Node *child = create_node("child", FILE_TYPE, NULL);

    add_child(parent, child);
    assert(get_first_child(parent) == child);  
    assert(get_parent(child) == parent);  

    free_tree(parent);
    printf("test_add_child: OK\n");
}

// Prueba para remove_node
void test_remove_node() {
    Node *parent = create_node("parent", DIR_TYPE, NULL);
    Node *child = create_node("child", FILE_TYPE, NULL);

    add_child(parent, child);
    remove_node(child); 
    assert(get_first_child(parent) == NULL);  

    free_tree(parent);
    printf("test_remove_node: OK\n");
}

// Prueba para test_find_node 
void test_find_node() {
    // Crear un árbol de prueba
    Node *root = create_node("root", DIR_TYPE, NULL);
    Node *child1 = create_node("child1", DIR_TYPE, root);
    Node *child2 = create_node("child2", FILE_TYPE, root);
    Node *child3 = create_node("child3", DIR_TYPE, child1);

    add_child(root, child1);
    add_child(root, child2);
    add_child(child1, child3);

    // Pruebas
    assert(find_node(root, "child1", DIR_TYPE) == child1);
    assert(find_node(root, "child2", FILE_TYPE) == child2);
    assert(find_node(root, "child3", DIR_TYPE) == child3);
    assert(find_node(root, "nonexistent", DIR_TYPE) == NULL);

    free_tree(root);

    printf("test_find_node: OK\n");
}

int main() {
    test_create_node();
    test_add_child();
    test_remove_node();
    test_find_node();

    printf("Todas las pruebas pasaron.\n");
    return 0;
}

//Puedes probarlo con este comando: gcc -Wall -Wextra -g node.c ../test/test_node.c -o test_node