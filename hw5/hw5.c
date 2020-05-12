/* Name, hw5.c, CS 24000, Spring 2020
 * Last updated February 3, 2020
 */

/* Add any includes here */
#include "hw5.h"


#include <stdio.h>
#include <assert.h>
#include <string.h>


/*
 * This is a helper function
 * to see if a given employee
 * is valid or not
 */

int is_valid_employee(employee_t employee) {
  if (sizeof(employee) == 0) {
    return 0;
  }
  if (employee.last_name == NULL) {
    return 0;
  }

  int null_term_count = 0;
  for (int i = 0; i < MAX_NAME_LEN; i++) {
    if (employee.last_name[i] == '\0') {
      null_term_count++;
      break;
    }
  }

  if (null_term_count < 1) {
    return 0;
  }
  null_term_count = 0;

  for (int i = 0; i < MAX_NAME_LEN; i++) {
    if (employee.first_name[i] == '\0') {
      null_term_count++;
      break;
    }
  }

  if (null_term_count < 1) {
    return 0;
  }

  if (employee.id_number != FREE_OFFICE) {
    if (employee.id_number < 0) {
      return 0;
    }

    if ((employee.title < 0) || (employee.title > 2)) {
      return 0;
    }
  }

  for (int i = 0; i < N_DAYS; i++) {
    for (int j = 0 ; j < N_HOURS; j++) {
      if ((employee.schedule[i][j] != 'A')
        && (employee.schedule[i][j] != 'B')) {
        return 0;
      }
    }
  }
  return 1;
} /* is_valid_employee() */


/*
 * This function reads in the employee
 * at given position
 */

employee_t read_employee(FILE* file_in, int position) {
  assert(file_in != NULL);
  assert(position >= 0);
  employee_t temp = BAD_EMPLOYEE;

  fseek(file_in, sizeof(employee_t) * (position), SEEK_SET);
  if (!fread(&temp, sizeof(employee_t), 1, file_in)) {
    return BAD_EMPLOYEE;
  }

  return temp;
} /* read_employee() */

/*
 * This function writes the given employee
 * at the given position
 */

int write_employee(FILE *file_in, employee_t employee, int position) {
  assert(file_in != NULL);
  assert(position >= 0);
  fseek(file_in, 0, SEEK_END);
  if (position * sizeof(employee_t) > ftell(file_in)) {
    return WRITE_ERR;
  }

  assert(is_valid_employee(employee) == 1);
  fseek(file_in, sizeof(employee_t) * position, SEEK_SET);
  if (!fwrite(&employee, sizeof(employee_t), 1, file_in)) {
    return WRITE_ERR;
  }
  return OK;
} /* write_employee() */

/*
 * This fucntion hires the given employee
 */

int hire_employee(FILE *file_in, employee_t to_add) {
  assert(file_in != NULL);
  assert(is_valid_employee(to_add) == 1);

  int first_available_pos = -1;
  int index = 0;
  fseek(file_in, 0, SEEK_END);
  int len = ftell(file_in);
  fseek(file_in, 0, SEEK_SET);
  while (ftell(file_in) < len) {
    employee_t temporary = BAD_EMPLOYEE;
    if (!fread(&temporary, sizeof(employee_t), 1, file_in)) {
      return NO_OFFICE;
    }
    if (temporary.id_number == FREE_OFFICE) {
      first_available_pos = index;
      break;
    }
    index++;
  }

  if (first_available_pos == -1) {
    return NO_OFFICE;
  } else {
    fseek(file_in, sizeof(employee_t) * first_available_pos, SEEK_SET);
    if (fwrite(&to_add, sizeof(employee_t), 1, file_in) == 0) {
      return WRITE_ERR;
    } else {
      return first_available_pos;
    }
  }

} /* hire_employee() */

/*
 * This function fires the given employee
 */

int fire_employee(FILE *file_in, employee_t to_fire) {
  assert(file_in != NULL);
  assert(is_valid_employee(to_fire) == 1);

  fseek(file_in, 0, SEEK_END);
  int len = ftell(file_in);

  fseek(file_in, 0, SEEK_SET);

  int index = 0;
  while (ftell(file_in) < len) {
    employee_t temp = BAD_EMPLOYEE;
    if (!fread(&temp, sizeof(employee_t), 1, file_in)) {
      return NO_EMPLOYEE;
    }
    if (temp.id_number == to_fire.id_number) {
      if (strcmp(to_fire.last_name, temp.last_name) == 0) {
        if (strcmp(to_fire.first_name, temp.first_name) == 0) {
          fseek(file_in, sizeof(employee_t) * index, SEEK_SET);
          temp.id_number = FREE_OFFICE;
          if (!fwrite(&temp, sizeof(employee_t), 1, file_in)) {
            return WRITE_ERR;
          }
          return index;
        }
      }
    }
    index++;
  }
  return NO_EMPLOYEE;
} /* fire_employee() */

/*
 * This fucntion gives the percentage
 * of employees with salary more than
 * the given salary
 */

float percent_occupancy(FILE *file_in, float salary) {
  assert(file_in != NULL);
  assert(salary >= 0);

  fseek(file_in, 0, SEEK_END);
  int len = ftell(file_in);
  fseek(file_in, 0, SEEK_SET);
  int num_of_employee = 0;
  int num_of_employee_above = 0;
  while (ftell(file_in) < len) {
    employee_t temp = BAD_EMPLOYEE;
    if (!fread(&temp, sizeof(employee_t), 1, file_in)) {
      return 0.0;
    }
    num_of_employee++;
    if ((temp.salary > salary)
      && (temp.id_number != FREE_OFFICE)) {
      num_of_employee_above++;
    }
  }
  return (float) num_of_employee_above * 100.0 / (float) num_of_employee;
} /* percent_occupancy() */

/*
 * This function return the average salary
 * of all employess with the given title
 */

float average_salary_by_title(FILE *file_in, enum title_t title) {
  assert(file_in != NULL);
  assert((title >= 0) && (title <= 2));

  float total_salary = 0;
  int num_of_employee = 0;

  fseek(file_in, 0, SEEK_END);
  int len = ftell(file_in);
  fseek(file_in, 0, SEEK_SET);

  while (ftell(file_in) < len) {
    employee_t temp = BAD_EMPLOYEE;
    if (!fread(&temp, sizeof(employee_t), 1, file_in)) {
      return NO_EMPLOYEE;
    }
    if ((temp.id_number != FREE_OFFICE)
      && (temp.title == title)) {
      total_salary += temp.salary;
      num_of_employee++;
    }
  }

  if (num_of_employee == 0) {
    return NO_EMPLOYEE;
  }

  return total_salary / num_of_employee;
} /* average_salary_by_title() */


/*
 * This function return the employee
 * with the given ID
 */

employee_t find_employee_by_id(FILE *file_in, int id) {
  assert(file_in != NULL);
  assert(id > 0);
  fseek(file_in, 0, SEEK_END);
  int len = ftell(file_in);
  fseek(file_in, 0, SEEK_SET);

  while (ftell(file_in) < len) {
    employee_t temp = BAD_EMPLOYEE;
    if (!fread(&temp, sizeof(employee_t), 1, file_in)) {
      return BAD_EMPLOYEE;
    }
    if (temp.id_number == id) {
      return temp;
    }
  }

  return BAD_EMPLOYEE;
} /* find_employee_by_id() */

/*
 * This function return the employee
 * with the given name
 */

employee_t find_employee_by_name(FILE *file_in, char *first, char *last) {
  assert(file_in != NULL);
  assert(strlen(first) < MAX_NAME_LEN);
  assert(strlen(last) < MAX_NAME_LEN);

  fseek(file_in, 0, SEEK_END);
  int len = ftell(file_in);
  fseek(file_in, 0, SEEK_SET);

  while (ftell(file_in) < len) {
    employee_t temp = BAD_EMPLOYEE;
    if (!fread(&temp, sizeof(employee_t), 1, file_in)) {
      return BAD_EMPLOYEE;
    }
    if (strcmp(first, temp.first_name) == 0) {
      if (strcmp(last, temp.last_name) == 0) {
        return temp;
      }
    }
  }
  return BAD_EMPLOYEE;
} /* find_employee_by_name() */


/*
 * This function gives raise to the employee
 * with the given ID
 */

int give_raise(FILE *file_in, int id, float raise) {
  assert(file_in != NULL);
  assert(id > 0);

  fseek(file_in, 0, SEEK_END);
  int len = ftell(file_in);
  fseek(file_in, 0, SEEK_SET);
  int index = 0;
  while (ftell(file_in) < len) {
    employee_t temp = BAD_EMPLOYEE;
    if (!fread(&temp, sizeof(employee_t), 1, file_in)) {
      return NO_EMPLOYEE;
    }
    if (temp.id_number == id) {
      temp.salary += raise;
      if (temp.salary <= 0) {
        return fire_employee(file_in, temp);
      }
      fseek(file_in, sizeof(employee_t) * index, SEEK_SET);
      if (!fwrite(&temp, sizeof(temp), 1, file_in)) {
        return WRITE_ERR;
      }
      return OK;
    }
    index++;
  }

  return NO_EMPLOYEE;
} /* give_raise() */


/*
 * This function return the earliest possible time
 * when the given employees can meet
 */

int schedule_meeting(FILE *file_in, int id1, int id2) {
  assert(file_in != NULL);
  assert(id1 > 0);
  assert(id2 > 0);

  fseek(file_in, 0, SEEK_END);
  int len = ftell(file_in);
  fseek(file_in, 0, SEEK_SET);


  employee_t first = BAD_EMPLOYEE;
  employee_t second = BAD_EMPLOYEE;
  int first_found = 0;
  int second_found = 0;

  while (ftell(file_in) < len) {
    employee_t temp = BAD_EMPLOYEE;
    if (!fread(&temp, sizeof(temp), 1, file_in)) {
      return NO_OVERLAP;
    }
    if (temp.id_number == id1) {
      first = temp;
      first_found = 1;
    }
    if (temp.id_number == id2) {
      second = temp;
      second_found = 1;
    }
  }

  if ((first_found == 0) || (second_found == 0)) {
    return NO_EMPLOYEE;
  }

  for (int i = 0 ; i < N_DAYS; i++) {
    for (int j = 0; j < N_HOURS; j++) {
      if ((first.schedule[i][j] == 'A') && (second.schedule[i][j] == 'A')) {
        return i * 100 + (j + 8);
      }
    }
  }

  return NO_OVERLAP;
} /* schedule_meeting() */




/* Remember, you don't need a main function!
 * it is provided by hw5_main.c or hw5_test.o
 */
