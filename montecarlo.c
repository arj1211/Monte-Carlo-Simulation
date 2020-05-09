#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static unsigned long int iterations = 0;
static unsigned long int count = 0;
static unsigned int num_child_processes = 1;

unsigned long int montecarlo(unsigned long int iterations, unsigned int seed) {
    unsigned long int i, c = 0;
    double x, y, z;
    
    for (i = 0; i < iterations; ++i) {
        x = (double)rand_r(&seed)/RAND_MAX;
        y = (double)rand_r(&seed)/RAND_MAX;
        z = x*x + y*y;
        if (z <= 1.0) {
            ++c;
        }
    }
    return c;
}


int main(int argc, char *argv[]) {
    // Command line options for seed and iterations
    int c;
    if (argc == 1) {
        printf("Usage: %s -i <iterations> -p <number_of_child_processes>\n", argv[0]);
        return -1;
    }
    
    while ((c = getopt (argc, argv, "i:p:")) != -1) {
        switch (c) {
        case 'i':
            iterations = strtoul(optarg, NULL, 10);
            break;
        case 'p':
            num_child_processes = strtoul(optarg, NULL, 10);
            if (num_child_processes == 0) {
                printf("%s: number of child processes must be greater than 0 -- 'p'\n", argv[0]);
                return EXIT_FAILURE;
            }
            break;
        default:
            return EXIT_FAILURE;
        }
    }

    // Check that iterations is present and valid
    if (iterations == 0) {
        printf("%s: the number of iterations must be greater than 0 -- 'i'\n", argv[0]);
        return EXIT_FAILURE;
    }
   
    /*  Write your code here to create child processes to run the simulation
        and collect the results */
    

    double pi = (double)count/iterations*4;
    printf("%f\n", pi);

    return EXIT_SUCCESS;
}
