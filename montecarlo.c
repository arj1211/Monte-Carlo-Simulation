#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>

static unsigned long int iterations = 0;
static unsigned long int count = 0;
static unsigned int num_child_processes = 1;

struct msg
{
    long mtype;
    unsigned long int data;
};

unsigned long int montecarlo(unsigned long int iterations, unsigned int seed)
{
    unsigned long int i, c = 0;
    double x, y, z;
    for (i = 0; i < iterations; ++i)
    {
        x = (double)rand_r(&seed) / RAND_MAX;
        y = (double)rand_r(&seed) / RAND_MAX;
        z = x * x + y * y;
        if (z <= 1.0)
            ++c;
    }
    return c;
}

unsigned int seedgen()
{
    struct timeval ct;
    gettimeofday(&ct, NULL);
    return (ct.tv_sec + ct.tv_usec) % ((unsigned long)(1e10));
}

int main(int argc, char *argv[])
{
    // Command line options for no of child processes and iterations
    int c;
    if (argc == 1)
    {
        printf("Usage: %s -i <iterations> -p <number_of_child_processes>\n", argv[0]);
        return -1;
    }

    while ((c = getopt(argc, argv, "i:p:")) != -1)
    {
        switch (c)
        {
        case 'i':
            iterations = strtoul(optarg, NULL, 10);
            break;
        case 'p':
            num_child_processes = strtoul(optarg, NULL, 10);
            if (num_child_processes == 0)
            {
                printf("%s: number of child processes must be greater than 0 -- 'p'\n", argv[0]);
                return EXIT_FAILURE;
            }
            break;
        default:
            return EXIT_FAILURE;
        }
    }

    // Check that iterations is present and valid
    if (iterations == 0)
    {
        printf("%s: the number of iterations must be greater than 0 -- 'i'\n", argv[0]);
        return EXIT_FAILURE;
    }

    /*  Write your code here to create child processes to run the simulation
        and collect the results */
    /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~START~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
    /* Create msgqueue */
    int msgqid = msgget(IPC_PRIVATE, 0666 | IPC_CREAT);
    struct msg montemsg;
    /* Create all but one of the child processes (last one deals with 
        the remainder iterations) */
    for (unsigned int i = 0; i < num_child_processes - 1; i++)
    {
        pid_t pid = fork();
        if (pid < 0)
        {
            /* Parent process; fork failed */
        }
        else if (pid == 0)
        {
            /* Child process */
            /* Calculate no of iters this child is responsible for */
            unsigned long int child_iters = iterations / (num_child_processes - 1);
            count = montecarlo(child_iters, seedgen());
            /* Set up and send msg to msgqueue */
            montemsg.mtype = 7;
            montemsg.data = count;
            msgsnd(msgqid, &montemsg, sizeof(struct msg), 0);
            /* End child's life */
            exit(0);
        }
        else
        { /* Parent process, making more children */
        }
    }
    /* How many iterations are remaining to be done? (because of int division) */
    unsigned long int rem_iters =
        iterations - (iterations / (num_child_processes - 1)) * (num_child_processes - 1);
    /* Dispatch one child to take care of remaining iterations */
    pid_t pid = fork();
    if (pid < 0)
    {
        /* Parent process; fork failed */
    }
    else if (pid == 0)
    {
        /* Child process */
        /* Child responsible for rem_iters iterations */
        count = montecarlo(rem_iters, seedgen());
        /* Set up and send msg to msgqueue */
        montemsg.mtype = 7;
        montemsg.data = count;
        msgsnd(msgqid, &montemsg, sizeof(struct msg), 0);
        /* End child's life */
        exit(0);
    }
    /* Parent process is the only one remaining now */
    /* while (wait(NULL) > 0); */
    /* Reap all the children... */
    for (unsigned int i = 0; i < num_child_processes; i++)
    {
        wait(NULL);
        msgrcv(msgqid, &montemsg, sizeof(struct msg), 7, 0);
        count += montemsg.data;
    }
    msgctl(msgqid, IPC_RMID, NULL);
    double pi = (double)count / iterations * 4;
    printf("%f\n", pi);
    return EXIT_SUCCESS;
}
