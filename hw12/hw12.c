/* Name, hw3.c, CS 24000, Spring 2020
 * Last updated January 29, 2020
 */

/* Add any includes here */

#include "hw12.h"

#include <assert.h>
#include <string.h>
#include <malloc.h>
#include <malloc_debug.h>

/*
 * This function returns the
 * number of matching data found
 * in the tree
 */

int return_match_count(struct node *root, void *data, int match_count) {
  if (!root) {
    return match_count;
  }
  if (root->compare(root->data, data) == 1) {
    return return_match_count(root->left, data, match_count);
  }
  if (root->compare(root->data, data) == 0) {
    return return_match_count(root->left, data, match_count + 1);
  }
  if (root->compare(root->data, data) == -1) {
    return return_match_count(root->right, data, match_count);
  }
  return match_count;
} /* return_match_count() */

/*
 * This function fills the array with nodes
 * matching the given data
 */

void fill_matches(struct node *root, void *data,
  struct node **match_array, int *index, int max_index) {
  if (!root) {
    return;
  }
  if (*index == max_index) {
    return;
  }
  if (root->compare(root->data, data) == 1) {
    fill_matches(root->left, data, match_array, index, max_index);
  }
  if (root->compare(root->data, data) == 0) {
    match_array[*index] = root;
    *index += 1;
    fill_matches(root->left, data, match_array, index, max_index);
  }
  if (root->compare(root->data, data) == -1) {
    fill_matches(root->right, data, match_array, index, max_index);
  }
} /* fill_matches() */

/*
 * This function creates and mallocs
 * a new cpu data
 */

void create_cpu_data(void **cpu, const char *model,
    const char *manufacturer, int speed, int cores) {
  assert(cpu);
  assert(!(*cpu));
  assert(model);
  assert(manufacturer);

  cpu_t *new_cpu = malloc(sizeof(cpu_t));
  assert(new_cpu);
  new_cpu->model = malloc(strlen(model) + 1);
  assert(new_cpu->model);
  new_cpu->manufacturer = malloc(strlen(manufacturer) + 1);
  assert(new_cpu->manufacturer);

  strcpy(new_cpu->model, model);
  strcpy(new_cpu->manufacturer, manufacturer);
  new_cpu->model[strlen(new_cpu->model)] = '\0';
  new_cpu->manufacturer[strlen(new_cpu->manufacturer)] = '\0';
  new_cpu->speed = speed;
  new_cpu->cores = cores;
  (*cpu) = (void *) new_cpu;
} /* create_cpu_data() */

/*
 * This function deletes and frees
 * the given cpu data
 */

void delete_cpu_data(void **cpu) {
  assert(cpu);
  assert(*cpu);

  free(((cpu_t *) *cpu)->model);
  ((cpu_t *) *cpu)->model = NULL;
  free(((cpu_t *) *cpu)->manufacturer);
  ((cpu_t *) *cpu)->manufacturer = NULL;

  free((cpu_t *) *cpu);
  *cpu = NULL;
  cpu = NULL;
} /* delete_cpu_data() */

/*
 * This function compares the
 * given cpu data and returns
 * a value
 */

int compare_cpu_data(void *cpu_1, void *cpu_2) {
  assert(cpu_1);
  assert(cpu_2);
  int power_1 = (((cpu_t *) cpu_1)->cores) * (((cpu_t *) cpu_1)->speed);
  int power_2 = (((cpu_t *) cpu_2)->cores) * (((cpu_t *) cpu_2)->speed);
  if (power_1 > power_2) {
    return 1;
  }
  if (power_1 == power_2) {
    return 0;
  }
  return -1;
} /* compare_cpu_data() */

/*
 * This function creates and mallocs
 * a new memory data
 */

void create_memory_data(void **memory, const char *model,
  const char *manufacturer, int size, int speed, int ddr_gen) {
  assert(memory);
  assert(model);
  assert(manufacturer);
  assert(!(*memory));

  memory_t *new_memory = malloc(sizeof(memory_t));
  assert(new_memory);
  new_memory->model = malloc(strlen(model) + 1);
  assert(new_memory->model);
  new_memory->manufacturer = malloc(strlen(manufacturer) + 1);
  assert(new_memory->manufacturer);

  strcpy(new_memory->model, model);
  strcpy(new_memory->manufacturer, manufacturer);
  new_memory->model[strlen(new_memory->model)] = '\0';
  new_memory->manufacturer[strlen(new_memory->manufacturer)] = '\0';
  new_memory->speed = speed;
  new_memory->size = size;
  new_memory->ddr_gen = ddr_gen;

  (*memory) = (void *) new_memory;
} /* create_memory_data() */

/*
 * This function deletes and frees
 * the given memory data
 */

void delete_memory_data(void **memory) {
  assert(memory);
  assert(*memory);

  free(((memory_t *) *memory)->model);
  ((memory_t *) *memory)->model = NULL;
  free(((memory_t *) *memory)->manufacturer);
  ((memory_t *) *memory)->manufacturer = NULL;

  free((memory_t *) *memory);
  *memory = NULL;
  memory = NULL;
} /* delete_memory_data() */

/*
 * This function compares the
 * two given memory data and
 * return a value
 */

int compare_memory_data(void *memory_1, void *memory_2) {
  assert(memory_1);
  assert(memory_2);
  char *model_1 = ((memory_t *) memory_1)->model;
  char *model_2 =  ((memory_t *) memory_2)->model;
  if (strcmp(model_1, model_2) != 0) {
    if (strcmp(model_1, model_2) > 0) {
      return 1;
    }
    if (strcmp(model_1, model_2) < 0) {
      return -1;
    }
  }

  char *manufacturer_1 = ((memory_t *) memory_1)->manufacturer;
  char *manufacturer_2 = ((memory_t *) memory_2)->manufacturer;
  if (strcmp(manufacturer_1, manufacturer_2) != 0) {
    if (strcmp(manufacturer_1, manufacturer_2) > 0) {
      return 1;
    }
    if (strcmp(manufacturer_1, manufacturer_2) < 0) {
      return -1;
    }
  }
  return 0;
} /* compare_memory_data() */


/*
 * This function creates and mallocs
 * a new node
 */

void create_node(struct node **node_struct, void *data, void (*print)(void *) ,
    void (*delete)(void **) , int (*compare)(void *, void *) ) {

  assert(node_struct);
  assert(data);
  assert(print);
  assert(delete);
  assert(compare);
  assert(!(*node_struct));

  (*node_struct) = malloc(sizeof(struct node));
  assert(*node_struct);
  (*node_struct)->left = NULL;
  (*node_struct)->right = NULL;
  (*node_struct)->data = data;
  (*node_struct)->print = print;
  (*node_struct)->delete = delete;
  (*node_struct)->compare = compare;
} /* create_node() */

/*
 * This function deletes and
 * frees the memory of the given node
 */

void delete_node(struct node **node_struct) {
  assert(node_struct);
  assert(*node_struct);
  assert((*node_struct)->left == NULL);
  assert((*node_struct)->right == NULL);
  (*node_struct)->left = NULL;
  (*node_struct)->right = NULL;
  (*node_struct)->delete(&((*node_struct)->data));
  (*node_struct)->data = NULL;
  (*node_struct)->print = NULL;
  (*node_struct)->delete = NULL;
  (*node_struct)->compare = NULL;
  free((*node_struct));
  (*node_struct) = NULL;
  node_struct = NULL;
} /* delete_node() */

/*
 * This function adds the given node
 * in the tree
 */

void insert_node(struct node **root, struct node *node_to_add) {
  assert(root);
  assert(node_to_add);
  if (!(*root)) {
    *root = node_to_add;
    return;
  }

  if ((*root)->compare((*root)->data, node_to_add->data) >= 0) {
    if (!((*root)->left)) {
      (*root)->left = node_to_add;
      return;
    }
    insert_node(&((*root)->left), node_to_add);
    return;
  }
  if (!((*root)->right)) {
    (*root)->right = node_to_add;
    return;
  }
  insert_node(&((*root)->right), node_to_add);
} /* insert_node() */

/*
 * This function finds nodes with
 * matching data and returns an
 * array of matching pointers
 */

struct node **find_nodes(struct node *root, void *data, int *match) {
  assert(root);
  assert(data);
  assert(match);

  *match = return_match_count(root, data, 0);
  if (*match == 0) {
    return NULL;
  }
  struct node **match_array = NULL;
  match_array = malloc(sizeof(struct node *) * (*match));
  assert(match_array);
  int index = 0;
  int *index_ptr = &index;
  fill_matches(root, data, match_array, index_ptr, *match);
  return match_array;
} /* find_nodes() */

/*
 * This function removes the given
 * node from the tree
 */

void remove_node(struct node **root, struct node *to_remove) {
  assert(root);
  assert(to_remove);
  assert(*root);
  if (*root == to_remove) {
    if (to_remove->left) {
      struct node *new_head = to_remove->left;
      struct node *rightmost = new_head;
      while (rightmost->right) {
        rightmost = rightmost->right;
      }
      rightmost->right = to_remove->right;
      *root = new_head;
      to_remove->left = NULL;
      to_remove->right = NULL;
      delete_node(&to_remove);
      return;
    }
    *root = to_remove->right;
    to_remove->left = NULL;
    to_remove->right = NULL;
    delete_node(&to_remove);
    return;
  }

  if ((*root)->compare((*root)->data, to_remove->data) >= 0) {
    if ((*root)->left == to_remove) {
      (*root)->left = to_remove->left;
      struct node *rightmost_node = to_remove->left;
      if (rightmost_node) {
        while (rightmost_node->right) {
          rightmost_node = rightmost_node->right;
        }
        rightmost_node->right = to_remove->right;
      }
      else {
        (*root)->left = to_remove->right;
      }
      to_remove->left = NULL;
      to_remove->right = NULL;
      delete_node(&to_remove);
      return;
    }
    remove_node(&((*root)->left), to_remove);
    return;
  }
  if ((*root)->compare((*root)->data, to_remove->data) < 0) {
    if ((*root)->right == to_remove) {
      (*root)->right = to_remove->left;
      struct node *rightmost_node = to_remove->left;
      if (rightmost_node) {
        while (rightmost_node->right) {
          rightmost_node = rightmost_node->right;
        }
        rightmost_node->right = to_remove->right;
      }
      else {
        (*root)->right = to_remove->right;
      }
      to_remove->left = NULL;
      to_remove->right = NULL;
      delete_node(&to_remove);
      return;
    }
    remove_node(&((*root)->right), to_remove);
    return;
  }
} /* remove_node() */

/*
 * This function deletes and
 * frees the entire tree
 */

void delete_tree(struct node **root) {
  assert(root);
  if (!root) {
    return;
  }
  if (!(*root)) {
    return;
  }
  delete_tree(&(*root)->left);
  delete_tree(&(*root)->right);
  delete_node(root);
} /* delete_tree() */

/* Remember, you don't need a main function!
 * it is provided by hw3_main.c or hw3_test.o
 */
