/* Name, hw7.c, CS 24000, Spring 2020
 * Last updated February 19, 2020
 */

/* Add any includes here */

#include "hw7.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

int g_password_count = 0;

password_t g_password_array[MAX_PASSWORDS];



/*
 * This function return the min
 */

int min(int a, int b) {
  return a > b ? b : a;
} /* min() */

/*
 * This return the reverse string
 */

char * strrev(char *string) {
  assert(string != NULL);
  int i = 0;
  char temp[strlen(string)];
  while (string[i] != '\0') {
    temp[i] = string[strlen(string) - i - 1];
    i++;
  }
  string = temp;
  return string;
} /* strrev() */


/*
 * This function determines whether the given
 * thing is a spy or not
 */

int is_spy(password_t *ptr) {
  if (strstr(ptr->passcode_value, "rooster") != NULL) {
    return 1;
  }

  if (strlen(ptr->passcode_name) >= 7) {
    if (strncmp(ptr->passcode_name, "the", 3) == 0) {
      if (strncmp(strrev(ptr->passcode_name), "puos", 4) == 0) {
        strrev(ptr->passcode_name);
        return 1;
      }
      strrev(ptr->passcode_name);
    }
  }

  if (strlen(ptr->passcode_value) < strlen(ptr->passcode_name)) {
    return 1;
  }
  return 0;
} /* is_spy() */

/*
 * This function reads the passwords
 */

int read_passwords(char* file) {
  assert(file != NULL);

  if (access(file, R_OK) != 0) {
    return NON_READABLE_FILE;
  }
  FILE *file_ptr_in = NULL;
  file_ptr_in = fopen(file, "r");

  int counter = 0;
  fseek(file_ptr_in, 0, SEEK_END);
  int len = ftell(file_ptr_in);
  if (len == 0) {
    fclose(file_ptr_in);
    file_ptr_in = NULL;
    return NO_PASSWORDS;
  }

  fseek(file_ptr_in, 0, SEEK_SET);
  while (!feof(file_ptr_in)) {
    if (counter >= MAX_PASSWORDS) {
      fclose(file_ptr_in);
      file_ptr_in = NULL;
      return TOO_MUCH_DATA;
    }
    password_t password = {0};

    char buffer_1[MAX_BUFF_LEN];
    //buffer_1[MAX_BUFF_LEN] = '\0';
    char buffer_2[MAX_BUFF_LEN];
    //buffer_2[MAX_BUFF_LEN] = '\0';
    char buffer_3[MAX_BUFF_LEN];
    //buffer_3[MAX_BUFF_LEN] = '\0';
    char buffer_4[MAX_BUFF_LEN];


    if (fscanf(file_ptr_in,
      "%50[^&\n]&%50[^&\n]&%50[^&\n]&%50[^\n]\n",
        buffer_1, buffer_2, buffer_3, buffer_4) < 4) {
      fclose(file_ptr_in);
      file_ptr_in = 0;
      return BAD_RECORD;
    }

    strncpy(password.code_name, buffer_1, min(MAX_NAME_LEN - 1,
      strlen(buffer_1)));
    password.code_name[strlen(password.code_name)] = '\0';
    strncpy(password.passcode_name, buffer_2, min(MAX_NAME_LEN - 1,
      strlen(buffer_2)));
    password.passcode_name[strlen(password.passcode_name)] = '\0';
    strncpy(password.passcode_value, buffer_3, min(MAX_NAME_LEN - 1,
      strlen(buffer_3)));
    password.passcode_value[strlen(password.passcode_value)] = '\0';
    strncpy(password.next_member, buffer_4, min(MAX_NAME_LEN - 1,
      strlen(buffer_4)));
    password.next_member[strlen(password.next_member)] = '\0';
    g_password_array[counter] = password;
    counter++;
  }
  g_password_count = counter;
  fclose(file_ptr_in);
  file_ptr_in = NULL;
  return counter;
} /* read_passwords() */


/*
 * This function connects the members
 */

void connect_members() {
  for (int i = 0; i < g_password_count; i++) {
    g_password_array[i].next_member_ptr = NULL;
    for (int j = 0; j < g_password_count; j++) {
      if (i != j) {
        if (!strcmp(g_password_array[i].next_member,
          g_password_array[j].code_name)) {
          g_password_array[i].next_member_ptr = &g_password_array[j];
          break;
        }
      }
    }
  }
} /* connect_members() */

/*
 * This function isolates the spy
 */

int isolate_spy(password_t *spy) {
  assert(spy != NULL);

  for (int i = 0; i < g_password_count; i++) {
    if (&g_password_array[i] == spy) {
      int spy_found = 0;
      if (strstr(spy->passcode_value, "rooster") != NULL) {
        spy_found = 1;
      } else if (strlen(spy->passcode_name) >= 7) {
        if (strncmp(spy->passcode_name, "the", 3) == 0) {
          if (strncmp(strrev(spy->passcode_name), "puos", 4) == 0) {
            spy_found = 1;
          }
          strrev(spy->passcode_name);
        }
      } else if (strlen(spy->passcode_value) < strlen(spy->passcode_name)) {
        spy_found = 1;
      }

      if (spy_found) {
        /*if (spy->next_member_ptr == NULL) {
          return 0;
        }*/
        int count = 0;
        for (int j = 0; j < g_password_count; j++) {
          if (i != j) {
            if (g_password_array[j].next_member_ptr == spy) {
              strcpy(g_password_array[j].next_member,
                  spy->next_member);

              if (spy->next_member_ptr == &g_password_array[j]) {
                g_password_array[j].next_member_ptr = NULL;
              }
              else {
                g_password_array[j].next_member_ptr = spy->next_member_ptr;
              }
              if (spy->next_member_ptr == spy) {
                g_password_array[j].next_member_ptr = NULL;
              }
              count++;
            }
          }
        }
        spy->next_member_ptr = NULL;
        return count;
      }
    }
  }
  return NOT_A_SPY;
} /* isolate_spy() */

/*
 * This function sends message from
 * one member to another
 */

int send_message(password_t *sender, password_t *receiver) {
  assert(sender != NULL);
  assert(receiver != NULL);

  if (sender->next_member_ptr == NULL) {
    return MESSAGE_NOT_SENT;
  }

  if (sender == receiver) {
    return MESSAGE_NOT_SENT;
  }

  if (sender->next_member_ptr == sender) {
    return MESSAGE_NOT_SENT;
  }

  int counter = 1;
  password_t *ptr = sender;
  int spy_count = 0;
  while (counter < g_password_count) {
    if (is_spy(ptr)) {
      spy_count++;
    }
    if (ptr == NULL) {
      return MESSAGE_NOT_SENT;
    }
    if (counter >= g_password_count) {
      return MESSAGE_NOT_SENT;
    }
    if (ptr == receiver) {
      return spy_count;
    }
    if (ptr->next_member_ptr == NULL) {
      return MESSAGE_NOT_SENT;
    }
    ptr = ptr->next_member_ptr;
    counter++;
  }

  return MESSAGE_NOT_SENT;
} /* send_message() */

/* Remember, you don't need a main function!
 * it is provided by hw7_main.c or hw7_test.o
 */
