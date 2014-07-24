#ifndef AST_H
#define AST_H
#include <stdbool.h>
#include <stddef.h>

//#define RDEBUG

#ifdef RDEBUG
#define dbg_info printf
#else
#define dbg_info
#endif

enum ff_node_type {
  node_op = 1,
  node_id,
  node_list,
  node_bool
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
  bool is_empty;
  enum ff_node_type type;

  union {
    enum ff_op_type op_type;
    char *id_name;
    struct FfNode *list;
    bool is_true;
  } node_val;

  struct FfNode* next;

} FfNode;

#define node_val_id         node_val.id_name
#define node_val_op         node_val.op_type
#define node_val_list       node_val.list
#define node_val_bool       node_val.is_true

FfNode* ff_create_op_node(enum ff_op_type type);
FfNode* ff_create_id_node(char *id_name);
FfNode* ff_create_list_node(FfNode *list);
FfNode* ff_create_empty_node();
FfNode* ff_link_node(FfNode *left, FfNode *right);
FfNode* ff_copy_node(FfNode *src);
FfNode* ff_create_bool_node(bool is_true);

#endif
