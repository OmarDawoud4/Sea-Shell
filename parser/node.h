#ifndef NODE_H
#define NODE_H

#include <stdlib.h>
#include <string.h>

typedef enum {
    NODE_COMMAND,
    NODE_VAR,
    NODE_REDIRECT
} node_type_e;

typedef enum {
    VAL_STR
} val_type_e;

typedef struct node_s {
    node_type_e type;
    char *redirect_type; // for redirection nodes
    char *filename;      
    val_type_e val_type; 
    union {
        char *str;
    } val;
    struct node_s *first_child;
    struct node_s *next_sibling;
    struct node_s *prev_sibling;
    int children; 
} node_s;

node_s *create_node(node_type_e type);
void add_child(node_s *parent, node_s *child);
void free_node_tree(node_s *node);
void set_node_val_str(node_s *node, char *str);

#endif 