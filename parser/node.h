
#ifndef NODE_H
#define NODE_H

enum node_type_e {
  NODE_COMMAND,
  NODE_VAR,
};
typedef enum node_type_e node_type_e;

enum val_type_e {
  VAL_SINT = 1,
  VAL_UINT,
  VAL_SLLONG,
  VAL_ULLONG,
  VAL_FLOAT,
  VAL_LDOUBLE,
  VAL_CHR,
  VAL_STR,
};
typedef enum val_type_e val_type_e;

union symval_u {
  long sint;
  unsigned long uint;
  long long sllong;
  unsigned long long ullong;
  double sfloat;
  long double ldouble;
  char chr;
  char *str;
};
typedef union symval_u symval_u;

struct node_s {
  enum node_type_e type;
  enum val_type_e val_type;
  union symval_u val;
  int children;
  struct node_s *first_child;
  struct node_s *next_sibling, *prev_sibling;
};
typedef struct node_s node_s;

node_s *create_node(node_type_e type);
void add_child (node_s *parent, node_s *child);
void free_node_tree(node_s *root); 
void set_node_val_str(node_s *node, char *str);

#endif
