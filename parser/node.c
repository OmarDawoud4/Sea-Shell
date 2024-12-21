#include "node.h"
#include "../core/shell.h"
#include "parser.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

node_s *create_node(node_type_e type) {
  node_s *node = malloc(sizeof(node_s));
  if (!node) {
    return NULL;
  }
  memset(node, 0, sizeof(node_s));
  node->type = type;
  return node;
}

void add_child(node_s *parent, node_s *child) {
  if (!parent || !child)
  {
    return;
  }
  if (!parent->first_child) {
    parent->first_child = child;
  } else {
    node_s *last_sibling = parent->first_child;
    while (last_sibling->next_sibling) {
      last_sibling = last_sibling->next_sibling;
    }

    last_sibling->next_sibling = child;
    child->prev_sibling = last_sibling;
  }

  parent->children++;
}


void free_node_tree(node_s *node) {
  if (!node) {
    return;
  }
  node_s *child = node->first_child;
  while (child) {
    node_s *next = child->next_sibling;
    free_node_tree(child);
    child = next;
  }
  if (node->val_type == VAL_STR) {
    if (node->val.str) {
      free(node->val.str);
    }
  }
  free(node);
}

void set_node_val_str(node_s *node, char *str) {
  node->val_type = VAL_STR;

  if (!str) {

    node->val.str = NULL;
  } else {

    char *allocated_string = malloc(strlen(str) + 1);

    if (!allocated_string) {
      node->val.str = NULL;
    } else {

      strcpy(allocated_string, str);

      node->val.str = allocated_string;
    }
  }
}
