//SSOO-P3 23/24

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stddef.h>
#include <sys/stat.h>
#include <pthread.h>
#include "queue.h"
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>



// struct for the operation data thats in the ARRAY
// typedef struct Operation {
//     int product_id;
//     char operation_type[9]; // PURCHASE or SALE
//     int units;
// } Operation;

// put the producer args in a struct to reduce complexity
typedef struct {
    queue *q;
    element *operations;
    int start_proc;
    int end_proc;
} ProducerArgs;


// the func exec by each 
void *producer_thread(void *args) {
    ProducerArgs *arguments = (ProducerArgs *)args;
    queue *q = arguments->q;
    element *operations = arguments->operations;
    int start_proc = arguments->start_proc;
    int end_proc = arguments->end_proc;

    for (int i = start_proc; i < end_proc; i++) {
        queue_put(q, &operations[i]);
    }
    
    
    return NULL;
}




int main (int argc, const char * argv[])
{
    // int profits = 0;
    // int product_stock [5] = {0};
    // int product_profit [5] = {1,5,5,15,25};

    if (argc != 5) {
        fprintf(stderr, "Usage: %s <file name> <num producers> <num consumers> <buff size>\n", argv[0]);
        return 1;
    }

    // get the command line arguments
    const char *file_name = argv[1];
    int num_producers = atoi(argv[2]);
    // int num_consumers = atoi(argv[3]);
    int buff_size = atoi(argv[4]);

    // creating the queue of designated size
    queue *q = queue_init(buff_size);


    // Open the file
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    // gets the num of operations to read from the file
    int num_operations;
    if (fscanf(file, "%d", &num_operations) != 1) {
        fprintf(stderr, "Error reading number of operations from file\n");
        fclose(file);
        return 1;
    }

    // ************************************************************ //
    // should there be a check for the number of operations?
    // ************************************************************ //

    // creates an array of num_operations containing struct operations
    element *operations = malloc(num_operations * sizeof(element));
    if (operations == NULL) {
        perror("Error allocating memory for operations");
        fclose(file);
        return 1;
    }


    // reads the operations from file and stores them in the operations array
    for (int i = 0; i < num_operations; i++) {
        if (fscanf(file, "%d %8s %d", &operations[i].product_id, operations[i].op, &operations[i].units) != 3) {
            fprintf(stderr, "Error reading operation %d from file\n", i + 1);
            free(operations);
            fclose(file);
            return 1;
        }
    }

    // each producer will do num_operations/num_producers(floor division), 
    // but last producer will do num_ops - the floor division number
    int ops_per_prod_thread = num_operations / num_producers;


    // Create arguments for each producer thread
    ProducerArgs producer_args[num_producers];
    pthread_t producer_threads[num_producers];


    // execute the producers threads
    for (int i = 0; i < num_producers; i++) {
        // start idx for each prod thread
        int start_idx = i * ops_per_prod_thread;
        // end idx for each prod thread and incls the last thread to handle to the end
        // 2nd ternary logic, just uses the supposed next startidx as the end
        int end_idx = (i == num_producers - 1) ? num_operations : (i + 1) * ops_per_prod_thread;
        
        ProducerArgs args = {q, operations, start_idx, end_idx};
        producer_thread(&args);

    }


    // // making sure that elements are added to our queue with the producer func
    // struct element *elem = queue_get(q);
    // printf("Element: product_id = %d, Operation Type: %s, units = %d\n", elem->product_id, elem->op ,elem->units);
    // struct element *elem1 = queue_get(q);
    // printf("Element: product_id = %d, units = %d\n", elem1->product_id, elem1->units);
    // struct element *elem2 = queue_get(q);
    // printf("Element: product_id = %d, units = %d\n", elem2->product_id, elem2->units);





    
    

    





    

    // Output
    // printf("Total: %d euros\n", profits);
    printf("Stock:\n");
    // printf("  Product 1: %d\n", product_stock[0]);
    // printf("  Product 2: %d\n", product_stock[1]);
    // printf("  Product 3: %d\n", product_stock[2]);
    // printf("  Product 4: %d\n", product_stock[3]);
    // printf("  Product 5: %d\n", product_stock[4]);

    return 0;
}