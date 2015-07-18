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
#define T_HEADER "NETWORK TRAFFIC"
#define L_HEADER "NETWORK TOPOLOGY"
#define C_HEADER "NETWORK LINK CAPACITY"
#define TAM_BUFFER BUFSIZ
/* get the number of network links */
int get_l_size(char path_to_file[]);
/* load datacenter network topology */
int** load_T(int v_size, char path_to_file[], int iterator_t);
int** load_G(int h_size, int l_size, char path_to_file[]);
int* load_K(int l_size, char path_to_file[]);
/* load utilization of network resources */
int** load_network_utilization(int **population, int **T, int number_of_individuals, int v_size);
