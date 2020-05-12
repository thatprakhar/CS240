/* Name, hw9.c, CS 24000, Spring 2020
 * Last updated March 6, 2020
 */

/* Add any includes here */

#include "hw9.h"

#include <string.h>
#include <malloc.h>
#include <malloc_debug.h>
#include <assert.h>

/*
 * creates a commit
 */

commit_t *create_commit(char *name, float time, char *hash) {
  assert(name != NULL);
  assert(hash != NULL);
  assert((time <= 24) && (time >= 0));

  entry_t *new_entry = malloc(sizeof(entry_t));
  assert(new_entry != NULL);
  new_entry->author = malloc(strlen(name) + 1);
  assert(new_entry->author != NULL);
  strcpy(new_entry->author, name);
  new_entry->hash = malloc(strlen(hash) + 1);
  assert(new_entry->hash != NULL);
  strcpy(new_entry->hash, hash);
  new_entry->time = time;

  commit_t *new_commit = malloc(sizeof(commit_t));
  assert(new_commit != NULL);
  new_commit->data = new_entry;
  new_commit->next_commit = NULL;
  new_commit->prev_commit = NULL;
  return new_commit;
} /* create_commit() */

/*
 * finds a commit
 */

commit_t *find_commit(commit_t *head, char *hash) {
  assert(head != NULL);
  assert(hash != NULL);

  commit_t *ptr = head;

  while (ptr != NULL) {
    if (!strcmp(ptr->data->hash, hash)) {
      return ptr;
    }
    ptr = ptr->next_commit;
  }
  return NULL;
} /* find_commit() */

/*
 * inserts a commit
 */

void insert_commit(commit_t *head, commit_t *add, char *hash) {
  assert(add != NULL);
  if (head == NULL) {
    head = add;
  }

  if (hash == NULL) {
    add->next_commit = head;
    head->prev_commit = add;
    return;
  }

  if (strcmp(hash, "\0") == 0) {
    add->next_commit = head;
    head->prev_commit = add;
    return;
  }

  commit_t *ptr = head;

  while (ptr != NULL) {
    if (!strcmp(ptr->data->hash, hash)) {
      if (ptr->next_commit != NULL) {
        add->next_commit = ptr->next_commit;
        add->next_commit->prev_commit = add;
      }
      add->prev_commit = ptr;
      add->prev_commit->next_commit = add;
      return;
    }
    ptr = ptr->next_commit;
  }


  add->next_commit = head;
  head->prev_commit = add;

} /* insert_commit() */

/*
 * removes a commit
 */

commit_t *remove_commit(commit_t *head, char *hash) {
  assert(head != NULL);
  assert(hash != NULL);

  commit_t *ptr = head;

  if (!strcmp(head->data->hash, hash)) {
    head->next_commit->prev_commit = NULL;
    head->next_commit = NULL;
    return head;
  }

  while (ptr != NULL) {
    if (!strcmp(ptr->data->hash, hash)) {
      ptr->prev_commit->next_commit = ptr->next_commit;
      if (ptr->next_commit != NULL) {
        ptr->next_commit->prev_commit = ptr->prev_commit;
        ptr->next_commit = NULL;
      }
      ptr->prev_commit = NULL;
      return ptr;
    }
    ptr = ptr->next_commit;
  }

  return NULL;
} /* remove_commit() */

/*
 * removes all the commit by the author
 */

int remove_all_commits_by_author(commit_t *head, char *author) {
  assert(author != NULL);
  assert(head != NULL);

  commit_t *ptr = head;
  int count = 0;
  while (ptr != NULL) {
    commit_t *next = ptr->next_commit;
    if (!strcmp(ptr->data->author, author)) {
      if (ptr->prev_commit != NULL) {
        ptr->prev_commit->next_commit = ptr->next_commit;
      }
      if (ptr->next_commit != NULL) {
        ptr->next_commit->prev_commit = ptr->prev_commit;
      }
      ptr->prev_commit = NULL;
      ptr->next_commit = NULL;
      count++;
    }
    ptr = next;
  }

  return count;
} /* remove_all_commits_by_author() */

/*
 * fixes connections in list
 */

commit_t *repair_log(commit_t *head, commit_t *tail) {
  assert(head != NULL);
  assert(tail != NULL);

  if (head == tail) {
    printf("sike\n");
    return NULL;
  }

  // checking if next is broken
  commit_t *ptr = head;
  while (ptr->next_commit != NULL) {
    ptr = ptr->next_commit;
  }

  if (ptr != tail) {
    // next is broken
    commit_t *ptr_2 = tail;
    while (ptr_2->prev_commit != ptr) {
      ptr_2 = ptr_2->prev_commit;
    }
    ptr->next_commit = ptr_2;
    return ptr;
  }
  // prev is broken
  commit_t *ptr_2 = tail;
  while (ptr_2->prev_commit != NULL) {
    ptr_2 = ptr_2->prev_commit;
  }
  ptr = head;
  while (ptr->next_commit != ptr_2) {

    ptr = ptr->next_commit;
    if (ptr == NULL) {
      // NO ERROR IN THE LIST
      return ptr;
    }
  }

  ptr_2->prev_commit = ptr;

  return ptr_2;
} /* repair_log() */

/*
 * fixes the tail
 */

commit_t *disconnect_loop(commit_t *head) {
  assert(head != NULL);
  commit_t *ptr = head;

  while (ptr->next_commit != NULL) {
    if (ptr->next_commit->prev_commit != ptr) {
      ptr->next_commit = NULL;
      return ptr;
    }
    ptr = ptr->next_commit;
  }
  return NULL;
} /* disconnect_loop() */

/*
 * frees a particular commit
 */

void free_commit(commit_t *ptr) {
  if (ptr != NULL) {
    free(ptr->data->author);
    free(ptr->data->hash);
    free(ptr->data);
    ptr->next_commit = NULL;
    ptr->prev_commit = NULL;
    free(ptr);
    ptr = NULL;
  }
} /* free_commit() */

/* Remember, you don't need a main function!
 * it is provided by hw9_main.c or hw9_test.o
 */
