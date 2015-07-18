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
/* include arai headers */
#include "pareto.h"
#include "common.h"
#include "network.h"

/* structure of an element */
struct pareto_element
{
	int *solution;
	float *costs;
	int generation;
	struct pareto_element *prev;
	struct pareto_element *next;
};

struct pareto_element * pareto_create(int v_size, int *individual, float *objectives_functions, int generation)
{
	struct pareto_element *pareto_current = (struct pareto_element *) malloc (sizeof(struct pareto_element));
	pareto_current->solution = (int *) malloc (v_size *sizeof(int));
	pareto_current->costs = (float *) malloc (5 *sizeof(float));

	//pareto_current->solution = individual;
	//pareto_current->costs = objectives_functions;

    pareto_current->generation = generation;

    int i;
	for (i = 0; i<v_size ; i++) {
        pareto_current->solution[i] = individual[i];
    }

    for (i = 0; i<5; i++){
        pareto_current->costs[i] = objectives_functions[i];
    }

	pareto_current->prev = NULL;
	pareto_current->next = NULL;
	return pareto_current;
}

struct pareto_element * pareto_insert(struct pareto_element *pareto_head, int v_size, int *individual, float *objectives_functions, int generation)
{
	struct pareto_element *ptr1;
	ptr1 = pareto_head;

	if (pareto_head == NULL)
	{
	    pareto_head = pareto_create(v_size, individual, objectives_functions, generation);
		return pareto_head;
	}
	else
	{
		int iterator_virtual;
		int duplicate;
		/* is it duplicate? */
		while(ptr1 != NULL)
		{
		    duplicate = 1;
			for (iterator_virtual = 0 ; iterator_virtual < v_size ; iterator_virtual++)
			{
		   		if(ptr1->solution[iterator_virtual] != individual[iterator_virtual])
		   		{
					duplicate = 0;
					break;
				}
			}
			/* If I find a duplicate, break the loop; else test the next individual. */
			if (duplicate == 1) {
                break;
			}

			ptr1 = ptr1->next;
		}
		/* is it NOT duplicate */
        if(duplicate == 0) {
            struct pareto_element *pareto_current = pareto_create(v_size, individual, objectives_functions, generation);

            /* add to the end of the list */
//            ptr1 = pareto_head;
//      		while (ptr1 != NULL) {
//                if (ptr1->next != NULL) {
//                    ptr1 = ptr1->next;
//                } else {
//                    break;
//                }
//      		}
//            ptr1->next = pareto_current;
//            pareto_current->prev = ptr1;

            pareto_head->prev = pareto_current;
            pareto_current->next = pareto_head;
            pareto_head = pareto_current;
		}

		return pareto_head;
	}
}

struct pareto_element * pareto_remove_duplicates(struct pareto_element *pareto_head, int v_size)
{
	int pareto_size = load_pareto_size(pareto_head);
    struct pareto_element *ptr1, *tmp, *new_head;
    int iterator_individual;
    int i;
    float **objective_functions_best_P = (float **) malloc (pareto_size *sizeof (float *));

    ptr1 = pareto_head;
	for (iterator_individual=0; iterator_individual < pareto_size; iterator_individual++)
	{
		objective_functions_best_P[iterator_individual] = (float *) malloc (5 *sizeof (float));
		for (i=0; i<5; i++) {
            objective_functions_best_P[iterator_individual][i] = ptr1->costs[i];
		}
		ptr1 = ptr1->next;
	}

    int *fronts = non_dominated_sorting(objective_functions_best_P, pareto_size);

    new_head = pareto_head;
    ptr1 = pareto_head;
    for (iterator_individual = 0 ; iterator_individual < pareto_size ; iterator_individual++)
    {
        if(fronts[iterator_individual] == 1)
        {
            ptr1 = ptr1->next;
        } else {
            //if not first element
            if (ptr1->prev != NULL) {
                //if last element
                if (ptr1->next == NULL) {
                    tmp = ptr1;

                    ptr1->prev->next = NULL;

                    free(tmp->costs);
                    free(tmp->solution);
                } else {
                    tmp = ptr1;

                    ptr1 = tmp->next;

                    tmp->prev->next = ptr1;

                    ptr1->prev = tmp->prev;

                    free(tmp->costs);
                    free(tmp->solution);
                }
            } else {
                tmp = ptr1;
                ptr1 = tmp->next;

                ptr1->prev = NULL;

                free(tmp->costs);
                free(tmp->solution);

                new_head = ptr1;
            }
        }
    }

	for (iterator_individual=0; iterator_individual < pareto_size; iterator_individual++)
	{
		free(objective_functions_best_P[iterator_individual]);
	}
	free(objective_functions_best_P);
	free(fronts);

	return new_head;
}

/* get_min_cost: calculate the min value of an objective function in a population
 * parameter: array of the values of the objective functions
 * parameter: objective functions id for evaluation
 * parameter: number of individuals
 * parameter: non-dominated sorting fronts
 * returns: min cost of the identified objetive function
 */
float get_min_cost(struct pareto_element *pareto_head, int objective)
{
	float min_cost = BIG_COST;
	struct pareto_element *ptr1;
	ptr1 = pareto_head;
	/* Pick elements one by one */
	while(ptr1 != NULL)
	{
		if(ptr1->costs[objective] < min_cost)
		{
			min_cost = ptr1->costs[objective];
		}
		ptr1 = ptr1->next;
	}
	return min_cost;
}

/* get_max_cost: calculate the max value of an objective function in a population
 * parameter: array of the values of the objective functions
 * parameter: objective functions id for evaluation
 * parameter: number of individuals
 * parameter: non-dominated sorting fronts
 * returns: max cost of the identified objetive function
 */
float get_max_cost(struct pareto_element *pareto_head, int objective)
{
	float max_cost = -1.0;
	struct pareto_element *ptr1;
	ptr1 = pareto_head;
	/* Pick elements one by one */
	while(ptr1 != NULL)
	{
		if(ptr1->costs[objective] > max_cost)
		{
			max_cost = ptr1->costs[objective];
		}
		ptr1 = ptr1->next;
	}
	return max_cost;
}

void print_pareto_set(struct pareto_element *pareto_head, int v_size)
{
	int iterator_virtual;
	while (pareto_head != NULL)
	{
		/* iterate on columns */
		for (iterator_virtual = 0; iterator_virtual < v_size; iterator_virtual++)
		{
			printf("%d\t",pareto_head->solution[iterator_virtual]);
		}
		printf("\n");
		pareto_head = pareto_head->next;
	}
}

void print_pareto_front(struct pareto_element *pareto_head)
{
	while (pareto_head != NULL)
	{
		printf("%g\t%g\t%g\t%g\t%g\t",pareto_head->costs[0],pareto_head->costs[1],pareto_head->costs[2],pareto_head->costs[3],pareto_head->costs[4]);
		printf("\n");
		pareto_head = pareto_head->next;
	}
}

int load_pareto_size(struct pareto_element *pareto_head)
{
	struct pareto_element *ptr1 = pareto_head;
	int size = 0;
	while(ptr1 != NULL)
	{
		size++;
		ptr1 = ptr1->next;
	}
	return size;
}

void report_best_population(char *argv[], struct pareto_element *pareto_head, int **T, int **H, int **V, int v_size, int h_size, int *selected_placement, int iterator_t)
{
    int i;
    int is_selected_solution;
	int pareto_size = load_pareto_size(pareto_head);
	int **best_P = (int **) malloc (pareto_size *sizeof (int *));
	float **objective_functions_best_P = (float **) malloc (pareto_size *sizeof (float *));
	int *generation = malloc (pareto_size * sizeof (int));

	double objective_functions_sum[5] = {0, 0, 0, 0, 0};
	double objective_functions_std[5] = {0, 0, 0, 0, 0};
	int pareto_front_count = 0;

	FILE *pareto_set;
	FILE *pareto_front;
	//FILE *pareto_network_utilization;
	FILE *pareto_cpu_utilization;
	FILE *pareto_ram_utilization;
	FILE *pareto_hdd_utilization;

	FILE *of_sum_results;
	FILE *of_avg_results;
	FILE *of_std_results;

	FILE *P_file;

    const char* pareto_front_file_name = "\\results\\pareto_front";
    const char* pareto_set_file_name = "\\results\\pareto_set";
    const char* pareto_cpu_file_name = "\\results\\pareto_cpu_utilization";
    const char* pareto_ram_file_name = "\\results\\pareto_ram_utilization";
    const char* pareto_hdd_file_name = "\\results\\pareto_hdd_utilization";

    const char* of_sum_file_name = "\\results\\of_sum";
    const char* of_avg_file_name = "\\results\\of_avg";
    const char* of_std_file_name = "\\results\\of_std";

    const char* placements_file_name = "\\P";

    char* pareto_front_file;
    char* pareto_set_file;
    //char* pareto_network_file;
    char* pareto_cpu_file;
    char* pareto_ram_file;
    char* pareto_hdd_file;

    char* of_sum_file;
    char* of_avg_file;
    char* of_std_file;

    char* P_file_name;

    pareto_front_file = malloc(strlen(argv[1]) + strlen(pareto_front_file_name) + 1 + 1);
    strcpy(pareto_front_file, argv[1]);
    strcat(pareto_front_file, pareto_front_file_name);
    strcat(pareto_front_file, argv[3]);

    pareto_set_file = malloc(strlen(argv[1]) + strlen(pareto_set_file_name) + 1 + 1);
    strcpy(pareto_set_file, argv[1]);
    strcat(pareto_set_file, pareto_set_file_name);
    strcat(pareto_set_file, argv[3]);

    pareto_cpu_file = malloc(strlen(argv[1]) + strlen(pareto_cpu_file_name) + 1 + 1);
    strcpy(pareto_cpu_file, argv[1]);
    strcat(pareto_cpu_file, pareto_cpu_file_name);
    strcat(pareto_cpu_file, argv[3]);

    pareto_ram_file = malloc(strlen(argv[1]) + strlen(pareto_ram_file_name) + 1 + 1);
    strcpy(pareto_ram_file, argv[1]);
    strcat(pareto_ram_file, pareto_ram_file_name);
    strcat(pareto_ram_file, argv[3]);

    pareto_hdd_file = malloc(strlen(argv[1]) + strlen(pareto_hdd_file_name) + 1 + 1);
    strcpy(pareto_hdd_file, argv[1]);
    strcat(pareto_hdd_file, pareto_hdd_file_name);
    strcat(pareto_hdd_file, argv[3]);


    of_sum_file = malloc(strlen(argv[1]) + strlen(of_sum_file_name) + 1);
    strcpy(of_sum_file, argv[1]);
    strcat(of_sum_file, of_sum_file_name);

    of_avg_file = malloc(strlen(argv[1]) + strlen(of_avg_file_name) + 1);
    strcpy(of_avg_file, argv[1]);
    strcat(of_avg_file, of_avg_file_name);

    of_std_file = malloc(strlen(argv[1]) + strlen(of_std_file_name) + 1);
    strcpy(of_std_file, argv[1]);
    strcat(of_std_file, of_std_file_name);

    P_file_name = malloc(strlen(argv[1]) + 3 + 1);
    strcpy(P_file_name, argv[1]);
    strcat(P_file_name, placements_file_name);
    strcat(P_file_name, argv[3]);

	pareto_front = fopen(pareto_front_file,"a");
	pareto_set = fopen(pareto_set_file,"a");
	pareto_cpu_utilization = fopen(pareto_cpu_file,"a");
	pareto_ram_utilization = fopen(pareto_ram_file,"a");
	pareto_hdd_utilization = fopen(pareto_hdd_file,"a");

    //of_sum_results = fopen(of_sum_file,"a");
    //of_avg_results = fopen(of_avg_file,"a");
    //of_std_results = fopen(of_std_file,"a");

    P_file = fopen(P_file_name, "a");
    if (iterator_t > 0) fprintf(P_file,"\n");

	int iterator_individual;
	int iterator_virtual;
	int iterator_link;
	int iterator_physical;
	/* copying pareto individuals */
	for (iterator_individual=0; iterator_individual < pareto_size; iterator_individual++)
	{
		best_P[iterator_individual] = (int *) malloc (v_size *sizeof (int));
		objective_functions_best_P[iterator_individual] = (float *) malloc (5 *sizeof (float));
		best_P[iterator_individual] = pareto_head->solution; //OJO TODO
		objective_functions_best_P[iterator_individual] = pareto_head->costs;
		generation[iterator_individual] = pareto_head->generation;

		pareto_head = pareto_head->next;//latest generation first, earlier last
	}

	int ***utilization_best_P = (int ***) malloc (pareto_size *sizeof (int **));
    for (iterator_individual=0;iterator_individual < pareto_size; iterator_individual++)
    {
        utilization_best_P[iterator_individual] = (int **) malloc (h_size *sizeof (int *));

        for (iterator_physical=0;iterator_physical<h_size;iterator_physical++) {
            utilization_best_P[iterator_individual][iterator_physical] = (int *) malloc (3 *sizeof (int));
        }
    }

	/* once the best population is on a matrix, non-dominated sorting is performed */
	int *fronts_best_P = (int *) non_dominated_sorting(objective_functions_best_P, pareto_size);
	load_utilization(utilization_best_P, best_P, H, V, pareto_size, h_size, v_size);

	/* save the Pareto set and the Pareto front */
	/* for each individual */
	for (iterator_individual=0; iterator_individual < pareto_size; iterator_individual++)
	{
		/* only the first pareto front */
		if(fronts_best_P[iterator_individual]==1 )
		{
            is_selected_solution = 1;
            if (iterator_t == 0) {
                    fprintf(pareto_set,"#");
                    fprintf(pareto_front,"#");
                    fprintf(pareto_cpu_utilization,"#");
                    fprintf(pareto_ram_utilization,"#");
                    fprintf(pareto_hdd_utilization,"#");
            }

			for (iterator_virtual = 0; iterator_virtual < v_size ; iterator_virtual++)
			{
			    if (best_P[iterator_individual][iterator_virtual] != selected_placement[iterator_virtual]) {
                    is_selected_solution = 0;
			    }
			}

			/* Pareto set */
			for (iterator_virtual = 0; iterator_virtual < v_size ; iterator_virtual++)
			{
			    if (is_selected_solution == 1) {
                    fprintf(P_file,"%d\t",best_P[iterator_individual][iterator_virtual]);
			    }
				/* for each element of the individual/solution */
				fprintf(pareto_set,"%d\t",best_P[iterator_individual][iterator_virtual]);
			}
			if (is_selected_solution == 1) {
                fprintf(pareto_set, " ***");
			}
			fprintf(pareto_set,"\n");

            if (is_selected_solution == 1) {
                /* Pareto front */
                fprintf(pareto_front,"%g\t%g\t%g\t%g\t%g\t %i ***\n", objective_functions_best_P[iterator_individual][0],
                                                                      objective_functions_best_P[iterator_individual][1],
                                                                      objective_functions_best_P[iterator_individual][2],
                                                                      objective_functions_best_P[iterator_individual][3],
                                                                      objective_functions_best_P[iterator_individual][4],
                                                                      generation[iterator_individual]);
			} else {
                /* Pareto front */
                fprintf(pareto_front,"%g\t%g\t%g\t%g\t%g\t %i \n", objective_functions_best_P[iterator_individual][0],
                                                                   objective_functions_best_P[iterator_individual][1],
                                                                   objective_functions_best_P[iterator_individual][2],
                                                                   objective_functions_best_P[iterator_individual][3],
                                                                   objective_functions_best_P[iterator_individual][4],
                                                                   generation[iterator_individual]);
			}

			/* physical resources utilization */
			for (iterator_physical = 0; iterator_physical < h_size ; iterator_physical++)
			{
				/* for each physical resource */
				fprintf(pareto_cpu_utilization,"%d\t",utilization_best_P[iterator_individual][iterator_physical][0]);
				fprintf(pareto_ram_utilization,"%d\t",utilization_best_P[iterator_individual][iterator_physical][1]);
				fprintf(pareto_hdd_utilization,"%d\t",utilization_best_P[iterator_individual][iterator_physical][2]);
			}
            if (is_selected_solution == 1)
            {
                fprintf(pareto_cpu_utilization, " ***");
                fprintf(pareto_ram_utilization, " ***");
                fprintf(pareto_hdd_utilization, " ***");
			}
			fprintf(pareto_cpu_utilization,"\n");
			fprintf(pareto_ram_utilization,"\n");
			fprintf(pareto_hdd_utilization,"\n");

            objective_functions_sum[0] += objective_functions_best_P[iterator_individual][0];
            objective_functions_sum[1] += objective_functions_best_P[iterator_individual][1];
            objective_functions_sum[2] += objective_functions_best_P[iterator_individual][2];
            objective_functions_sum[3] += objective_functions_best_P[iterator_individual][3];
            objective_functions_sum[4] += objective_functions_best_P[iterator_individual][4];

			pareto_front_count++;
		}
	}
    fprintf(pareto_set,"\n");
    fprintf(pareto_front,"\n");
    fprintf(pareto_cpu_utilization,"\n");
    fprintf(pareto_ram_utilization,"\n");
    fprintf(pareto_hdd_utilization,"\n");

    /* print sum of objective functions */
//    fprintf(of_sum_results, "%f\t%f\t%f\t%f\t%f\n", objective_functions_sum[0], objective_functions_sum[1], objective_functions_sum[2], objective_functions_sum[3], objective_functions_sum[4]);

    /* print average of objective functions */
//    fprintf(of_avg_results, "%f\t%f\t%f\t%f\t%f\n", objective_functions_sum[0] / pareto_front_count,
//                                                    objective_functions_sum[1] / pareto_front_count,
//                                                    objective_functions_sum[2] / pareto_front_count,
//                                                    objective_functions_sum[3] / pareto_front_count,
//                                                    objective_functions_sum[4] / pareto_front_count);

//	for (iterator_individual=0; iterator_individual < pareto_size; iterator_individual++)
//	{
//		if(fronts_best_P[iterator_individual]==1 )
//		{
//            objective_functions_std[0] += pow(objective_functions_best_P[iterator_individual][0] - objective_functions_sum[0] / pareto_front_count, 2);
//            objective_functions_std[1] += pow(objective_functions_best_P[iterator_individual][1] - objective_functions_sum[1] / pareto_front_count, 2);
//            objective_functions_std[2] += pow(objective_functions_best_P[iterator_individual][2] - objective_functions_sum[2] / pareto_front_count, 2);
//            objective_functions_std[3] += pow(objective_functions_best_P[iterator_individual][3] - objective_functions_sum[3] / pareto_front_count, 2);
//            objective_functions_std[4] += pow(objective_functions_best_P[iterator_individual][4] - objective_functions_sum[4] / pareto_front_count, 2);
//		}
//	}
//
//	objective_functions_std[0] /= pareto_front_count;
//	objective_functions_std[1] /= pareto_front_count;
//	objective_functions_std[2] /= pareto_front_count;
//	objective_functions_std[3] /= pareto_front_count;
//	objective_functions_std[4] /= pareto_front_count;

    /* print standard deviation of objective functions */
//    fprintf(of_std_results, "%f\t%f\t%f\t%f\t%f\n", sqrt(objective_functions_sum[0]),
//                                                    sqrt(objective_functions_sum[1]),
//                                                    sqrt(objective_functions_sum[2]),
//                                                    sqrt(objective_functions_sum[3]),
//                                                    sqrt(objective_functions_sum[4]));

	fclose(pareto_set);
	fclose(pareto_front);
	fclose(pareto_cpu_utilization);
	fclose(pareto_hdd_utilization);
	fclose(pareto_ram_utilization);

	//fclose(of_sum_results);
	//fclose(of_avg_results);
	//fclose(of_std_results);
	return;
}
