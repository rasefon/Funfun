#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "st.h"

FfNode* ff_create_op_node(enum ff_op_type type)
{
  FfNode *node = malloc(sizeof(FfNode));
  node->type = node_op;
  node->node_val_op = type;
  node->is_empty = false;
  return node;
}

FfNode* ff_create_id_node(char *id_name)
{
  assert(!!id_name);

  FfNode *node = malloc(sizeof(FfNode));
  node->type = node_id;
  node->node_val_id = id_name;
  node->is_empty = false;
  return node;
}

FfNode* ff_create_list_node(FfNode *list)
{
  assert(!!list);

  FfNode *node = malloc(sizeof(FfNode));
  node->type = node_list;
  node->node_val_list = list;
  node->is_empty = false;
  return node;
}

FfNode* ff_create_empty_node()
{
  FfNode *node = malloc(sizeof(FfNode));
  node->is_empty = true;
  return node;
}

FfNode* ff_link_node(FfNode *left, FfNode *right)
{
  assert(!!left && !!right);

  FfNode *curr_tail = left;
  while(curr_tail->next) {
    curr_tail = curr_tail->next;
  }
  curr_tail->next = right;

  return left;
}
