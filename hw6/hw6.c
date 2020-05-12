/* Name, hw6.c, CS 24000, Spring 2020
 * Last updated February 10, 2020
 */

/* Add any includes here */
#include "hw6.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>

int g_password_count = 0;
password_t g_password_array[MAX_PASSWORDS];

/*
 * This functions return the min of
 * two numbers
 */

int min(int a, int b) {
  return a > b ? b : a;
} /* min() */


/*
 * this function return the reverse of a string
 */

char* strrev(char *string) {
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
 * This function reads values
 * from the input and
 * writes them to the array
 */

int read_passwords(char *file) {
  assert(file != NULL);
  if (access(file, R_OK) != 0) {
    return NON_READABLE_FILE;
  }
  FILE *file_ptr_in = NULL;
  file_ptr_in = fopen(file, "r");

  int counter = 0;
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

    if (fscanf(file_ptr_in,
      "%50[^&\n]&%50[^&\n]&%50[^\n]\n",
        buffer_1, buffer_2, buffer_3) < 3) {
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
    g_password_array[counter] = password;
    counter++;
  }
  g_password_count = counter;
  fclose(file_ptr_in);
  file_ptr_in = NULL;
  return counter;
} /* read_passwords() */


/*
 * This function returns the
 * number of spies
 */

int locate_spies(char **code_names, int num_of_code_names) {
  assert(code_names != NULL);
  assert(num_of_code_names > 0);

  for (int i = 0; i < num_of_code_names; i++) {
    assert(code_names[i] != NULL);
  }

  int spy_count = 0;

  for (int i = 0; i < g_password_count; i++) {
    char *passcode_value = g_password_array[i].passcode_value;
    char *passcode_name = g_password_array[i].passcode_name;

    for (int j = 0; j < num_of_code_names; j++) {
      char *code_name = code_names[j];
      int flag = 0;
      if (strcmp(g_password_array[i].code_name, code_name) == 0) {
        if (strlen(passcode_value) >= 7) {
          if (strstr(passcode_value, "rooster") != NULL) {
            flag = 1;
          }
        }
        if (strlen(passcode_name) >= 7) {
          if (strncmp(passcode_name, "the", 3) == 0) {
            if (strncmp(strrev(passcode_name), "puos", 4) == 0) {
              flag = 1;
              strrev(passcode_name);
            } else {
              strrev(passcode_name);
            }
          }
        }
        if (strlen(passcode_value) < strlen(passcode_name)) {
          flag = 1;
        }
        if (flag) {
          spy_count++;
          break;
        }
      }
    }
  }

  return spy_count;
} /* locate_spies() */

/* Remember, you don't need a main function!
 * it is provided by hw6_main.c or hw6_test.o
 */
