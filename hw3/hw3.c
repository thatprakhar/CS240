/* Prakhar, hw3.c, CS 24000, Spring 2020
 * Last updated January 29, 2020
 */

/* Add any includes here */

#include "hw3.h"

#include <stdio.h>
#include <string.h>

#define SIZE (15)


int g_dealership_count = 0;
char g_dealerships[MAX_RECORDS][MAX_NAME_LEN];
char g_salespeople[MAX_RECORDS][MAX_NAME_LEN];
int g_prices[MAX_RECORDS][NUM_PRICE_COLS];
int g_sales[MAX_RECORDS][NUM_SALES_COLS];




/*
 * This method reads in the
 * values from the input file
 * and stores them in the
 * arrays
 */

int read_tables(char *file_name) {
  g_dealership_count = 0;


  FILE *file_ptr = NULL;
  file_ptr = fopen(file_name, "r");


  if (file_ptr == NULL) {
    g_dealership_count = 0;
    return FILE_READ_ERROR;
  }

  while (1) {


    int num_of_var = 0;
    char dealership_name[MAX_NAME_LEN];
    char salesperson[MAX_NAME_LEN];
    int sedan = 0;
    int suv = 0;
    int motorcycle = 0;
    int truck = 0;
    int sedan_sales = 0;
    int suv_sales = 0;
    int motorcycle_sales = 0;
    int truck_sales = 0;
    float commision = 0.0;


  // if name read is EOF and no date is read previously
    if (((num_of_var = fscanf(file_ptr, "%[^#]#", dealership_name)) == EOF)
    && (g_dealership_count == 0)) {
      fclose(file_ptr);
      file_ptr = NULL;
      g_dealership_count = 0;
      return NO_DATA_POINTS;
    }

    // if name read is EOF but data was read previously
    if (((num_of_var == EOF)
      || (num_of_var == 0))
      && (g_dealership_count > 0)) {
      break;
    }


    if (strlen(dealership_name) == 0) {
      fclose(file_ptr);
      file_ptr = NULL;
      g_dealership_count = 0;
      return NO_DATA_POINTS;
    }

    // if lenght is more
    if (strlen(dealership_name) > MAX_NAME_LEN) {
      fclose(file_ptr);
      file_ptr = NULL;
      return RECORD_ERROR;
    }

    // if malformed data
    if ((num_of_var = fscanf(file_ptr, "%[^#]#", salesperson)) == 0) {
      fclose(file_ptr);
      file_ptr = NULL;
      return RECORD_ERROR;
    }


    if (strlen(salesperson) == 0) {
      fclose(file_ptr);
      file_ptr = NULL;
      g_dealership_count = 0;
      return NO_DATA_POINTS;
    }


    if (strlen(salesperson) > MAX_NAME_LEN) {
      fclose(file_ptr);
      file_ptr = NULL;
      return RECORD_ERROR;
    }

    if ((num_of_var = fscanf(file_ptr, "%d#", &sedan)) == 0) {
      fclose(file_ptr);
      file_ptr = NULL;
      return RECORD_ERROR;
    }


    if ((num_of_var = fscanf(file_ptr, "%d#", &suv)) == 0) {
      fclose(file_ptr);
      file_ptr = NULL;
      return RECORD_ERROR;
    }

    if ((num_of_var = fscanf(file_ptr, "%d#", &truck)) == 0) {
      fclose(file_ptr);
      file_ptr = NULL;
      return RECORD_ERROR;
    }

    if ((num_of_var = fscanf(file_ptr, "%d#", &motorcycle)) == 0) {
      fclose(file_ptr);
      file_ptr = NULL;
      return RECORD_ERROR;
    }

    if ((num_of_var = fscanf(file_ptr, "%d#", &sedan_sales)) == 0) {
      fclose(file_ptr);
      file_ptr = NULL;
      return RECORD_ERROR;
    }

    if ((num_of_var = fscanf(file_ptr, "%d#", &suv_sales)) == 0) {
      fclose(file_ptr);
      file_ptr = NULL;
      return RECORD_ERROR;
    }

    if ((num_of_var = fscanf(file_ptr, "%d#", &truck_sales)) == 0) {
      fclose(file_ptr);
      file_ptr = NULL;
      return RECORD_ERROR;
    }

    if ((num_of_var = fscanf(file_ptr, "%d#", &motorcycle_sales)) == 0) {
      fclose(file_ptr);
      file_ptr = NULL;
      return RECORD_ERROR;
    }

    if ((num_of_var = fscanf(file_ptr, "%f\n", &commision)) == 0) {
      fclose(file_ptr);
      file_ptr = NULL;
      return RECORD_ERROR;
    }

    if (num_of_var == EOF) {
      break;
    }


    strcpy(g_dealerships[g_dealership_count], dealership_name);
    g_dealerships[g_dealership_count ][strlen(dealership_name)] = '\0';

    strcpy(g_salespeople[g_dealership_count], salesperson);
    g_salespeople[g_dealership_count][strlen(salesperson)] = '\0';

    g_prices[g_dealership_count][0] = sedan;
    g_prices[g_dealership_count][1] = suv;
    g_prices[g_dealership_count][2] = truck;
    g_prices[g_dealership_count][3] = motorcycle;
    g_sales[g_dealership_count][0] = sedan_sales;
    g_sales[g_dealership_count][1] = suv_sales;
    g_sales[g_dealership_count][2] = truck_sales;
    g_sales[g_dealership_count][3] = motorcycle_sales;
    g_sales[g_dealership_count][4] = (int) (commision * 100);

    g_dealership_count++;

    if (g_dealership_count > MAX_RECORDS) {
      fclose(file_ptr) ;
      file_ptr = NULL;
      return OUT_OF_BOUNDS;
    }
  }
  fclose(file_ptr);
  file_ptr = NULL;

  if (g_dealership_count == 0) {
    return NO_DATA_POINTS;
  }
  return g_dealership_count;
} /* read_tables() */

/*
 * This function writes
 * the total sales of
 * all dealerships to the
 * given output file
 */

int show_total_sales(char *out_file) {
  // file pointer
  FILE *file_out = NULL;
  file_out = fopen(out_file, "w");

  if (file_out == NULL) {
    return FILE_WRITE_ERROR;
  }

  // NO_data_points check
  if (g_dealership_count == 0) {
    fclose(file_out);
    file_out = NULL;
    return NO_DATA_POINTS;
  }

  // reading data and writing it
  for (int counter = 0; counter < g_dealership_count; counter++) {
    int total_sale = g_sales[counter][0] +
    g_sales[counter][1] + g_sales[counter][2] + g_sales[counter][3];
    fprintf(file_out, "%s: %d\n", g_dealerships[counter], total_sale);
  }


  fclose(file_out);
  file_out = NULL;

  return 0;
} /* show_total_sales() */


/*
 * This method writes out the
 * average prices to the given
 * output file
 */

int show_average_prices(char *out_file) {
  FILE *file_out = NULL;
  file_out = fopen(out_file, "w");

  if (file_out == NULL) {
    return FILE_WRITE_ERROR;
  }

  if (g_dealership_count == 0) {
    fclose(file_out);
    file_out = NULL;
    return NO_DATA_POINTS;
  }

  for (int counter = 0; counter < g_dealership_count; counter++) {
    float avg_price = g_prices[counter][0] +
      g_prices[counter][1] + g_prices[counter][2] + g_prices[counter][3];
    avg_price /= 4;
    fprintf(file_out, "%s: %.2f\n", g_dealerships[counter], avg_price);
  }


  fclose(file_out);
  file_out = NULL;
  return 0;

} /* show_average_prices() */

/*
 * This method calculates the total
 * revenue of the given dealership
 */

int calculate_revenue(char *dealership) {
  if (g_dealership_count == 0) {
    return NO_DATA_POINTS;
  }
  int index_of_dealership = -1;
  for (int i = 0; i < g_dealership_count; i++) {
    if (strcmp(g_dealerships[i], dealership) == 0) {
      index_of_dealership = i;
      break;
    }
  }

  if (index_of_dealership == -1) {
    return NO_SUCH_NAME;
  }

  int sedan_revenue = g_prices[index_of_dealership][0]
    * g_sales[index_of_dealership][0];
  int suv_revenue = g_prices[index_of_dealership][1]
    * g_sales[index_of_dealership][1];
  int truck_revenue = g_prices[index_of_dealership][2]
    * g_sales[index_of_dealership][2];
  int motorcycle_revenue = g_prices[index_of_dealership][3]
    * g_sales[index_of_dealership][3];

  int total_revenue = sedan_revenue + suv_revenue
    + truck_revenue + motorcycle_revenue;

  return total_revenue;
} /* calculate_revenue() */


/*
 * This method return the
 * salary of the given employee
 */

float employee_salary(char *salesperson) {
  if (g_dealership_count == 0) {
    return NO_DATA_POINTS;
  }

  int index_of_salesperson = -1;

  for (int i = 0; i < g_dealership_count; i++) {
    if (strcmp(g_salespeople[i], salesperson) == 0) {
      index_of_salesperson = i;
      break;
    }
  }

  if (index_of_salesperson == -1) {
    return NO_SUCH_NAME;
  }

  int total_revenue = calculate_revenue(g_dealerships[index_of_salesperson]);

  float salary =(float) total_revenue * g_sales[index_of_salesperson][4];

  salary /= 100;

  return salary;
} /* employee_salary() */


/*
 * This method return the maximum
 * salary among all employees
 */

float calculate_max_salary() {
  if (g_dealership_count == 0) {
    return NO_DATA_POINTS;
  }

  float max_salary = 0;

  for (int i = 0; i < g_dealership_count; i++) {
    float salary = employee_salary(g_salespeople[i]);
    printf("Salary of person at %d is %.2f\n", i, salary);
    if (salary > max_salary) {
      max_salary = salary;
    }
  }

  return max_salary;
} /* calculate_max_salary() */


/*
 * This method writes the most
 * common sales of all dealerships
 * to the given file
 */

int show_most_common_sale(char *out_file) {
  if (g_dealership_count == 0) {
    return NO_DATA_POINTS;
  }

  FILE *file_out = NULL;
  file_out = fopen(out_file, "w");
  if (file_out == NULL) {
    return FILE_WRITE_ERROR;
  }

  for (int i = 0; i < g_dealership_count; i++) {
    int count_of_max = 0;
    int max_sale = 0;
    int index_of_max_sale = -1;
    char type[SIZE];

    for (int j = 0; j < 4; j++) {
      if (g_sales[i][j] > max_sale) {
        index_of_max_sale = j;
        count_of_max = 0;
        max_sale = g_sales[i][j];
      }
      if (g_sales[i][j] == max_sale) {
        count_of_max++;
      }
    }
    if (index_of_max_sale == 0) {
      strcpy(type, "Sedan");
      type[strlen(type)] = '\0';
    } else if (index_of_max_sale == 1) {
      strcpy(type, "SUV");
      type[strlen(type)] = '\0';
    } else if (index_of_max_sale == 2) {
      strcpy(type, "Truck");
      type[strlen(type)] = '\0';
    } else {
      strcpy(type, "Motorcycle");
      type[strlen(type)] = '\0';
    }

    if (count_of_max > 1) {
      strcpy(type, "More than one");
      type[strlen(type)] = '\0';
    }

    fprintf(file_out, "%s: %s\n", g_dealerships[i], type);
  }
  fclose(file_out);
  file_out = NULL;

  return 0;
} /* show_most_common_sale() */


/*
 * This function writes the data
 * of the given table within the
 * given columns
 */

int write_tables(char *out_file, int table_index, int start_col , int end_col) {
  if (g_dealership_count == 0) {
    return NO_DATA_POINTS;
  }
  if ((table_index < 1) || (table_index > 4)) {
    return INVALID_TABLE_INDEX;
  }

  if (table_index == 1) {
    if ((start_col != 0) || (end_col != 0)) {
      return INVALID_COLUMN;
    }
    FILE *file_out = NULL;
    file_out = fopen(out_file, "w");

    if (file_out == NULL) {
      return FILE_WRITE_ERROR;
    }

    for (int i = 0; i < g_dealership_count; i++) {
      fprintf(file_out, "%s\n", g_dealerships[i]);
    }
    fclose(file_out);
    file_out = NULL;
  }

  if (table_index == 2) {
    if ((start_col != 0) || (end_col != 0)) {
      return INVALID_COLUMN;
    }
    FILE *file_out = NULL;
    file_out = fopen(out_file, "w");

    if (file_out == NULL) {
      return FILE_WRITE_ERROR;
    }

    for (int i = 0; i < g_dealership_count; i++) {
      fprintf(file_out, "%s\n", g_salespeople[i]);
    }
    fclose(file_out);
    file_out = NULL;
  }

  if (table_index == 3) {
    if ((start_col < 0)
      || (start_col > 3)
      || (end_col < 0)
      || (end_col > 3)
      || (end_col < start_col)) {
      return INVALID_COLUMN;
    }

    FILE *file_out = NULL;
    file_out = fopen(out_file, "w");

    if (file_out == NULL) {
      return FILE_WRITE_ERROR;
    }

    for (int i = 0; i < g_dealership_count; i++) {
      for (int j = start_col; j <= end_col; j++) {
        if (j != end_col) {
          fprintf(file_out, "%d,", g_prices[i][j]);
        } else {
          fprintf(file_out, "%d\n", g_prices[i][j]);
        }
      }
    }

    fclose(file_out);
    file_out = NULL;
  }

  if (table_index == 4) {
    if ((start_col < 0)
      || (start_col > 4)
      || (end_col < 0)
      || (end_col > 4)
      || (end_col < start_col)) {
      return INVALID_COLUMN;
    }

    FILE *file_out = NULL;
    file_out = fopen(out_file, "w");

    if (file_out == NULL) {
      return FILE_WRITE_ERROR;
    }

    for (int i = 0; i < g_dealership_count; i++) {
      for (int j = start_col; j <= end_col; j++) {
        if (j != end_col) {
          fprintf(file_out, "%d,", g_sales[i][j]);
        } else {
          fprintf(file_out, "%d\n", g_sales[i][j]);
        }
      }
    }
    fclose(file_out);
    file_out = NULL;
  }
  return 0;
} /* write_tables() */


/* Remember, you don't need a main function!
 * it is provided by hw3_main.c or hw3_test.o
 */
