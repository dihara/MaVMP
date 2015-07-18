/* 
 * imavmp.c: Memetic Algorithm for Many-Objective Virtual Machine Placement (MaVMP)
 * Date: 01-07-2015
 * Author: Diego Ihara (dihara@gmail.com)
 * Corresponding Conference Paper: Many-Objective Virtual Machine Placement for Dynamic Environments
 */

/* include common header */


struct pareto_element
{
	int *solution;
	float *costs;
	int generation;
	struct pareto_element *prev;
	struct pareto_element *next;
};
#include "common.h"

/* get_h_size: returns the number of physical machines
 * parameter: path to the datacenter file
 * returns: number of physical machines
 */
int get_h_size(char path_to_file[])
{
	/* datacenter file to read from */
	FILE *datacenter_file;
	/* line readed from file */
	//char input_line[TAM_BUFFER];
	char input_line[TAM_BUFFER];
	/* number of physical machines */
	int h_size = 0;
	/* 1 if reading the physical machines block in the file */
	int reading_physical = 0;
 	/* open the file for reading */
	datacenter_file = fopen(path_to_file,"r");
    /* if it is opened ok, we continue */
	if (datacenter_file != NULL)
	{
		/* read until the end */
		while(!feof(datacenter_file))
		{
			/* get line per line */
			fgets(input_line, TAM_BUFFER, datacenter_file);
			/* if the line is equal to H_HEADER, we begin the physical machines block in the file */
			if (strstr(input_line,H_HEADER) != NULL)
			{
				reading_physical = 1;
			}
			/* if the line is equal to V_HEADER, we end the physical machines block in the file */
			if (strstr(input_line,V_HEADER) != NULL)
			{
				reading_physical = 0;
				break;
			}
			/* if it is the correct block in the file, it is not the header and it is not a blank line, we count */
			if (reading_physical == 1 && strstr(input_line,H_HEADER) == NULL && strcmp(input_line, "\n") != 0)
			{
				h_size++;
			}
		}
	}
	/* close the file */
	fclose(datacenter_file);
	/* return the value */
	return h_size;
}

/* get_t_count: returns the number of vm request sets to be considered
 * parameter: path to the datacenter file
 * returns: number of vm request sets to be considered
 */
int get_t_count(char path_to_file[])
{
	/* datacenter file to read from */
	FILE *datacenter_file;
	/* line readed from file */
	char input_line[TAM_BUFFER];
	/* vm request sets count */
	int t_count = 1;
 	/* open the file for reading */
	datacenter_file = fopen(path_to_file,"r");
    /* if it is opened ok, we continue */
	if (datacenter_file != NULL)
	{
		/* read until the end */
		while(!feof(datacenter_file))
		{
			/* get line per line */
			fgets(input_line, TAM_BUFFER, datacenter_file);

			/* if it is a blank line, we count */
			if (strcmp(input_line, "\n") == 0)
			{
				t_count++;
			}
		}
	}
	/* close the file */
	fclose(datacenter_file);
	/* return the value */
	return t_count;
}

/* get_v_size: returns the number of virtual machines
 * parameter: path to the datacenter file
 * returns: number of virtual machines
 */
int get_v_size(char path_to_file[], int v_set_number)
{
	/* datacenter file to read from */
	FILE *datacenter_file;
	/* line readed from file */
	char input_line[TAM_BUFFER];
	/* number of virtual machines */
	int v_size = 0;
	/* the current v set we are reading */
	int current_v_set = 0;
	/* 1 if reading the virtual machines block in the file */
	//int reading_virtual = 0;
 	/* open the file for reading */
	datacenter_file = fopen(path_to_file,"r");
    /* if it is opened ok, we continue */
	if (datacenter_file != NULL)
	{
		/* read until the end */
		while(!feof(datacenter_file))
		{
			/* get line per line */
			fgets(input_line, TAM_BUFFER, datacenter_file);
			/* if the line is blank, we are getting to another set */
			if (strcmp(input_line, "\n") == 0)
			{
				current_v_set++;
			}
            /* if the desired set is over, stop reading */
			if (current_v_set > v_set_number)
            {
                break;
            }
			/* if we are in the correct block in the file, it is not the header and it is not a blank line, we count */
			if ((current_v_set == v_set_number) && strcmp(input_line, "\n") != 0)
			{
				v_size++;
			}
		}
	}
	/* close the file */
	fclose(datacenter_file);
	/* return the value */
	return v_size;
}

/* print_int_matrix: prints on screen a integer matrix
 * parameter: matrix to print
 * parameter: number of individuals
 * parameter: number of virtual machines
 * returns: nothing, it's void
 */
void print_int_matrix(int **matrix, int rows, int columns)
{
	/* iterators */
	int iterator_row;
	int iterator_column;
	/* iterate on rows */
	for (iterator_row=0; iterator_row < rows; iterator_row++)
	{
		printf("[DEBUG] ROW %d:\t\t",iterator_row);
		/* iterate on columns */
		for (iterator_column = 0; iterator_column < columns; iterator_column++)
		{
			printf("%d ",matrix[iterator_row][iterator_column]);
		}
		printf("\n");
	}
}

/* print_float_matrix: prints on screen a float matrix
 * parameter: matrix to print
 * parameter: number of individuals
 * parameter: number of virtual machines
 * returns: nothing, it's void
 */
void print_float_matrix(float **matrix, int rows, int columns)
{
	/* iterators */
	int iterator_row;
	int iterator_column;
	/* iterate on rows */
	for (iterator_row=0; iterator_row < rows; iterator_row++)
	{
		printf("[DEBUG] ROW %d:\t",iterator_row);
		/* iterate on columns */
		for (iterator_column = 0; iterator_column < columns; iterator_column++)
		{
			printf("%g\t",matrix[iterator_row][iterator_column]);
		}
		printf("\n");
	}
}

/* print_int_array: prints on screen a int array
 * parameter: array to print
 * parameter: number of virtual machines
 * returns: nothing, it's void
 */
void print_int_array(int *array, int columns)
{
	/* iterators */
	int iterator_column;
	/* iterate on columns */
	for (iterator_column = 0; iterator_column < columns; iterator_column++)
	{
		//printf("[DEBUG] [%d]: %d\n",iterator_column,array[iterator_column]);
		printf("%d ", array[iterator_column]);
	}
	printf("\n");
}

/* print_float_array: prints on screen a float array
 * parameter: array to print
 * parameter: number of columns
 * returns: nothing, it's void
 */
void print_float_array(float *array, int columns)
{
	/* iterators */
	int iterator_column;
	/* iterate on columns */
	for (iterator_column = 0; iterator_column < columns; iterator_column++)
	{
		printf("[DEBUG] [%d]: %g\n",iterator_column,array[iterator_column]);
	}
}

/* load_H: load the values of H
 * parameter: number of physical machines
 * parameter: path to the datacenter file
 * returns: H matrix
 */
int** load_H(int h_size, char path_to_file[])
{
	/* datacenter file to read from */
	FILE *datacenter_file;
	/* line readed from file */
	char input_line[TAM_BUFFER];
	/* iterator */
	int iterator = 0;
	/* 1 if is reading the physical machines block in the file */
	int reading_physical = 0;
	/* memory reserve for h_size physical machines */
	int **H = (int **) malloc (h_size *sizeof (int *));
 	/* open the file for reading */
	datacenter_file = fopen(path_to_file,"r");
    /* if it is opened ok, we continue */
	if (datacenter_file != NULL)
	{
		/* read until the end */
		while(!feof(datacenter_file))
		{
			/* get line per line */
			fgets(input_line, TAM_BUFFER, datacenter_file);
			/* if the line is equal to H_HEADER, we begin the physical machines block in the file */
			if (strstr(input_line,H_HEADER) != NULL)
			{
				reading_physical = 1;
			}
			/* if the line is equal to V_HEADER, we end the physical machines block in the file */
			if (strstr(input_line,V_HEADER) != NULL)
			{
				reading_physical = 0;
				break;
			}
			/* if it's the correct block in the file, it is not the header and it is not a blank line, we count */
			if (reading_physical == 1 && strstr(input_line,H_HEADER) == NULL && strcmp(input_line, "\n") != 0)
			{
				/* reserve 4 columns for Processor, Memory, Storage and Power Consumption */
				H[iterator] = (int *) malloc (4 *sizeof (int));
				/* load on the matrix and increment iterator */
				sscanf(input_line,"%d %d %d %d\n",&H[iterator][0],&H[iterator][1],&H[iterator][2],&H[iterator][3]);
				iterator++;
			}
		}
	}
	fclose(datacenter_file);
	return H;
}

int get_t_value(char path_to_file[])
{
    FILE *placements_file;
	char input_line[100000];
    int t_value = 0;

	placements_file = fopen(path_to_file,"r");

    /* if it is opened ok, we continue */
	if (placements_file != NULL)
	{
		/* read until the end */
		while(fgets(input_line, 100000, placements_file) != NULL) {
            t_value++;
		}
	}

	return t_value;
}

int load_previous_v_size(char path_to_file[], int v_set_number)
{
    FILE *placements_file;
	char input_line[100000];
    int t_value = 1;
    int i;
    int v_size = 0;

	placements_file = fopen(path_to_file,"r");

    /* if it is opened ok, we continue */
	if (placements_file != NULL)
	{
        /* read until the end */
		while(fgets(input_line, 100000, placements_file) != NULL) {
            t_value++;

            if (t_value == v_set_number) {
                break;
            }
		}

		for (i=0;i<strlen(input_line);i++) {
            if (input_line[i]=='\t') {
                v_size++;
            }
		}

    }

	return v_size;
}

int* load_current_placement(char path_to_file[], int v_set_number)
{
    FILE *placements_file;
	char input_line[100000];
    int t_value = 0;
    int i;
    int v_size = 0;
    int *placement;
    char *token;

	placements_file = fopen(path_to_file,"r");

    /* if it is opened ok, we continue */
	if (placements_file != NULL)
	{
		/* read until the end */
		while(fgets(input_line, 100000, placements_file) != NULL) {
            t_value++;

            if (t_value == v_set_number) {
                break;
            }
		}

		for (i=0;i<strlen(input_line);i++) {
            if (input_line[i]=='\t')  {
                v_size++;
            }
		}

		placement = (int *) malloc(sizeof(int) * v_size);
        i = 0;

        token = strtok(input_line, "\t");
		while( token != NULL ) {
            sscanf(token, "%d", &placement[i]);

            i++;
            token = strtok(NULL, "\t");
		}
	}

	return placement;
}

/* load_V: load the values of V
 * parameter: number of virtual machines
 * parameter: path to the datacenter file
 * returns: V matrix
 */
int** load_V(int v_size, char path_to_file[], int v_set_number)
{
	/* datacenter file to read from */
	FILE *datacenter_file;
	/* line readed from file */
	char input_line[TAM_BUFFER];
	/* iterator */
	int iterator = 0;

	/* the current V set we are reading */
	int current_v_set = 0;

	/* memory reserve for v_size virtual machines */
	int **V = (int **) malloc (v_size *sizeof (int *));

 	/* open the file for reading */
	datacenter_file = fopen(path_to_file,"r");

    /* if it is opened ok, we continue */
	if (datacenter_file != NULL)
	{
		/* read until the end */
		while(!feof(datacenter_file))
		{
			/* get line per line */
			fgets(input_line, TAM_BUFFER, datacenter_file);
			/* if the line is blank, we are getting to another set */
			if (strcmp(input_line, "\n") == 0)
			{
				current_v_set++;
			}
            /* if the desired set is over, stop reading */
			if (current_v_set > v_set_number)
            {
                break;
            }
			/* if we are in the correct block in the file, it is not the header and it is not a blank line, we count */
			if ((current_v_set == v_set_number) && strcmp(input_line, "\n") != 0)
			{
				/* reserve 5 columns for Processor, Memory, Storage, SLA and Economical Revenue */
				V[iterator] = (int *) malloc (5 *sizeof (int));
				sscanf(input_line,"%d %d %d %d %d\n",&V[iterator][0],&V[iterator][1],&V[iterator][2],&V[iterator][3],&V[iterator][4]);
				iterator++;
			}
		}
	}

	return V;
}

/* load_utilization: loads the utilization of the physical machines of all the individuals
 * parameter: population matrix
 * parameter: physical machines matrix
 * parameter: virtual machines matrix
 * parameter: number of individuals
 * parameter: number of physical machines
 * parameter: number of virtual machines
 * returns: utilization tridimentional matrix
 */
int*** load_utilization(int ***utilization_ref, int **population, int **H, int **V, int number_of_individuals, int h_size, int v_size)
{
	/* iterators */
	int iterator_individual;
	int iterator_virtual;
	int iterator_physical;
	/* utilization holds the physical machines utilization of Processor, Memory and Storage of every individual */
	//int ***utilization = (int ***) malloc (number_of_individuals *sizeof (int **));
	/* iterate on individuals */
	for (iterator_individual=0; iterator_individual < number_of_individuals; iterator_individual++)
	{
		/* requirements matrix, holds the sum of virtual machines requirements for each physical machine */
		int **requirements = (int **) malloc (h_size *sizeof (int *));
		/* utilization holds the physical machines utilization of Processor, Memory and Storage of every individual */
		//utilization[iterator_individual] = (int **) malloc (h_size *sizeof (int *));
		for (iterator_physical=0; iterator_physical < h_size; iterator_physical++)
		{
			/* virtual machine requirements in Processor, Memory and Storage. Initialized to 0 */
			requirements[iterator_physical] = (int *) malloc (3 *sizeof (int));
 			requirements[iterator_physical][0] = requirements[iterator_physical][1] = requirements[iterator_physical][2] = 0;
			/* physical machine utilization of Processor, Memory and Storage. Initialized to 0 */
			//utilization[iterator_individual][iterator_physical] = (int *) malloc (3 *sizeof (int));
			utilization_ref[iterator_individual][iterator_physical][0] = utilization_ref[iterator_individual][iterator_physical][1] =
			utilization_ref[iterator_individual][iterator_physical][2] = 0;
		}
		/* iterate on positions of an individual */
		for (iterator_virtual = 0; iterator_virtual < v_size; iterator_virtual++)
		{
			/* if the virtual machine has a placement assigned */
			if (population[iterator_individual][iterator_virtual] != 0)
			{
				/* increment the requirements of the assigned physical machine with the virtual machine requirements
				of Processor, Memory and Storage */
				requirements[population[iterator_individual][iterator_virtual]-1][0] += V[iterator_virtual][0];
				requirements[population[iterator_individual][iterator_virtual]-1][1] += V[iterator_virtual][1];
				requirements[population[iterator_individual][iterator_virtual]-1][2] += V[iterator_virtual][2];
			}
		}
		/* iterate on positions of an individual */
		for (iterator_physical=0; iterator_physical < h_size; iterator_physical++)
		{
			/* virtual machine requirements in Processor, Memory and Storage. Initialized to 0 */
			utilization_ref[iterator_individual][iterator_physical][0] = requirements[iterator_physical][0];
			utilization_ref[iterator_individual][iterator_physical][1] = requirements[iterator_physical][1];
			utilization_ref[iterator_individual][iterator_physical][2] = requirements[iterator_physical][2];
		}
	}
	return utilization_ref;
}

/* load_objectives: calculate the cost of each objective of each solution
 * parameter: population matrix
 * parameter: physical machines matrix
 * parameter: virtual machines matrix
 * parameter: network traffic matrix
 * parameter: number of individuals
 * parameter: number of physical machines
 * parameter: number of virtual machines
 * returns: cost of each objetive matrix
 */
float** load_objectives(float **objectives_functions_ref, int **population, int ***utilization, int **H, int **V, int **T, int number_of_individuals, int h_size, int v_size, int *previous_placement)
{
	/* iterators */
	int iterator_individual;
	int iterator_virtual2;
	int iterator_virtual;
	int physical_position;
	int iterator_physical;

	float power_consumption;
	/* count the network traffic */
	int network_traffic;
	int count_traffic;
	/* utility of a physical machine */
	float utilidad;
	/* objectives_functions_ref holds the cost of each solution
	  objectives_functions_ref[INDIVIDUO][0] = power consumption; OF 1
	  objectives_functions_ref[INDIVIDUO][1] = network traffic; OF 2
	  objectives_functions_ref[INDIVIDUO][2] = revenue; OF 3
	  objectives_functions_ref[INDIVIDUO][3] = migration count; OF 4
	  objectives_functions_ref[INDIVIDUO][4] = migration overhead; OF 5
	*/

	//float **value_solution = (float **) malloc (number_of_individuals *sizeof (float *));
	//for (iterator_individual = 0 ; iterator_individual < number_of_individuals; iterator_individual++)
        /* Funciones Objetivo: Energy consumption, Traffic, Revenue, Migration impact, Migration cost */
		//value_solution[iterator_individual] = (float *) malloc (5 *sizeof (float));
	/* iterate on individuals */
	for (iterator_individual = 0; iterator_individual < number_of_individuals; iterator_individual++)
	{
		power_consumption = 0.0;
		objectives_functions_ref[iterator_individual][0] = objectives_functions_ref[iterator_individual][1] = objectives_functions_ref[iterator_individual][2] =
		objectives_functions_ref[iterator_individual][3] = objectives_functions_ref[iterator_individual][4] = 0.0;

		/* (OF1) calculate energy consumption of each solution*/
		/* iterate on physical machines */
		for (iterator_physical = 0 ; iterator_physical < h_size ; iterator_physical++)
		{
			if (utilization[iterator_individual][iterator_physical][0] > 0)
			{
				/* calculates utility of a physical machine */
				utilidad = (float)utilization[iterator_individual][iterator_physical][0] / H[iterator_physical][0];
				/* calculates energy consumption of a physical machine */
				power_consumption += ((float)H[iterator_physical][3] - ((float)H[iterator_physical][3]*0.6)) * utilidad +
				(float)H[iterator_physical][3]*0.6;
			}
		}
		/* loads energy consumption of each solution */
		objectives_functions_ref[iterator_individual][0] = power_consumption;

		/* (OF2) calculate network traffic between virtual machines */
		network_traffic = 0;
		/* iterate on virtual machines */
		for (iterator_virtual = 0; iterator_virtual < v_size; iterator_virtual++)
		{
			physical_position = population[iterator_individual][iterator_virtual];
			/* count the network traffic */
			count_traffic = 0;
			if (physical_position != 0)
			{
				for (iterator_virtual2 = 0 ; iterator_virtual2 < v_size ; iterator_virtual2++)
				{
					if (physical_position == population[iterator_individual][iterator_virtual2] ||
						population[iterator_individual][iterator_virtual2] == 0)
					{
						count_traffic = 0;
					}
					else
					{
						count_traffic = T[iterator_virtual][iterator_virtual2];
					}
					/* calculates network traffic between all virtual machines of each solution */
					network_traffic = network_traffic + count_traffic;
				}

                /* (OF3) calculate revenue of each solution */
                /* (OF4) count migrations required */
                /* (OF5) sum memory of VMs to be migrated */

				/* loads the revenue of each solution */
				objectives_functions_ref[iterator_individual][2] += (float) V[iterator_virtual][4];

                if ((previous_placement[iterator_virtual] > 0) && (physical_position != previous_placement[iterator_virtual]))
                {
                    objectives_functions_ref[iterator_individual][3]++;

                    objectives_functions_ref[iterator_individual][4] += V[iterator_virtual][1];
                }
			}
		}
		/* loads the network traffic of each solution */
		objectives_functions_ref[iterator_individual][1] = (float)network_traffic;
	}
	return objectives_functions_ref;
}

int* select_placement(int selection_strategy, struct pareto_element *pareto_head, int v_size)
{
    int *selected_placement;
    int i;
    int lex_order_revenue_first[5] = {2, 0, 1, 4, 3}; //first 2 = revenue, then 0 = energy, then 1 = traffic, then 4 = overhead, then 3 = migrations
    int lex_order_migration_overhead_first[5] = {3, 4, 1, 2, 0};

    switch (selection_strategy) {

    /* random from pareto front */
    case 0:
        printf("selecting random\n");
        selected_placement = (int *) malloc (v_size *sizeof(int));

        /* copy the first solution */
        for (i = 0; i < v_size; i++)
        {
            selected_placement[i] = pareto_head->solution[i];
        };
    break;

    /* preferred solution, random */
    case 1:
        printf("selecting preferred solution\n");
        /* how many are best in 4, 3, 2... then random */
        selected_placement = get_preferred_solution(pareto_head, v_size);
    break;

    /* closest to origin */
    case 2:
        printf("selecting closest to origin\n");
        /* calculate distance of every solution, choose nearest or random of nearest */
        selected_placement = get_closest_to_origin(pareto_head, v_size);
    break;

    /* lexicographic order energy consumption, traffic, revenue, migration cost, migration impact */
    case 3:
        printf("selecting revenue first\n");
        /* order 1 energy, revenue, mcost, mimpact, traffic */
        selected_placement = get_lexicographic_cost_order(pareto_head, v_size, lex_order_revenue_first);
    break;
    case 4:
        printf("selecting migration first\n");
        selected_placement = get_lexicographic_cost_order(pareto_head, v_size, lex_order_migration_overhead_first);
        /* order 3  revenue, energy, mcost, mimpact, traffic */
    break;
    case 5:
        /* order 3  mcost, mimpact, revenue, energy, traffic */
    break;
    }

    return selected_placement;
}

int* get_lexicographic_cost_order(struct pareto_element *pareto_head, int v_size, int *LEXICOGRAPHIC_ORDER)
{
    int* selected_placement;
    float costs_sum[5] = {0,0,0,0,0};
    struct pareto_element *ptr1;
    int i;
    int cost_order_iterator;

    selected_placement = (int *) malloc (v_size *sizeof(int));

    //select the first solution
    for (i = 0; i < 5; i++) {
        costs_sum[i] = pareto_head->costs[i];
    }
    for (i = 0; i < v_size; i++) {
        selected_placement[i] = pareto_head->solution[i];
    }

    //compare selected solution with all the others
    ptr1 = pareto_head->next;
	while(ptr1 != NULL)
    {
        cost_order_iterator = 0;
        while (cost_order_iterator < 5) {
            //if we are comparing revenue, then more is better
            if ((LEXICOGRAPHIC_ORDER[cost_order_iterator] == 2) && (ptr1->costs[LEXICOGRAPHIC_ORDER[cost_order_iterator]] > costs_sum[LEXICOGRAPHIC_ORDER[cost_order_iterator]]))
            {
                for (i = 0; i < 5; i++) {
                    costs_sum[i] = ptr1->costs[i];
                }
                for (i = 0; i < v_size; i++) {
                    selected_placement[i] = ptr1->solution[i];
                }
                break;
            }
            //if it is better, select the solution and continue comparing with the next solution
            else if ((LEXICOGRAPHIC_ORDER[cost_order_iterator] != 2) && (ptr1->costs[LEXICOGRAPHIC_ORDER[cost_order_iterator]] < costs_sum[LEXICOGRAPHIC_ORDER[cost_order_iterator]]))
            {
                for (i = 0; i < 5; i++) {
                    costs_sum[i] = ptr1->costs[i];
                }
                for (i = 0; i < v_size; i++) {
                    selected_placement[i] = ptr1->solution[i];
                }
                break;
            }
            //if it is equal on the first, continue comparing with the second one
            else if (ptr1->costs[LEXICOGRAPHIC_ORDER[cost_order_iterator]] == costs_sum[LEXICOGRAPHIC_ORDER[cost_order_iterator]])
            {
                cost_order_iterator++;
            } else {
                //if it is less, then continue with the next solution
                break;
            }
        }

        ptr1 = ptr1->next;
    }

    return selected_placement;
}

int* get_preferred_solution(struct pareto_element *pareto_head, int v_size)
{
    int* preferred_placement;
    preferred_placement = (int *) malloc (v_size *sizeof(int));

    /* One loop to count how many solutions have the max number of better parameters */
    int i = 0;
    int individual_comparison;
    int win_count;
    int max_win_count = 0;

    struct pareto_element *ptr1;
    struct pareto_element *ptr2;

	ptr1 = pareto_head;
	while(ptr1 != NULL)
    {
        win_count = 0;

        ptr2 = pareto_head;
        while(ptr2 != NULL){
            individual_comparison = 0;

            /* if ptr2 solution consumes less energy */
            if (ptr1->costs[0] < ptr2->costs[0]) {
                individual_comparison++;
            } else if (ptr1->costs[0] > ptr2->costs[0]) {
                individual_comparison--;
            }
            if (ptr1->costs[1] < ptr2->costs[1]) {
                individual_comparison++;
            } else if (ptr1->costs[1] > ptr2->costs[1]) {
                individual_comparison--;
            }
            /* if ptr2 solution receives more revenue */
            if (ptr1->costs[2] > ptr2->costs[2]) {
                individual_comparison++;
            } else if (ptr1->costs[2] < ptr2->costs[2]) {
                individual_comparison--;
            }
            /* if ptr2 solution implies less migrations */
            if (ptr1->costs[3] < ptr2->costs[3]) {
                individual_comparison++;
            } else if (ptr1->costs[3] > ptr2->costs[3]) {
                individual_comparison--;
            }
            if (ptr1->costs[4] < ptr2->costs[4]) {
                individual_comparison++;
            } else if (ptr1->costs[4] > ptr2->costs[4]) {
                individual_comparison--;
            }

            /* if more objective functions of ptr1 are better than ptr2,
               then the variable individual_comparison will have a positive value */
            if (individual_comparison > 0) {
                win_count++;
            }

            ptr2 = ptr2->next;
        }

        /* if we find that ptr1 is not beaten by any other
           solution, then ptr1 is the preferred solution */
        if (win_count > max_win_count) {
            max_win_count = win_count;

            /* copy selected solution */
            for (i = 0; i < v_size; i++) {
                preferred_placement[i] = ptr1->solution[i];
            }
        }

        ptr1 = ptr1->next;
    }

    /* the list was randomly generated, so the last one with max quantity of best parameters is accepted as random */
    return preferred_placement;
}

int* get_closest_to_origin(struct pareto_element *pareto_head, int v_size)
{
    int* selected_individual;
    selected_individual = (int *) malloc (v_size *sizeof(int));

    /* One loop to count how many solutions have the max number of better parameters */
    int i = 0;
    double shortest_distance = -1;
    double individual_distance;
    double costs_sum[5] = {0,0,0,0,0};
    struct pareto_element *ptr1;

    /* find the max costs in this pareto set */
	ptr1 = pareto_head;
	while(ptr1 != NULL)
    {
        for (i = 0; i < 5; i++) {
            //if (max_costs[i] < ptr1->costs[i]) {
            //    max_costs[i] = ptr1->costs[i];
           //}
           costs_sum[i] += ptr1->costs[i];
        }
        ptr1 = ptr1->next;
    }

    /* calculate distance to origin of each solution in pareto set */
	ptr1 = pareto_head;
	while(ptr1 != NULL)
    {
        /* euclidean distance of this individual, normalized by dividing (cost)/(max cost) */
        individual_distance = sqrt( pow(ptr1->costs[0]/costs_sum[0], 2) +
                                    pow(ptr1->costs[1]/costs_sum[1], 2) +
                                    pow(ptr1->costs[2]/costs_sum[2], 2) +
                                    pow(ptr1->costs[3]/costs_sum[3], 2) +
                                    pow(ptr1->costs[4]/costs_sum[4], 2) );

        /* if this individual is closest to origin, or this is the first iteration */
        if ((individual_distance < shortest_distance) || (shortest_distance == -1)) {
            /* new best */
            shortest_distance = individual_distance;
            /* copy selected solution */
            for (i = 0; i < v_size; i++) {
                selected_individual[i] = ptr1->solution[i];
            }
        }

        ptr1 = ptr1->next;
    }

    return selected_individual;
}
