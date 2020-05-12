/* Name, hw11.c, CS 24000, Spring 2020
 * Last updated April 04, 2020
 */

/* Add any includes here */

#include "hw11.h"

#include <malloc.h>
#include <assert.h>

/*
 * This function appends a new hash_value
 * to the given hash_list
 */

void append(hash_list_t **hash_list_head, int hash_value) {
  hash_list_t *hash_ptr = *hash_list_head;
  if (!hash_ptr) {
    *hash_list_head = malloc(sizeof(hash_list_t));
    assert(*hash_list_head);
    (*hash_list_head)->next_hash_ptr = NULL;
    (*hash_list_head)->hash_value = hash_value;
    return;
  }
  while (hash_ptr->next_hash_ptr) {
    hash_ptr = hash_ptr->next_hash_ptr;
  }
  hash_list_t *new_hash = malloc(sizeof(hash_list_t));
  assert(new_hash);
  new_hash->next_hash_ptr = NULL;
  new_hash->hash_value = hash_value;
  hash_ptr->next_hash_ptr = new_hash;
} /* append() */


/*
 * This function forms a prefix hash list
 */

void form_prefix_list(tree_node_t *node, hash_list_t **hash_list_head) {
  if (!node) {
    return;
  }
  append(hash_list_head, node->hash_value);
  form_prefix_list(node->left_child_ptr, hash_list_head);
  form_prefix_list(node->right_child_ptr, hash_list_head);
} /* form_prefix_list() */

/*
 * This function forms a postfix hash list
 */

void form_postfix_list(tree_node_t *node, hash_list_t **hash_list_head) {
  if (!node) {
    return;
  }
  form_postfix_list(node->left_child_ptr, hash_list_head);
  form_postfix_list(node->right_child_ptr, hash_list_head);
  append(hash_list_head, node->hash_value);
} /* form_postfix_list() */

/*
 * This function forms a forward hash list
 */

void form_forward_list(tree_node_t *node, hash_list_t **hash_list_head) {
  if (!node) {
    return;
  }
  form_forward_list(node->left_child_ptr, hash_list_head);
  append(hash_list_head, node->hash_value);
  form_forward_list(node->right_child_ptr, hash_list_head);
} /* form_forward_list() */

/*
 * This function forms a reverse hash list
 */

void form_reverse_list(tree_node_t *node, hash_list_t **hash_list_head) {
  if (!node) {
    return;
  }
  form_reverse_list(node->right_child_ptr, hash_list_head);
  append(hash_list_head, node->hash_value);
  form_reverse_list(node->left_child_ptr, hash_list_head);
} /* form_reverse_list() */


/*
 * This function frees a given node
 */

void free_node(tree_node_t *node) {
  node->left_child_ptr = NULL;
  node->right_child_ptr = NULL;
  free(node);
  node = NULL;
} /* free_node() */


/*
 * This function creates a new node
 */

tree_node_t *create_node(int hash_value) {
  assert(hash_value > 0);
  tree_node_t *new_node = NULL;
  new_node = malloc(sizeof(tree_node_t));
  assert(new_node);
  new_node->left_child_ptr = NULL;
  new_node->right_child_ptr = NULL;
  new_node->hash_value = hash_value;
  return new_node;
} /* create_node() */

/*
 * This function searches for a node
 */

tree_node_t *search_node(tree_node_t *root, int hash_value) {
  assert(hash_value > 0);
  if (!root) {
    return NULL;
  }
  if (root->hash_value == hash_value) {
    return root;
  }

  if (root->left_child_ptr) {
    tree_node_t *left_search = search_node(root->left_child_ptr, hash_value);
    if (left_search) {
      return left_search;
    }
  }

  if (root->right_child_ptr) {
    tree_node_t *right_search = search_node(root->right_child_ptr, hash_value);
    if (right_search) {
      return right_search;
    }
  }
  return NULL;
} /* search_node() */

/*
 * This function deletes the tree
 */

void delete_tree(tree_node_t *root) {
  if (!root) {
    return;
  }
  delete_tree(root->left_child_ptr);
  delete_tree(root->right_child_ptr);
  free_node(root);
} /* delete_tree() */

/*
 * This function inserts a new ndoe
 */

void insert_node(tree_node_t **parent, tree_node_t *new_node, int child_type) {
  assert(parent);
  assert(new_node);
  assert((child_type == 1) || (child_type == 0));

  if (*parent == NULL) {
    *parent = new_node;
    return;
  }

  if (child_type == 0) {
    if ((*parent)->left_child_ptr) {
      delete_tree((*parent)->left_child_ptr);
    }
    (*parent)->left_child_ptr = new_node;
    return;
  }

  if ((*parent)->right_child_ptr) {
    delete_tree((*parent)->right_child_ptr);
  }
  (*parent)->right_child_ptr = new_node;
} /* insert_node() */

/*
 * This function forms a prefix hash list
 */

hash_list_t *get_hash_list_prefix(tree_node_t *root) {
  assert(root);
  hash_list_t *list_head = NULL;
  form_prefix_list(root, &list_head);
  return list_head;
} /* get_hash_list_prefix() */

/*
 * This function forms a postfix hash list
 */

hash_list_t *get_hash_list_postfix(tree_node_t *root) {
  assert(root);
  hash_list_t *list_head = NULL;
  form_postfix_list(root, &list_head);
  return list_head;
} /* get_hash_list_postfix() */

/*
 * This function forms a forward hash list
 */

hash_list_t *get_hash_list_forward(tree_node_t *root) {
  assert(root);
  hash_list_t *list_head = NULL;
  form_forward_list(root, &list_head);
  return list_head;
} /* get_hash_list_forward() */

/*
 * This function forms a reverse hash list
 */

hash_list_t *get_hash_list_reverse(tree_node_t *root) {
  assert(root);
  hash_list_t *list_head = NULL;
  form_reverse_list(root, &list_head);
  return list_head;
} /* get_hash_list_reverse() */

/*
 * This function deletes the hash_list
 */

void delete_hash_list(hash_list_t *hash_head) {
  while (hash_head) {
    hash_list_t *next = hash_head->next_hash_ptr;
    hash_head->next_hash_ptr = NULL;
    free(hash_head);
    hash_head = NULL;
    hash_head = next;
  }
} /* delete_hash_list() */

/* Remember, you don't need a main function!
 * it is provided by hw11_main.c or hw11_test.o
 */
