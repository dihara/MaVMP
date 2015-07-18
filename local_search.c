/* 
 * imavmp.c: Memetic Algorithm for Many-Objective Virtual Machine Placement (MaVMP)
 * Date: 01-07-2015
 * Author: Diego Ihara (dihara@gmail.com)
 * Corresponding Conference Paper: Many-Objective Virtual Machine Placement for Dynamic Environments
 */


/* include local search stage header */
#include "local_search.h"

/* local_search: local optimization of the population
 * parameter: population matrix
 * parameter: utilization matrix of the physical machines of all the individuals
 * parameter: physical machines matrix
 * parameter: virtual machines matrix
 * parameter: number of individuals
 * parameter: number of physical machines
 * parameter: number of virtual machines
 * returns: optimized population matrix
 */
int** local_search(int **population, int ***utilization, int **H, int **V, int number_of_individuals, int h_size, int v_size)
{
	/* iterators */
	int iterator_individual = 0;
	int physical_position = 0;
	int physical_position2 = 0;
	int iterator_position = 0;
	int iterator_virtual = 0;
	int iterator_virtual2 = 0;
	int iterator_physical = 0;
	int option = 0;
	/* randon value */
	float val_rand;
	/* value to executed */
	int option_to_execute;

	//val_rand = drand48() *1.0; WINDOWS
	//Numero aleatorio entre 0 y 1
	val_rand = (float) rand() / (float) RAND_MAX;

	if (val_rand >= 0 && val_rand <= 0.5)
			option_to_execute = 0;
	else
			option_to_execute = 3;

	while(option < 2)
	{
		/* if val_rand is equal to or less than 5 is executed 1 and 2, otherwise is executed 2 and 1 */
		if (val_rand >= 0 && val_rand <= 0.5)
		{
			option_to_execute++;
			option++;
		} else if (val_rand > 0.5 && val_rand <= 1.0)
		{
			option_to_execute--;
			option++;
		}
		/* comes of shut down physical machines to the migrate in other virtual machines */
		if (option_to_execute == 1)
		{
			/* iterate on individuals */
			for (iterator_individual = 0 ; iterator_individual < number_of_individuals ; iterator_individual++)
			{
				/* iterate on virtual machines */
				for (iterator_virtual = 0 ; iterator_virtual < v_size ; iterator_virtual++)
				{
					/* get the position of the physical machine */
					physical_position = population[iterator_individual][iterator_virtual];

					if (physical_position > 0)
					{
						/* iterate on virtual machines */
						for (iterator_virtual2 = 0 ; iterator_virtual2 < v_size ; iterator_virtual2++)
						{
							/* get the position of the physical machine */
							physical_position2 = population[iterator_individual][iterator_virtual2];

							if (physical_position2 > 0)
							{
								if (physical_position != physical_position2)
								{
									/* If the use of the VM not exceeds the capacity of the physical machine performs the migration */
									if (utilization[iterator_individual][physical_position-1][0] + V[iterator_virtual2][0] < H[physical_position-1][0]
									    && utilization[iterator_individual][physical_position-1][1] + V[iterator_virtual2][1] < H[physical_position-1][1]
									    && 	utilization[iterator_individual][physical_position-1][2] + V[iterator_virtual2][2] < H[physical_position-1][2])
									{
										/* refresh the utilization */
										utilization[iterator_individual][physical_position2-1][0] -= V[iterator_virtual2][0];
										utilization[iterator_individual][physical_position2-1][1] -= V[iterator_virtual2][1];
										utilization[iterator_individual][physical_position2-1][2] -= V[iterator_virtual2][2];

										utilization[iterator_individual][physical_position-1][0] += V[iterator_virtual2][0];
										utilization[iterator_individual][physical_position-1][1] += V[iterator_virtual2][1];
										utilization[iterator_individual][physical_position-1][2] += V[iterator_virtual2][2];
										/* refresh the population */
										population[iterator_individual][iterator_virtual2] = population[iterator_individual][iterator_virtual];

										break;
									}
								}
							}
						}
					}
				}
			}
		}
		/*  try to place the virtual machines turned off in the physical machines turned on */
		if (option_to_execute == 2)
		{
			/* iterate on individuals */
			for (iterator_individual = 0 ; iterator_individual < number_of_individuals ; iterator_individual++)
			{
				/* iterate on virtual machines */
				for (iterator_virtual = 0 ; iterator_virtual < v_size ; iterator_virtual++)
				{
					/* get the position of the physical machine */
					physical_position = population[iterator_individual][iterator_virtual];

                    /* If the vm was not placed and the vm is not marked as off (i.e. CPU == 0) */
					if ((physical_position == 0) && (V[iterator_virtual][0] > 0))
					{
						/* iterate on virtual machines */
						for (iterator_virtual2 = 0 ; iterator_virtual2 < v_size ; iterator_virtual2++)
						{
							/* get the position of the physical machine */
							physical_position2 = population[iterator_individual][iterator_virtual2];

							if (physical_position2 > 0)
							{
								/* If the use of the VM not exceeds the capacity of the physical machine performs the migration */
								if (utilization[iterator_individual][physical_position2-1][0] + V[iterator_virtual][0] < H[physical_position2-1][0]
								    && utilization[iterator_individual][physical_position2-1][1] + V[iterator_virtual][1] < H[physical_position2-1][1]
								    && 	utilization[iterator_individual][physical_position2-1][2] + V[iterator_virtual][2] < H[physical_position2-1][2])
								{
									/* refresh the utilization */
									utilization[iterator_individual][physical_position2-1][0] += V[iterator_virtual][0];
									utilization[iterator_individual][physical_position2-1][1] += V[iterator_virtual][1];
									utilization[iterator_individual][physical_position2-1][2] += V[iterator_virtual][2];
									/* refresh the population */
									population[iterator_individual][iterator_virtual] = population[iterator_individual][iterator_virtual2];

									iterator_virtual2 = v_size;

									break;
								}
							}
						}
					}
				}
			}
		}
	}

	return population;
}
