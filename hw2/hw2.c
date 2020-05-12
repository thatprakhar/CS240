/* Name, hw2.c, CS 24000, Spring 2020
 * Last updated January 22, 2020
 */

/* Add any includes here */

#include "hw2.h"
#include <stdio.h>
#include <math.h>
#include <string.h>

#define ARRAY_LENGTH (100)

/*
 * This function returns the
 * average number of sleep
 * hours for the given month
 * and year
 */

float get_average_sleep_hours(char *file_name, int year, int month) {
  // file pointer to read the file

  if ((month > 12) || (month < 1)) {
    return BAD_DATE;
  }

  FILE *file_ptr = 0;

  file_ptr = fopen(file_name, "r");

  if (file_ptr == NULL) {

    file_ptr = NULL;
    return FILE_READ_ERR ;
  }


  char buffer[100];
  int y = 0;
  int m = 0;
  int d = 0;
  float sleep_hours = 0.0;
  int moving_minutes = 0;
  int workout_minutes = 0;

  if (fscanf(file_ptr, "%[^\n]\n", buffer) == 0) {
    fclose(file_ptr);
    file_ptr = NULL;

    return BAD_RECORD;
  }

  if (strlen(buffer) > MAX_NAME_LEN) {
    fclose(file_ptr);
    file_ptr = NULL;
    return BAD_RECORD;
  }


  float avg_sleep = 0;
  int count = 0;
  int temp = 0;
  while ((temp = fscanf(file_ptr, "%d/%d/%d|%f|%d|%d\n", &m, &d,
    &y, &sleep_hours, &moving_minutes, &workout_minutes)) != EOF) {

    if (temp < 6) {
      fclose(file_ptr);
      file_ptr = NULL;
      return BAD_RECORD;
    }


    if ((m < 1) || (m > 12) || (d < 1) || (d > 31)) {
      fclose(file_ptr);
      file_ptr = NULL;
      return BAD_DATE;
    }

    if ((m == month) && (y == year)) {
      avg_sleep += sleep_hours;
      count += 1;
    }
  }
  if (count < 1) {
    fclose(file_ptr);
    file_ptr = NULL;
    return NO_DATA_POINTS;
  }
  avg_sleep /= count;

  fclose(file_ptr);
  file_ptr = NULL;
  return avg_sleep;

} /* get_average_sleep_hours() */

/*
 * This method creates a
 * visualization of the
 * sleeping habits of the
 * individual
 */

int get_sleep_log(char *in_file, char *out_file) {
  FILE *file_ptr = 0;
  file_ptr = fopen(in_file, "r");

  if (file_ptr == NULL) {
    return FILE_READ_ERR;
  }



  char name[ARRAY_LENGTH];
  if (fscanf(file_ptr, "%[^\n]\n", name) == 0) {
    fclose(file_ptr);
    file_ptr = NULL;
    return BAD_RECORD;
  }

  if (strlen(name) > MAX_NAME_LEN) {
    fclose(file_ptr);
    file_ptr = NULL;
    return BAD_RECORD;
  }


  int date[2000][3];
  float sleep_hours[2000];
  int moving_minutes = 0;
  int workout_minutes = 0;
  int year = 0;
  int month = 0;
  float sleep_hour = 0.0;
  int counter = 0;
  int num_of_values = 0;

  while ((num_of_values = fscanf(file_ptr,
    "%d/%d/%d|%f|%d|%d\n", &date[counter][0],
    &date[counter][1], &date[counter][2], &sleep_hour,
      &moving_minutes, &workout_minutes)) != EOF) {
    if (num_of_values < 6) {
      fclose(file_ptr);
      file_ptr = NULL;
      return BAD_RECORD;
    }

    if ((date[counter][0] < 1) || (date[counter][0] > 12) ||
      (date[counter][1] < 1) || (date[counter][1] > 31)) {
      fclose(file_ptr);
      file_ptr = NULL;
      return BAD_DATE;
    }
    if (date[counter][2] >= year) {
      year = date[counter][2];
    }
    sleep_hours[counter] = sleep_hour;
    counter++;
  }

  for (int i = 0; i < counter; i++) {
    if (date[i][2] == year) {
      if (date[i][0] >= month) {
        month = date[i][0];
      }
    }
  }

  fclose(file_ptr);
  file_ptr = NULL;

  file_ptr = fopen(out_file, "w");

  fprintf(file_ptr, "Name: %s, Month: %d, Year: %d\n", name, month, year);
  fprintf(file_ptr, "HOUR: 0 1 2 3 4 5 6 7 8 9 10\n");

  int num_of_days = 0;
  float total_sleep = 0;

  for (int i = 0; i < counter; i++) {
    if ((date[i][0] == month) && (date[i][2] == year)) {
      num_of_days++;
      total_sleep += sleep_hours[i];



      char write_string[100];

      if (sleep_hours[i] > 0) {

        for (int j = 0; j <= (int) (sleep_hours[i] * 2.0) + 1; j++) {
          if (j > 20) {
            write_string[j] = '\0';
            break;
          }
          if (j == 0) {
            write_string[j] = '|';
          } else if (j == (int) (sleep_hours[i] * 2.0) ) {
            write_string[j] = '|';
            write_string[j + 1] = '\0';
            break;
          } else {
            write_string[j] = '-';
          }
        }
      } else {
        write_string[0] = '|';
        write_string[1] = '\0';
      }
      write_string[strlen(write_string)] = '\0';

      if (date[i][1] > 9) {
        fprintf(file_ptr, "%d%s%s\n", date[i][1], "    " , write_string);
      } else {
        fprintf(file_ptr, "%d%s%s\n", date[i][1], "     ", write_string);
      }

    }
  }

  fprintf(file_ptr, "Average Sleep Hours: %.2f hours",
    total_sleep / num_of_days);
  fclose(file_ptr);
  file_ptr = NULL;

  return 0;
} /* get_sleep_log() */

/*
 * This method compares sleeping
 * pattern for two people and writes
 * them to a file
 */

int compare_sleep_hours(char *in_file_1, char *in_file_2, char *out_file) {

  FILE *file_ptr_1 = NULL;
  file_ptr_1 = fopen(in_file_1, "r");

  if (file_ptr_1 == NULL) {
    return FILE_READ_ERR;
  }

  FILE *file_ptr_2 = NULL;
  file_ptr_2 = fopen(in_file_2, "r");

  if (file_ptr_2 == NULL) {
    fclose(file_ptr_1);
    file_ptr_1 = NULL;
    return FILE_READ_ERR;
  }

  FILE *file_out = NULL;
  file_out = fopen(out_file, "w");

  if (file_out == NULL) {
    fclose(file_ptr_1);
    file_ptr_1 = NULL;
    fclose(file_ptr_2);
    file_ptr_2 = NULL;
    return FILE_WRITE_ERR;
  }


  char name_1[ARRAY_LENGTH];
  char name_2[ARRAY_LENGTH];

  if ((fscanf(file_ptr_1, "%[^\n]\n", name_1) == 0) ||
    (fscanf(file_ptr_2, "%[^\n]\n", name_2) == 0)) {
    fclose(file_ptr_1);
    fclose(file_ptr_2);
    file_ptr_1 = NULL;
    file_ptr_2 = NULL;
    fclose(file_out);
    file_out = NULL;
    return BAD_RECORD;
  }

  if ((strlen(name_1) > MAX_NAME_LEN) || (strlen(name_2) > MAX_NAME_LEN)) {
    fclose(file_ptr_1);
    fclose(file_ptr_2);
    file_ptr_1 = NULL;
    file_ptr_2 = NULL;
    fclose(file_out);
    file_out = NULL;
    return BAD_RECORD;
  }

  int date[2000][3] ;
  float sleep_hours_1[2000];
  float sleep_hours_2[2000];

  int working_minutes = 0;
  int moving_minutes = 0;

  if (fscanf(file_ptr_1, "%d/%d/%d|%f|%d|%d\n", &date[0][0], &date[0][1],
    &date[0][2], &sleep_hours_1[0], &moving_minutes,
      &working_minutes) == 0) {
    fclose(file_ptr_1);
    fclose(file_ptr_2);
    file_ptr_1 = NULL;
    file_ptr_2 = NULL;
    fclose(file_out);
    file_out = NULL;

    return BAD_RECORD;
  }
  int month = 0;
  int year = 0;
  int counter_1 = 1;
  int num_of_var = 0;
  while ((num_of_var = fscanf(file_ptr_1, "%d/%d/%d|%f|%d|%d\n",
    &date[counter_1][0], &date[counter_1][1], &date[counter_1][2],
      &sleep_hours_1[counter_1], &moving_minutes, &working_minutes)) != EOF) {
    if (num_of_var < 6) {
      fclose(file_ptr_1);
      fclose(file_ptr_2);
      file_ptr_1 = NULL;
      file_ptr_2 = NULL;
      fclose(file_out);
      file_out = NULL;

      return BAD_RECORD;
    }

    if ((date[counter_1][0] < 1) || (date[counter_1][0] > 12) ||
      (date[counter_1][1] < 1) || (date[counter_1][1] > 31)) {
      fclose(file_ptr_1);
      fclose(file_ptr_2);
      file_ptr_1 = NULL;
      file_ptr_2 = NULL;
      fclose(file_out);
      file_out = NULL;

      return BAD_DATE;
    }

    if (date[counter_1][2] >= year) {
      year = date[counter_1][2];
    }
    counter_1++;
  }

  for (int i = 0; i < counter_1; i++) {
    if (date[i][2] == year) {
      if (date[i][0] >= month) {
        month = date[i][0];
      }
    }
  }

  int counter_2 = 0;
  int date_2[2000][3];
  while ((num_of_var = fscanf(file_ptr_2, "%d/%d/%d|%f|%d|%d\n",
    &date_2[counter_2][0], &date_2[counter_2][1], &date_2[counter_2][2],
      &sleep_hours_2[counter_2], &moving_minutes,
        &working_minutes)) != EOF) {
    if (num_of_var < 6) {
      fclose(file_ptr_1);
      fclose(file_ptr_2);
      file_ptr_1 = NULL;
      file_ptr_2 = NULL;
      fclose(file_out);
      file_out = NULL;

      return BAD_RECORD;
    }

    if ((date_2[counter_2][0] < 1) || (date_2[counter_2][0] > 12) ||
      (date_2[counter_2][1] < 1) || (date_2[counter_2][1] > 31)) {
      fclose(file_ptr_1);
      fclose(file_ptr_2);
      file_ptr_1 = NULL;
      file_ptr_2 = NULL;
      fclose(file_out);
      file_out = NULL;

      return BAD_DATE;
    }
    counter_2++;
  }

  fclose(file_ptr_1);
  fclose(file_ptr_2);
  file_ptr_1 = NULL;
  file_ptr_2 = NULL;

  if ((counter_1 == 0) || (counter_2 == 0)) {
    fclose(file_out);
    file_out = NULL;

    return NO_DATA_POINTS;
  }

  if (counter_1 != counter_2) {
    fclose(file_out);
    file_out = NULL;

    return RECORDS_MISMATCH;
  }

  fprintf(file_out, "Name: %s\n", name_1);
  fprintf(file_out, "Name: %s\n", name_2);
  fprintf(file_out, "Month: %d, Year: %d\n", month, year);
  fprintf(file_out, "HOUR: 0 1 2 3 4 5 6 7 8 9 10\n");

  for (int i = 0; i < counter_1; i++) {
    if ((date[i][0] == month) && (date[i][2] == year)
      && (date_2[i][0] == month) && (date_2[i][2] == year)) {
      char sleep_string_1[ARRAY_LENGTH];

      if (sleep_hours_1[i] > 0) {

        for (int j = 0; j <= (int) (sleep_hours_1[i] * 2.0) + 1; j++) {
          if (j > 20) {
            sleep_string_1[j] = '\0';
            break;
          }
          if (j == 0) {
            sleep_string_1[j] = '|';
          } else if (j == (int) (sleep_hours_1[i] * 2.0) ) {
            sleep_string_1[j] = '|';
            sleep_string_1[j + 1] = '\0';
            break;
          } else {
            sleep_string_1[j] = '-';
          }
        }
      } else {
        sleep_string_1[0] = '|';
        sleep_string_1[1] = '\0';
      }
      sleep_string_1[strlen(sleep_string_1)] = '\0';

      char sleep_string_2[100];

      if (sleep_hours_2[i] > 0) {

        for (int j = 0; j <= (int) (sleep_hours_2[i] * 2.0) + 1; j++) {
          if (j > 20) {
            sleep_string_2[j] = '\0';
            break;
          }
          if (j == 0) {
            sleep_string_2[j] = '|';
          } else if (j == (int) (sleep_hours_2[i] * 2.0) ) {
            sleep_string_2[j] = '|';
            sleep_string_2[j + 1] = '\0';
            break;
          } else {
            sleep_string_2[j] = '-';
          }
        }
      } else {
        sleep_string_2[0] = '|';
        sleep_string_2[1] = '\0';
      }
      sleep_string_2[strlen(sleep_string_2)] = '\0';
      if (date[i][1] > 9) {
        fprintf(file_out, "%d%s%s\n", date[i][1], "    " , sleep_string_1);
      } else {
        fprintf(file_out, "%d%s%s\n", date[i][1], "     ", sleep_string_1);
      }

      fprintf(file_out, "%s%s\n", "      ", sleep_string_2);
    }
  }
  fprintf(file_out, "Average Sleep Hours of %s: %.2f hours\n",
    name_1, get_average_sleep_hours(in_file_1, year, month));
  fprintf(file_out, "Average Sleep Hours of %s: %.2f hours\n",
    name_2, get_average_sleep_hours(in_file_2, year, month));
  fclose(file_out);
  file_out = NULL;
  return 0;
} /* compare_sleep_hours() */

/*
 * This function takes in a file name,
 * month and year and return the
 * average calories burned per day
 * for the particular month and year
 */

float get_average_calories(char *file_name, int year, int month) {
  if ((month > 12) || (month < 1)) {
    return BAD_DATE;
  }

  FILE *file_ptr = 0;
  file_ptr = fopen(file_name, "r");

  if (file_ptr == NULL) {
    return FILE_READ_ERR;
  }

  char name[ARRAY_LENGTH];

  if (fscanf(file_ptr, "%[^\n]\n", name) == 0) {
    fclose(file_ptr);
    file_ptr = NULL;
    return BAD_RECORD;
  }

  if (strlen(name) > MAX_NAME_LEN) {
    fclose(file_ptr);
    file_ptr = 0;
    return BAD_RECORD;
  }

  int date[2000][3];
  int workout_minutes[2000];
  int moving_minutes[2000];
  float sleep = 0;
  int counter = 0;
  float calories_burned[2000];
  int count = 0;

  float total_calories = 0.0;
  int num_of_var = 0;
  while ((num_of_var = fscanf(file_ptr, "%d/%d/%d|%f|%d|%d\n",
    &date[counter][0], &date[counter][1], &date[counter][2],
      &sleep, &moving_minutes[counter], &workout_minutes[counter])) != EOF) {
    if (num_of_var < 6) {
      fclose(file_ptr);
      file_ptr = NULL;
      return BAD_RECORD;
    }

    if ((date[counter][0] > 12) || (date[counter][0] < 1)
      || (date[counter][1] > 31) || (date[counter][1] < 1))  {
      fclose(file_ptr);
      file_ptr = NULL;
      return BAD_DATE;
    }
    calories_burned[counter] = (2.5 * moving_minutes[counter]) +
      (11.3 * workout_minutes[counter]);
    if ((date[counter][0] == month) && (date[counter][2] == year)) {
      count++;
      total_calories += calories_burned[counter];
    }
    counter++;
  }

  fclose(file_ptr);
  file_ptr = 0;

  if (count == 0) {
    return NO_DATA_POINTS;
  }
  return (total_calories / count);
} /* get_average_calories() */

/*
 * This function compares the activity of
 * two people and writes it out to a file
 */

int compare_activity_log(char *in_file_1, char *in_file_2,
  int year, int month, char *out_file) {

  FILE *file_ptr_1 = NULL;
  file_ptr_1 = fopen(in_file_1, "r");

  if (file_ptr_1 == NULL) {
    return FILE_READ_ERR;
  }

  FILE *file_ptr_2 = NULL;

  file_ptr_2 = fopen(in_file_2, "r");

  if (file_ptr_2 == NULL) {
    fclose(file_ptr_1);
    file_ptr_1 = NULL;
    return FILE_READ_ERR;
  }

  FILE *file_out = NULL;
  file_out = fopen(out_file, "w");

  if (file_out == NULL) {
    fclose(file_ptr_1);
    fclose(file_ptr_2);
    file_ptr_1 = NULL;
    file_ptr_2 = NULL;
    return FILE_WRITE_ERR;
  }

  if ((month > 12) || (month < 1)) {
    fclose(file_ptr_1);
    fclose(file_ptr_2);
    file_ptr_1 = NULL;
    file_ptr_2 = NULL;
    fclose(file_out);
    file_out = NULL;
    return BAD_DATE;
  }

  char name_1[100];
  char name_2[100];

  if (fscanf(file_ptr_1, "%[^\n]\n", name_1) == 0) {
    fclose(file_ptr_1);
    fclose(file_ptr_2);
    file_ptr_1 = NULL;
    file_ptr_2 = NULL;
    fclose(file_out);
    file_out = NULL;
    return BAD_RECORD;
  }

  if (fscanf(file_ptr_2, "%[^\n]\n", name_2) == 0) {
    fclose(file_ptr_1);
    fclose(file_ptr_2);
    file_ptr_1 = NULL;
    file_ptr_2 = NULL;
    fclose(file_out);
    file_out = NULL;
    return BAD_RECORD;
  }

  int date_1[2000][3];
  int date_2[2000][3];

  float total_calories_1[2000];
  float total_calories_2[2000];


  float sleep_hour = 0;
  int moving_minutes = 0;
  int workout_minutes = 0;
  int counter_1 = 0;
  int num_of_var = 0;
  while ((num_of_var = fscanf(file_ptr_1, "%d/%d/%d|%f|%d|%d",
    &date_1[counter_1][0], &date_1[counter_1][1], &date_1[counter_1][2],
      &sleep_hour, &moving_minutes, &workout_minutes)) != EOF) {
    if (num_of_var < 6) {
      fclose(file_ptr_1);
      fclose(file_ptr_2);
      file_ptr_1 = NULL;
      file_ptr_2 = NULL;
      fclose(file_out);
      file_out = NULL;
      return BAD_RECORD;
    }
    if ((date_1[counter_1][0] > 12) || (date_1[counter_1][0] < 1)
      || (date_1[counter_1][1] > 31) || (date_1[counter_1][1] < 1)) {
      fclose(file_ptr_1);
      fclose(file_ptr_2);
      file_ptr_1 = NULL;
      file_ptr_2 = NULL;
      fclose(file_out);
      file_out = NULL;
      return BAD_DATE;
    }

    total_calories_1[counter_1] = (2.5 * moving_minutes) +
      (11.3 * workout_minutes);
    counter_1++;
  }
  int counter_2 = 0;
  while ((num_of_var = fscanf(file_ptr_2, "%d/%d/%d|%f|%d|%d",
    &date_2[counter_2][0], &date_2[counter_2][1], &date_2[counter_2][2],
      &sleep_hour, &moving_minutes, &workout_minutes)) != EOF) {
    if (num_of_var < 6) {
      fclose(file_ptr_1);
      fclose(file_ptr_2);
      file_ptr_1 = NULL;
      file_ptr_2 = NULL;
      fclose(file_out);
      file_out = NULL;
      return BAD_RECORD;
    }
    if ((date_2[counter_2][0] > 12) || (date_2[counter_2][0] < 1)
      || (date_2[counter_2][1] > 31) || (date_2[counter_2][1] < 1)) {
      fclose(file_ptr_1);
      fclose(file_ptr_2);
      file_ptr_1 = NULL;
      file_ptr_2 = NULL;
      fclose(file_out);
      file_out = NULL;
      return BAD_DATE;
    }

    total_calories_2[counter_2] = 2.5 * moving_minutes + 11.3 * workout_minutes;
    counter_2++;
  }

  if ((counter_1 == 0) || (counter_2 == 0)) {
    fclose(file_ptr_1);
    fclose(file_ptr_2);
    file_ptr_1 = NULL;
    file_ptr_2 = NULL;
    fclose(file_out);
    file_out = NULL;
    return NO_DATA_POINTS;
  }

  if (counter_1 != counter_2) {
    fclose(file_ptr_1);
    fclose(file_ptr_2);
    file_ptr_1 = NULL;
    file_ptr_2 = NULL;
    fclose(file_out);
    file_out = NULL;
    return RECORDS_MISMATCH;
  }

  fclose(file_ptr_1);
  fclose(file_ptr_2);
  file_ptr_1 = NULL;
  file_ptr_2 = NULL;

  fprintf(file_out, "Name: %s\n", name_1);
  fprintf(file_out, "Name: %s\n", name_2);
  fprintf(file_out, "Month: %d, Year: %d\n", month, year);

  for (int i = 0; i < counter_1; i++) {
    if ((date_1[i][0] == month) && (date_1[i][2] == year)
      && (date_2[i][0] == month) && (date_2[i][2] == year)) {
      if (date_1[i][1] > 9) {
        if (total_calories_1[i] > total_calories_2[i]) {
          fprintf(file_out, "%d: %s burned %.2f more calories.\n",
            date_1[i][1], name_1, total_calories_1[i] - total_calories_2[i]);
        } else if (total_calories_2[i] > total_calories_1[i]) {
          fprintf(file_out, "%d: %s burned %.2f more calories.\n",
            date_1[i][1], name_2, (total_calories_2[i] - total_calories_1[i]));
        } else {
          fprintf(file_out,
            "%d: %s and %s burned an equal amount of calories.\n",
              date_1[i][1], name_1, name_2);
        }
      } else {
        if (total_calories_1[i] > total_calories_2[i]) {
          fprintf(file_out, "%d : %s burned %.2f more calories.\n",
            date_1[i][1], name_1, total_calories_1[i] - total_calories_2[i]);
        } else if (total_calories_2[i] > total_calories_1[i]) {
          fprintf(file_out, "%d : %s burned %.2f more calories.\n",
          date_1[i][1], name_2, total_calories_2[i] - total_calories_1[i]);
        } else {
          fprintf(file_out,
            "%d : %s and %s burned an equal amount of calories.\n",
              date_1[i][1], name_1, name_2);
        }

      }
    }
  }

  fclose(file_out);
  file_out = NULL;

  return 0.0;
} /* compare_activity_log() */

/* Remember, you don't need a main function!
 * it is provided by hw2_main.c or hw2_test.o
 */
