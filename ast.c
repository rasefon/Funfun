#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "st.h"

FfNode* ff_create_op_node(enum ff_op_type op_type)
{
  FfNode *node = malloc(sizeof(FfNode));
  node->nd_type = nd_op;
  node->nd_op_val = op_type;
  node->leftmost = node->next = NULL;
  return node;
}

FfNode* ff_create_arg_node(char *arg_name)
{
  FfNode *node = malloc(sizeof(FfNode));
  node->nd_type = nd_arg;
  node->nd_arg_val = arg_name;
  node->leftmost = node->next = NULL;
  return node;
}

FfNode* ff_link_sibling(FfNode *leftmost, FfNode *right)
{
  assert(leftmost != NULL && right != NULL);
  FfNode *left = leftmost;
  while (left->next) {
    left = left->next;
  }
  left->next = right;
  return leftmost;
}

FfNode* ff_link_child(FfNode *parent, FfNode *child)
{
  assert(parent != NULL && child != NULL);
  parent->leftmost = child;
  return parent;
}
