#ifndef AST_H
#define AST_H

enum ff_node_type {
  nd_op = 1,
  nd_arg
};

enum ff_op_type {
  op_quote = 1,
  op_atom,
  op_eq,
  op_car,
  op_cdr,
  op_cons,
  op_cond
};

typedef struct FfNode {
  unsigned short nd_type;
  union {
    unsigned long op_type;
    char *arg_val;
  } nd_val;

  struct FfNode *leftmost;
  struct FfNode *next;
} FfNode;

#define nd_arg_val     nd_val.arg_val
#define nd_op_val       nd_val.op_type

FfNode* ff_create_op_node(enum ff_op_type op_type);
FfNode* ff_create_arg_node(char *arg_name);
FfNode* ff_link_sibling(FfNode *leftmost, FfNode *right);
FfNode* ff_link_child(FfNode *parent, FfNode *child);

#endif
