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

/* function headers definitions */
int** reparation(int ** population, int *** utilization, int ** H, int ** V, int number_of_individuals, int h_size, int v_size, int max_SLA);
void repair_population(int ** population, int *** utilization, int ** H, int ** V, int number_of_individuals, int h_size, int v_size, int max_SLA);
void repair_individual(int ** population, int *** utilization, int ** H, int ** V, int number_of_individuals, int h_size, int v_size, int max_SLA, int individual);
int is_overloaded(int **H, int ***utilization, int individual, int physical);
