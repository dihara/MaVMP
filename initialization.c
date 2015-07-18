/* 
 * imavmp.c: Memetic Algorithm for Many-Objective Virtual Machine Placement (MaVMP)
 * Date: 01-07-2015
 * Author: Diego Ihara (dihara@gmail.com)
 * Corresponding Conference Paper: Many-Objective Virtual Machine Placement for Dynamic Environments
 */

/* include initialization stage header */
#include "initialization.h"

/* initialization: initializes a population randomically
 * parameter: number of individuals
 * parameter: number of physical machines
 * parameter: number of virtual machines
 * parameter: virtual machines requirements matrix
 * returns: population matrix
 */
int** initialization(int** population_ref, int number_of_individuals, int h_size, int v_size, int **V, int max_SLA, int *selected_previous_placement, int previous_v_size)
{
	/* population: initial population matrix */
	//int **population = (int **) malloc (number_of_individuals *sizeof (int *));

	/* iterators */
	int iterator_individual;
	int iterator_individual_position;

	/* first individual is the selected placement from previous iteration */
    //population[0] = (int *) malloc (v_size *sizeof (int));

    /* iterate on positions of the first individual */
    for (iterator_individual_position = 0; iterator_individual_position < v_size; iterator_individual_position++)
    {
        /* copy the placements from the previous placement to the new individual */
        if (iterator_individual_position < previous_v_size) {
            /* only if the VM is still marked as on (CPU > 0) */
            if (V[iterator_individual_position][0] > 0) {
                population_ref[0][iterator_individual_position] = selected_previous_placement[iterator_individual_position];
            } else {
                population_ref[0][iterator_individual_position] = 0;
            }
        }
        /* generate random placements for the new VMs */
        else
        {
            /* Only when the vm is not marked as off (CPU > 0) it must be placed */
            if (V[iterator_individual_position][0] > 0) {

                /* individual with SLA != max_SLA */
                if (V[iterator_individual_position][3] != max_SLA)
                {
                    /* assign to each virtual machine a random generated physical machine */
                    population_ref[0][iterator_individual_position] = generate_solution_position(h_size, 0);
                }
                /* individual with SLA == max_SLA */
                if (V[iterator_individual_position][3] == max_SLA)
                {
                    /* assign to each virtual machine a random generated physical machine */
                    population_ref[0][iterator_individual_position] = generate_solution_position(h_size, 1);
                }
            } else {
                /* VM is off */
                population_ref[0][iterator_individual_position] = 0;
            }
        }
    }

	/* generate all other individuals (>0) */
	for (iterator_individual = 0 + 1; iterator_individual < number_of_individuals; iterator_individual++)
	{
	    //not necessary
		//population[iterator_individual] = (int *) malloc (v_size *sizeof (int));

		/* iterate on positions of an individual */
		for (iterator_individual_position = 0; iterator_individual_position < v_size; iterator_individual_position++)
		{
            /* Only when the vm is not marked as off (CPU > 0) it must be placed */
		    if (V[iterator_individual_position][0] > 0) {

                /* individual with SLA != max_SLA */
                if (V[iterator_individual_position][3] != max_SLA)
                {
                    /* assign to each virtual machine a random generated physical machine */
                    population_ref[iterator_individual][iterator_individual_position] = generate_solution_position(h_size, 0);
                }
                /* individual with SLA == max_SLA */
                if (V[iterator_individual_position][3] == max_SLA)
                {
                    /* assign to each virtual machine a random generated physical machine */
                    population_ref[iterator_individual][iterator_individual_position] = generate_solution_position(h_size, 1);
                }
			} else {
                /* VM is off */
                population_ref[iterator_individual][iterator_individual_position] = 0;
			}
		}
	}

	return population_ref;
}

/* generate_solution_position: generates a host number between 0 or 1 and parameter max_posible
 * parameter: maximum number for the random number to return
 * parameter: SLA of the individual. for 1 the placement is mandatory
 * returns: random number between 0 and parameter max_posible
 */
int generate_solution_position(int max_posible, int SLA)
{
	/* assign to each virtual machine a random generated physical machine from 0 to the maximum possible */
	if (SLA == 0)
	{
		/* integer from 0 to max_posible + 1*/
		//return rand() % (max_posible + 1);

		//int pos0 = ((float) rand() / (float) RAND_MAX) * (max_posible + 1);
		int pos0 = rand() % (max_posible + 1);

		return pos0;
	}
	/* assign to each virtual machine a random generated physical machine from 1 to the maximum possible */
	if (SLA == 1)
	{
		/* integer from 1 to max_posible */
		//return rand() % max_posible + 1;

		//int pos1 = ((float) rand() / (float) RAND_MAX) * (max_posible) + 1;
		int pos1 = (rand() % max_posible) + 1;

		return pos1;
	}
}
