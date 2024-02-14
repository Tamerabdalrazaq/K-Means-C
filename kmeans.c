#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <setjmp.h>

#define EPSILON 0.01
#define ITER 200

struct Cluster
{
    double *centroid;
    int size;
};
typedef struct Cluster Cluster;

void *createArray(int n, int size)
{
    void *array = malloc(n * size);
    if (array == NULL)
    {
        printf("Memory allocation failed!\n");
        exit(1); // Exit program if memory allocation fails
    }
    return array;
}

// void **create2DArray(int n, int d, int size)
// {
//     double **list[n][d];
//     for (int i = 0; i < 5; i++)
//     {
//         for (int j = 0; j < 8; j++)
//         {
//             list[i][j] = (double **)malloc(sizeof(double *));
//             if (list[i][j] == NULL)
//             {
//                 perror("Memory allocation failed");
//                 return 1; // return error
//             }
//         }
//     }
//     return list;
// }

// double **readXconv(char *data, int n, int d)
// {
//     double **f_num = create2DArray(n, d, sizeof(double));
//     int j, i = 0;
//     FILE *file;
//     int ch;
//     file = fopen(data, "r");
//     while ((ch = getchar()) != EOF)
//     {
//         if (ch != '\n')
//         {
//             f_num[i][j] = strtod(ch, NULL);
//             j++;
//         }
//         else
//         {
//             i++;
//         }
//     }
//     fclose(file);
//     return f_num;
// }

int arrayLength(void *arr)
{
    return (sizeof(arr) / sizeof(arr[0]));
}

double **sub_matrix_k(double **matrix, int k, int d)
{
    double **sub_array;
    sub_array = malloc(k * sizeof(double *));

    for (int i = 0; i < k; i++)
    {
        printf("Sub_matrix_k \n");
        sub_array[i] = (double *)createArray(d, sizeof(double));
        for (int j = 0; j < d; j++){
            sub_array[i][j] = matrix[i][j];
            printf("%lf ",sub_array[i][j]);
        }

    }
    printf("out\n");
    return sub_array;
}

void free_matrix(double **matrix, int k)
{
    for (int i = 0; i < k; i++)
        free(matrix[i]);
    free(matrix);
}

void free_clusters(Cluster *clusters)
{
    if (clusters != NULL)
    {
        free(clusters->centroid);
        free(clusters);
    }
}

// int main(int argc, char *argv[]){
//     if (setjmp(exception_buffer) != 0) {
//         printf("An Error Has Occurred\n");
//     }
//     else{
//     int K= atoi((const char * argv[1]);
//     int n= atoi((const char * argv[2]);
//     int d= atoi((const char * argv[3]);
//     if (argc==6){
//         int iter = atoi((const char * argv[4]);
//         double data[n][d] = readXcon(argv[5]);
//     }
//     else{
//         int iter = ITER
//         double data[n][d] = readXcon(argv[4]);
//     }
//     double output[k][d] = k_means(K,N,d,iter,data);

//     //add error messages
//     for (int i = 0; i < k; i++) {
//         for (int j = 0; j < d; j++) {
//             printf("%.4f ", array[i][j]);
//         }
//         printf("\n")
//             }
//     return 0;

//     // argc returns the count of arguments passed by the user including the program name
//     // argv is the array of strings after splitting the command line into arguments, including the program name

//     }
//     return 0;
// }


double euc_l2(double *v1, double *v2, int d)
{
    printf("Calculating L2 between: \n");
    printArray(v1, d);
    printArray(v2, d);
    double dist = 0.0;
    for (int i = 0; i < d; i++){
        printf("d_i: %d\n", v1[i] - v2[i]);
        dist += pow(v1[i] - v2[i], 2.0);
    }
    printf("Dist is: sqrt(%d): %d End;\n",dist, sqrt(dist));
    return sqrt(dist);
}


/*
double euc_l2(double *v1, double *v2, int size) {
    double distance = 0.0;
    // Calculate the sum of squared differences for each corresponding element
    for (int i = 0; i < size; i++) {
        double diff = v1[i] - v2[i];
        distance += diff * diff;
    }
    return sqrt(distance);
}
*/

int find_closest_centroid_index(double **centroids, double* v, int k, int d)
{
    printf("find_closest_centroid_index for vector: \n");
    printArray(v, d);
    double min_dist = INFINITY;
    int index;
    printf("Current Centroids: \n");
    for (int i = 0; i < k; i++)
    {
        printf("Distance from centroid:\n");
        printArray(centroids[i], d);
        double dist = euc_l2(v, centroids[i], d);
        printf("is %d\n", dist);
        if (dist < min_dist)
        {
            printf("updated index.\n");
            index = i;
            min_dist = dist;
        }
    }
    printf("End;\n");
    return index;
}

double *calc_centroid_average(Cluster cluster, int d)
{
    double *centroid = cluster.centroid;
    int size = cluster.size;
    double *averaged_vector = calloc(d, sizeof(double));
    printf("Calculating average of the centroid with size %d:\n", size);
    printArray(centroid, d);
    if(size == 0){
        printf("Average of centroid is: \n");
        printArray(averaged_vector, d);
        return averaged_vector;
    }
    for (int i = 0; i < d; i++)
    {
        averaged_vector[i] = (double) centroid[i] / size;
    }
    printf("Average of centroid is: \n", size);
    printArray(averaged_vector, d);

    return averaged_vector;
}

int check_centroid_convergence(double **centroids, double **new_centroids, int k, int d)
{
    int convergent_centroids = 0;
    for (int i = 0; i < k; i++)
    {
        double *centroid_old = centroids[i];
        double *centroid_new = new_centroids[i];
        if (euc_l2(centroid_old, centroid_new, d) <= EPSILON)
            convergent_centroids += 1;
    }
    return (convergent_centroids == k);
}

void add_vector_to_centroid(Cluster* clus, double vec[], int d)
{
    for (int i = 0; i < d ; i++)
    {
        double updated_entry_i = clus->centroid[i] + vec[i];
        clus->centroid[i] = updated_entry_i;
    }
    clus->size = clus->size + 1;
}

void printArray(double *arr, int size) {
    for (int i = 0; i < size; i++) {
        printf("%f ", (arr[i]));
    }
    printf("\n");
}

double **k_means(int k, int n, int d, int iter, double **data)
{
    struct Cluster Cl;
    double **centroids = sub_matrix_k(data, k, d);
    for (int i = 0; i < iter; i++)
    {
        printf("Centroids Matrix:\n");
        print2DArray(centroids, k, d);
        struct Cluster *new_centroids = (struct Cluster *)createArray(k, sizeof(Cl));
        for (int j = 0; j < k; j++)
        {
            new_centroids[j].size = 0;
            new_centroids[j].centroid = calloc(d, sizeof(double));
        }
        printf("Iterating through data:\n\n");
        for (int data_i = 0; data_i < n; data_i++)
        {
            double *x = data[data_i];
            printf("current x:\n");
            printArray(data[data_i], d);
            int closest_centroid_index = find_closest_centroid_index(centroids, x, k, d);
            printf("***************\n");
            printf("x is closest to\n");
            printArray(centroids[closest_centroid_index], d);
            printf("***************");
            add_vector_to_centroid(&new_centroids[closest_centroid_index], x, d);
            printf("new cluster size is: %d\n", new_centroids[closest_centroid_index].size);
        }
        double **updated_centroids = (double **)createArray(k, sizeof(double *));
        for (int j = 0; j < k; j++)
        {
            updated_centroids[j] = calc_centroid_average(new_centroids[j], d);
        }
        int convergence = check_centroid_convergence(updated_centroids, centroids, k, d);
        double* temp = centroids;
        centroids = updated_centroids;
        //free_matrix(temp, k);
        //free_clusters(new_centroids);
        if (convergence)
            break;
        printf("\n\n\nEND OF LOOP\n\n");
    }
    return centroids;
}

void print2DArray(double **array, int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            printf("%lf ", array[i][j]);
        }
        printf("\n");
    }
}

int main()
{
    int k = 2;
    int n = 4;
    int d = 2;
    int iter = 3;
    int rows = 4;
    int cols = 2;
    printf("1111111111111111111111");
    // Dynamically allocate memory for the 2D array
    double **array = malloc(rows * sizeof(double *));
    for (int i = 0; i < rows; i++)
    {
        array[i] = malloc(cols * sizeof(double));
    }
    array[0][0] = 1;
    array[0][1] = 1;
    array[1][0] = 2;
    array[1][1] = 0;
    array[2][0] = 2;
    array[2][1] = 1;
    array[3][0] = 4;
    array[3][1] = 0;
    print2DArray(array, rows, cols);

    printf("hiiiiiiiiiiiarraiiiiiiiiiiiiiiiiiiiiii\n");
    print2DArray(k_means(k, n, d, iter, array), rows, cols);
    printf("3333333333333333\n");
    return 0;
}
