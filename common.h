/* 
 * imavmp.c: Memetic Algorithm for Many-Objective Virtual Machine Placement (MaVMP)
 * Date: 01-07-2015
 * Author: Diego Ihara (dihara@gmail.com)
 * Corresponding Conference Paper: Many-Objective Virtual Machine Placement for Dynamic Environments

 */

/* include libraries */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
/* definitions */
#define H_HEADER "PHYSICAL MACHINES"
#define V_HEADER "VIRTUAL MACHINES"
#define T_HEADER "NETWORK TRAFFIC"
#define TAM_BUFFER BUFSIZ
#define CONSTANT 10000
#define BIG_COST 1000000
/* get the number of physical and virtual machines */
int get_h_size(char path_to_file[]);
int get_v_size(char path_to_file[], int v_set_number);
int get_t_count(char path_to_file[]);
/* print matrix and array */
void print_int_matrix(int ** matrix, int rows, int columns);
void print_float_matrix(float ** matrix, int rows, int columns);
void print_int_array(int * array, int columns);
void print_float_array(float * array, int columns);
/* load datacenter physical resources and virtual requirements */
int** load_H(int h_size, char path_to_file[]);
int** load_V(int v_size, char path_to_file[], int v_set_number);
/* load utilization of physical resources and costs of the considered objective functions */
int*** load_utilization(int ***utilization_ref, int **population, int **H, int **V, int number_of_individuals, int h_size, int v_size);
float** load_objectives(float **objectives_functions_ref, int **population, int ***utilization, int **H, int **V, int **T, int number_of_individuals, int h_size, int v_size, int *previous_placement);
int* select_placement(int selection_strategy, struct pareto_element *pareto_head, int v_size);
int* get_closest_to_origin(struct pareto_element *pareto_head, int v_size);
int* get_lexicographic_cost_order(struct pareto_element *pareto_head, int v_size, int *LEXICOGRAPHIC_ORDER);
int* get_preferred_solution(struct pareto_element *pareto_head, int v_size);
int* load_current_placement(char path_to_file[], int v_set_number);
int get_t_value(char path_to_file[]);
int load_previous_v_size(char path_to_file[], int v_set_number);
