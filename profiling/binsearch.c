#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int compare_ints(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}


// Function to perform linear search
void *find(int arr[], int size, int target) {
    return bsearch(&target,arr,size,sizeof(int),compare_ints);
}

int main(int argc, char *argv[]) {
    int vectorSize = 6000, searchCount = 10;
    int found = 0, notFound = 0;

    // Seed random number generator
    srand(time(NULL));

    if (argc > 1) {
	vectorSize = atoi(argv[1]);
    }

    if (argc > 2) {
	searchCount = atoi(argv[2]);
    }


    printf("Vector size:%d\n",vectorSize);
    printf("Search count:%d\n",searchCount);

    int *vector = (int *)malloc(vectorSize * sizeof(int));
    int *numbersToSearch = (int *)malloc(searchCount * sizeof(int));

    if (vector == NULL || numbersToSearch == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    for (int i = 0; i < vectorSize; i++) {
        vector[i] = rand() % 10000;
    }

    qsort(vector,vectorSize,sizeof(int),compare_ints);

    for (int i = 0; i < searchCount; i++) {
        numbersToSearch[i] = rand() % 10000;
    }

    for (int i = 0; i < searchCount; i++) {
        void* index = find(vector, vectorSize, numbersToSearch[i]);
        if (index != NULL)
            found++;
        else
            notFound++;
    }

    free(vector);
    free(numbersToSearch);

    printf("Found=%d\n",found);
    printf("Not found=%d\n",notFound);

    return 0;
}

