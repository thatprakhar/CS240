/* Name, hw8.c, CS 24000, Spring 2020
 * Last updated February 26, 2020
 */

/* Add any includes here */

#include "hw8.h"

#include <assert.h>
#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#define MAX_TEXT_LEN (500)

/*
 * This function create a
 * new operation
 */

operation_t* create_operation(char *text, int line) {
  operation_t *new_op = NULL;
  new_op = (operation_t *) malloc(sizeof(operation_t));
  assert(new_op != NULL);
  new_op->new_text = malloc(strlen(text) * sizeof(char) + 1);
  assert(new_op->new_text != NULL);
  strcpy(new_op->new_text, text);
  new_op->new_text[strlen(new_op->new_text)] = '\0';
  new_op->line_num = line;
  new_op->next_operation = NULL;
  return new_op;
} /* create_operation() */


/*
 * This function add new operation
 */

operation_t *add_new_operation(operation_t *head, char *text, int line) {
  assert(text != NULL);
  assert(line >= 0);
  operation_t *new_op = NULL;
  new_op = create_operation(text, line);
  if (head == NULL) {
    head = new_op;
    return new_op;
  }

  operation_t *ptr = head;
  while (ptr->next_operation != NULL) {
    ptr = ptr->next_operation;
  }

  ptr->next_operation = new_op;

  return new_op;
} /* add_new_operation() */

/*
 * This function returns the length
 * of the list
 */

int list_len(operation_t *head) {
  int counter = 0;
  operation_t *ptr = head;

  while (ptr != NULL) {
    ptr = ptr->next_operation;
    counter++;
  }
  ptr = NULL;
  return counter;
} /* list_len() */


/*
 * This function returns the
 * n-th operation
 */

operation_t *get_nth_operation(operation_t *head, int n) {
  assert(head != NULL);
  assert(n >= 0);

  int counter = 0;

  operation_t *ptr = head;

  while (ptr != NULL) {
    if (counter == n) {
      return ptr;
    }
    ptr = ptr->next_operation;
    counter++;
  }
  ptr = NULL;
  return NULL;
} /* get_nth_operation() */

/*
 * This function undoes the
 * n-th operation
 */

operation_t *undo_nth_operation(operation_t *head, int n) {
  assert(head != NULL);
  assert(n >= 0);
  if (head->next_operation == NULL) {
    if (n == 0) {
      return head;
    }
    return NULL;
  }
  operation_t *ptr = head;

  if (!n) {
    operation_t *prev = NULL;
    operation_t *curr = head;
    operation_t *next = NULL;

    while (curr != NULL) {
      next = curr->next_operation;
      curr->next_operation = prev;
      prev = curr;
      curr = next;
    }
    return prev;
  }

  int counter = 0;

  while (ptr != NULL) {
    if (counter == n - 1) {
      operation_t *temp = ptr;
      ptr = ptr->next_operation;
      temp->next_operation = NULL;
      // reverse the linked list from here :(
      operation_t *prev = NULL;
      operation_t *current = ptr;
      operation_t *next = NULL;

      while (current != NULL) {
        next = current->next_operation;
        current->next_operation = prev;
        prev = current;
        current = next;
      }

      return prev;

    }
    ptr = ptr->next_operation;
    counter++;
  }

  return NULL;
} /* undo_nth_operation() */

/*
 * This function redoes the
 * n-th operation
 */

void redo_n_operations(operation_t *list_1, operation_t *list_2, int n) {
  assert(list_1 != NULL);
  assert(list_2 != NULL);
  assert(n >= 0);
  assert(n <= list_len(list_2));

  if (n == 0) {
    return;
  }

  // reverse list_2 after the n-th pos and append to first one
  operation_t *prev = NULL;
  operation_t *nth_node = list_2;

  for (int i = 1; i < list_len(list_2) - n + 1; i++) {
    prev = nth_node;
    printf("%d -> ", nth_node->line_num);
    nth_node = nth_node->next_operation;
  }

  if (prev != NULL) {
    prev->next_operation = NULL;
  }
  prev = NULL;
  operation_t *current = nth_node;
  operation_t *next = NULL;

  while (current != NULL) {
    next = current->next_operation;
    current->next_operation = prev;
    prev = current;
    current = next;
  }

  // prev is the head of list_2 reversed from nth position

  //operation_t *ptr = prev;
  operation_t *tail = list_1;

  while (tail->next_operation != NULL) {
    tail = tail->next_operation;
  }

  tail->next_operation = prev;

} /* redo_n_operations() */

/*
 * This function frees the list
 */

void free_list(operation_t *head) {
  operation_t *current = head;
  operation_t *next = NULL;
  while (current != NULL) {
    next = current->next_operation;
    free(current->new_text);
    current->new_text = NULL;
    free(current);
    current = NULL;
    current = next;
  }
  current = NULL;
  next = NULL;
  head = NULL;
} /* free_list() */

/*
 * This function return the last line
 */

operation_t *doc_last_line(operation_t *head) {
  assert(head != NULL);
  int max_line = head->line_num;
  operation_t *max_ptr = head;
  operation_t *ptr = head;
  while (ptr != NULL) {
    if (max_line < ptr->line_num) {
      max_line = ptr->line_num;
      max_ptr = ptr;
    }
    ptr = ptr->next_operation;
  }

  return max_ptr;
} /* doc_last_line() */

/*
 * don't know what it does
 */

operation_t *interleave_operations(operation_t *list_1, operation_t *list_2) {
  assert(list_1 != NULL);
  assert(list_2 != NULL);
  operation_t *list_1_next = NULL;
  operation_t *list_2_next = NULL;

  while ((list_1 != NULL) && (list_2 != NULL)) {
    list_1_next = list_1->next_operation;
    list_2_next = list_2->next_operation;
    if (list_1_next == NULL) {
      list_1->next_operation = list_2;
      break;
    }
    list_1->next_operation = list_2;
    list_2->next_operation = list_1_next;
    list_1 = list_1_next;
    list_2 = list_2_next;
  }


  return list_1;
} /* interleave_operations() */

/*
 * this function writes
 */

int write_document(char *file, operation_t *head) {
  assert(file != NULL);
  assert(head != NULL);
  FILE *file_ptr = NULL;

  file_ptr = fopen(file, "w");
  if (file_ptr == NULL) {
    return NON_WRITABLE_FILE;
  }

  int max_value = 0;
  operation_t *ptr = head;
  while (ptr != NULL) {
    if (ptr->line_num > max_value) {
      max_value = ptr->line_num;
    }
    ptr = ptr->next_operation;
  }
  char *texts[max_value + 1];

  for (int i = 0; i <= max_value; i++) {
    (texts[i]) = malloc(sizeof(char) * MAX_TEXT_LEN);
    *(texts[i]) = '\0';
  }

  ptr = head;
  while (ptr != NULL) {
    strcpy(texts[ptr->line_num], ptr->new_text);
    ptr = ptr->next_operation;
  }

  int counter = 0;
  for (int i = 0; i <= max_value; i++) {
    if (strcmp(texts[i], "\0") != 0) {
      counter++;
    }
    fprintf(file_ptr, "%s\n", (texts[i]));
  }

  fclose(file_ptr);
  file_ptr = NULL;

  for (int i = 0; i <= max_value; i++) {
    free(texts[i]);
    texts[i] = NULL;
  }

  return counter;
} /* write_document() */

/* Remember, you don't need a main function!
 * it is provided by hw8_main.c or hw8_test.o
 */
