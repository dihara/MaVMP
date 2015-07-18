/* 
 * imavmp.c: Memetic Algorithm for Many-Objective Virtual Machine Placement (MaVMP)
 * Date: 01-07-2015
 * Author: Diego Ihara (dihara@gmail.com)
 * Corresponding Conference Paper: Many-Objective Virtual Machine Placement for Dynamic Environments
 */


/* include functions */
struct pareto_element * pareto_create(int v_size, int *individual, float *objectives_functions, int generation);
struct pareto_element * pareto_insert(struct pareto_element *pareto_head, int v_size, int *individual, float *objectives_functions, int generation);
struct pareto_element * pareto_remove_duplicates(struct pareto_element *pareto_head, int v_size);
float get_min_cost(struct pareto_element *pareto_head, int objective);
float get_max_cost(struct pareto_element *pareto_head, int objective);
void print_pareto_set(struct pareto_element *pareto_head, int v_size);
void print_pareto_front(struct pareto_element *pareto_head);
int load_pareto_size(struct pareto_element *pareto_head);
void report_best_population(char *argv[], struct pareto_element *pareto_head, int **T, int **H, int **V, int v_size, int h_size, int *selected_placement, int iterator_t);
