/* Name, hw10.c, CS 24000, Spring 2020
 * Last updated March 23, 2020
 */

/* Add any includes here */

#include "hw10.h"

#include <malloc.h>
#include <malloc_debug.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

/*
 * creates a new tab
 */

tab_t *create_tab(char *page_name, char *url) {
  tab_t *ptr = malloc(sizeof(tab_t));
  assert(ptr != NULL);
  ptr->tab_no = 1;
  ptr->page_info = malloc(sizeof(page_data_t));
  assert(ptr->page_info != NULL);
  ptr->page_info->current_page = true;
  ptr->page_info->page_name = malloc((strlen(page_name) + 1 ) * sizeof(char));
  assert(ptr->page_info->page_name != NULL);
  strcpy(ptr->page_info->page_name, page_name);
  ptr->page_info->page_no = 1;
  ptr->page_info->url = malloc((strlen(url) + 1) * sizeof(char));
  assert(ptr->page_info->url != NULL);
  strcpy(ptr->page_info->url, url);
  ptr->next_tab = NULL;
  ptr->prev_tab = NULL;
  ptr->next_page = NULL;
  ptr->prev_page = NULL;
  return ptr;
} /* create_tab() */

/*
 * open a new tab
 */

void new_tab(tab_t **head_ptr) {
  assert(head_ptr != NULL);

  if (!(*head_ptr)) {
    *head_ptr = create_tab("Purdue CS", "cs.purdue.edu");
    return;
  }
  tab_t *ptr = *head_ptr;
  int max_tab_no = 1;
  while (ptr->next_tab != NULL) {
    if (ptr->tab_no > max_tab_no) {
      max_tab_no = ptr->tab_no;
    }
    ptr = ptr->next_tab;
  }
  if (ptr->tab_no > max_tab_no) {
    max_tab_no = ptr->tab_no;
  }
  tab_t *new_tab = create_tab("Purdue CS", "cs.purdue.edu");
  new_tab->tab_no = max_tab_no + 1;
  new_tab->prev_tab = ptr;
  ptr->next_tab = new_tab;
  tab_t *prev_pages = ptr->prev_page;

  while (prev_pages) {
    prev_pages->next_tab = new_tab;
    prev_pages = prev_pages->prev_page;
  }
  tab_t *next_pages = ptr->next_page;

  while (next_pages) {
    next_pages->next_tab = new_tab;
    next_pages = next_pages->next_page;
  }
} /* new_tab() */

/*
 * closes a tab
 */

int close_tab(tab_t **head_ptr, int tab_no) {
  assert(head_ptr != NULL);
  assert(tab_no > 0);
  tab_t *ptr = *head_ptr;

  while (ptr) {
    if (ptr->tab_no == tab_no) {
      free(ptr->page_info->page_name);
      free(ptr->page_info->url);
      free(ptr->page_info);

      if (ptr->prev_tab) {
        tab_t *prev_pages = ptr->prev_tab;
        while (prev_pages) {
          prev_pages->next_tab = ptr->next_tab;
          prev_pages = prev_pages->prev_page;
        }
        tab_t *next_pages = ptr->prev_tab->next_page;
        while (next_pages) {
          next_pages->next_tab = ptr->next_tab;
          next_pages = next_pages->next_page;
        }
      }

      if (ptr->next_tab) {
        tab_t *prev_pages = ptr->next_tab;
        while (prev_pages) {
          prev_pages->prev_tab = ptr->prev_tab;
          prev_pages = prev_pages->prev_page;
        }
        tab_t *next_pages = ptr->next_tab->next_page;
        while (next_pages) {
          next_pages->prev_tab = ptr->prev_tab;
          next_pages = next_pages->next_page;
        }
      }

      if (!ptr->prev_tab) {
        if (ptr->next_tab) {
          *head_ptr = ptr->next_tab;
        }
        else {
          *head_ptr = NULL;
        }
      }

      ptr->prev_tab = NULL;
      ptr->next_tab = NULL;
      tab_t *prev_pages = ptr->prev_page;
      while (prev_pages) {
        tab_t *prev = prev_pages->prev_page;
        free(prev_pages->page_info->url);
        free(prev_pages->page_info->page_name);
        free(prev_pages->page_info);
        free(prev_pages);
        prev_pages = NULL;
        prev_pages = prev;
      }
      tab_t *next_pages = ptr->next_page;
      while (next_pages) {
        tab_t *next = next_pages->next_page;
        free(next_pages->page_info->url);
        free(next_pages->page_info->page_name);
        free(next_pages->page_info);
        free(next_pages);
        next_pages = NULL;
        next_pages = next;
      }
      prev_pages = NULL;
      next_pages = NULL;
      free(ptr);
      ptr = NULL;
      return SUCCESS;
    }
    ptr = ptr->next_tab;
  }
  return NO_TAB;
} /* close_tab() */

/*
 * close the browser
 */

void close_browser(tab_t **head_ptr) {
  assert(head_ptr != NULL);
  tab_t *ptr = *head_ptr;
  while (ptr) {
    tab_t *next = ptr->next_tab;
    close_tab(&ptr, ptr->tab_no);
    ptr = next;
  }
  *head_ptr = NULL;
} /* close_browser() */

/*
 * go to the prev page
 */

int page_go_prev(tab_t **head_ptr, int tab_no) {
  assert(head_ptr != NULL);
  assert(tab_no > 0);
  tab_t *ptr = *head_ptr;
  while (ptr) {
    if (ptr->tab_no == tab_no) {
      if (!ptr->prev_page) {
        return NO_PAGE;
      }
      ptr->page_info->current_page = false;
      ptr->prev_page->page_info->current_page = true;
      tab_t *prev_tab = ptr->prev_tab;
      tab_t *next_tab = ptr->next_tab;

      if (prev_tab) {
        tab_t *prev_pages = prev_tab;
        tab_t *next_pages = prev_tab->next_page;

        while (prev_pages) {
          prev_pages->next_tab = ptr->prev_page;
          prev_pages = prev_pages->prev_page;
        }
        while (next_pages) {
          next_pages->next_tab = ptr->prev_page;
          next_pages = next_pages->next_page;
        }
      }

      if (next_tab) {
        tab_t *prev_pages = next_tab;
        tab_t *next_pages = next_tab->next_page;

        while (prev_pages) {
          prev_pages->prev_tab = ptr->prev_page;
          prev_pages = prev_pages->prev_page;
        }
        while (next_pages) {
          next_pages->prev_tab = ptr->prev_page;
          next_pages = next_pages->next_page;
        }
      }
      return SUCCESS;
    }
    ptr = ptr->next_tab;
  }
  return NO_TAB;
} /* page_go_prev() */

/*
 * go to the next page
 */

int page_go_next(tab_t **head_ptr, int tab_no) {
  assert(head_ptr != NULL);
  assert(tab_no > 0);
  tab_t *ptr = *head_ptr;
  while (ptr) {
    if (ptr->tab_no == tab_no) {
      if (!ptr->next_page) {
        return NO_PAGE;
      }
      ptr->page_info->current_page = false;
      ptr->next_page->page_info->current_page = true;
      tab_t *prev_tab = ptr->prev_tab;
      tab_t *next_tab = ptr->next_tab;

      if (prev_tab) {
        tab_t *prev_pages = prev_tab;
        tab_t *next_pages = prev_tab->next_page;

        while (prev_pages) {
          prev_pages->next_tab = ptr->next_page;
          prev_pages = prev_pages->prev_page;
        }
        while (next_pages) {
          next_pages->next_tab = ptr->next_page;
          next_pages = next_pages->next_page;
        }
      }

      if (next_tab) {
        tab_t *prev_pages = next_tab;
        tab_t *next_pages = next_tab->next_page;

        while (prev_pages) {
          prev_pages->prev_tab = ptr->next_page;
          prev_pages = prev_pages->prev_page;
        }
        while (next_pages) {
          next_pages->prev_tab = ptr->next_page;
          next_pages = next_pages->next_page;
        }
      }
      return SUCCESS;
    }
    ptr = ptr->next_tab;
  }
  return NO_TAB;
} /* page_go_next() */

/*
 * opens a new page
 */

int open_page(tab_t **head_ptr, int tab_no, char *page_name, char *url) {
  assert(head_ptr != NULL);
  assert(tab_no > 0);
  assert(page_name != NULL);
  assert(url != NULL);

  tab_t *ptr = *head_ptr;

  while (ptr) {
    if (ptr->tab_no == tab_no) {
      tab_t *new_page = create_tab(page_name, url);
      new_page->prev_tab = ptr->prev_tab;
      new_page->next_tab = ptr->next_tab;
      new_page->tab_no = ptr->tab_no;
      new_page->prev_page = ptr;
      int max_page_no = 1;

      tab_t *prev_pages = ptr;
      while (prev_pages) {
        if (prev_pages->page_info->page_no > max_page_no) {
          max_page_no = prev_pages->page_info->page_no;
        }
        prev_pages = prev_pages->prev_page;
      }
      tab_t *next_pages = ptr->next_page;
      while (next_pages) {
        if (next_pages->page_info->page_no > max_page_no) {
          max_page_no = next_pages->page_info->page_no;
        }
        next_pages = next_pages->next_page;
      }

      next_pages = ptr->next_page;
      while (next_pages) {
        tab_t *next = next_pages->next_page;
        free(next_pages->page_info->page_name);
        free(next_pages->page_info->url);
        free(next_pages->page_info);
        free(next_pages);
        next_pages = NULL;
        next_pages = next;
      }

      new_page->page_info->page_no = max_page_no + 1;
      ptr->next_page = new_page;
      page_go_next(head_ptr, tab_no);
      return SUCCESS;
    }
    ptr = ptr->next_tab;
  }

  return NO_TAB;
} /* open_page() */

/*
 * returns total num of pages
 */

int num_pages(tab_t **head_ptr) {
  assert(head_ptr);

  tab_t *ptr = *head_ptr;
  int page_count = 0;

  while (ptr) {
    tab_t *prev_pages = ptr;
    tab_t *next_pages = ptr->next_page;
    while (prev_pages) {
      page_count++;
      prev_pages = prev_pages->prev_page;
    }
    while (next_pages) {
      page_count++;
      next_pages = next_pages->next_page;
    }
    ptr = ptr->next_tab;
  }
  return page_count;
} /* num_pages() */

/*
 * writes all tabs
 */

int write_all_tabs(tab_t **head_ptr, char *file_name) {
  assert(head_ptr);
  assert(file_name);

  FILE *file_ptr = NULL;
  file_ptr = fopen(file_name, "w");

  if (!file_ptr) {
    return NON_WRITABLE_FILE;
  }

  tab_t *ptr = *head_ptr;

  while (ptr) {
    fprintf(file_ptr, "TAB %d\n", ptr->tab_no);
    fprintf(file_ptr, "Page number = %d\n", ptr->page_info->page_no);
    fprintf(file_ptr, "Page name = %s\n", ptr->page_info->page_name);
    fprintf(file_ptr, "Link = %s\n", ptr->page_info->url);
    ptr = ptr->next_tab;
  }
  fclose(file_ptr);
  file_ptr = NULL;
  return SUCCESS;
} /* write_all_tabs() */

/* Remember, you don't need a main function!
 * it is provided by hw10_main.c or hw10_test.o
 */
