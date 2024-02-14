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

void *createArray(double n, int size)
{
    void *array = (double *)malloc(n * size);
    if (array == NULL)
    {
        printf("Memory allocation failed!\n");
        exit(1); // Exit program if memory allocation fails
    }
    return array;
}

int arrayLength(void *arr)
{
    return sizeof(arr) / sizeof(arr[0]);
}

double **sub_matrix_k(double **matrix, int k)
{
    double **sub_array;
    int d = arrayLength(matrix);
    sub_array = malloc(k * sizeof(double *));
    for (int i = 0; i < k; i++)
    {
        sub_array[i] = (double *)createArray(d, sizeof(double));
        for (int j = 0; j < d; j++)
            sub_array[i][j] = matrix[i][j];
    }
    return sub_array;
}

void free_matrix(double **matrix)
{
    for (int i = 0; i < arrayLength(matrix); i++)
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

double euc_l2(double *v1, double *v2)
{
    double dist = 0;
    for (int i = 0; i < sizeof(v1); i++)
        dist += pow(v1[i] - v2[i], 2.0);
    return sqrt(dist);
}

// struct tuple_of_centXdist
// Forward declaration of struct Cluster

double *find_closest_centroid_index(double **centroids, double v[])
{
    double min_dist = INFINITY;
    int index;
    for (int i = 0; i < arrayLength(centroids); i++)
    {
        double d = euc_l2(v, centroids[i]);
        if (d < min_dist)
        {
            index = i;
            min_dist = d;
        }
    }
    return index;
}

double *calc_centroid_average(struct Cluster cluster)
{
    double *centroid = cluster.centroid;
    int n = arrayLength(centroid);
    int size = cluster.size;
    int *averaged_vector = createArray(n, sizeof(double));
    for (int i = 0; i < n; i++)
    {
        averaged_vector[i] = centroid[i] / size;
    }
    return averaged_vector;
}

// def check_centroid_convergence(centroids, new_centroids):
//     convergent_centroids = 0
//     for i in range(len(centroids)):
//         if euc_l2(centroids[i], new_centroids[i]) <= EPSILON:
//             convergent_centroids += 1
//     return convergent_centroids == len(centroids)

int check_centroid_convergence(double **centroids, double **new_centroids)
{
    int convergent_centroids = 0;
    int k = arrayLength(centroids);
    for (int i = 0; i < k; i++)
    {
        double *centroid_old = centroids[i];
        double *centroid_new = new_centroids[i];
        if (euc_l2(centroid_old, centroid_new) <= EPSILON)
            convergent_centroids += 1;
    }
    return (convergent_centroids == k);
}

void add_vector_to_centroid(Cluster clus, double vec[])
{
    for (int i; i < sizeof(clus.centroid); i++)
    {
        double updated_entry_i = clus.centroid[i] + vec[i];
        clus.centroid[i] = updated_entry_i;
    }
    clus.size++;
}

double **k_means(int k, int n, int d, int iter, double **data)
{
    struct Cluster Cl;
    double **centroids = sub_matrix_k(data, k);
    for (int i = 0; i < iter; i++)
    {
        struct Cluster *new_centroids = (struct Cluster *)createArray(k, sizeof(Cl));
        for (int j = 0; j < k; j++)
        {
            new_centroids[j].size = 0;
            new_centroids[j].centroid = calloc(d, sizeof(double));
        }
        for (int data_i = 0; i < n; i++)
        {
            double *x = data[data_i];
            int closest_centroid_index = find_closest_centroid_index(centroids, x);
            add_vector_to_centroid(new_centroids[closest_centroid_index], x);
        }
        double **updated_centroids = (double **)createArray(k, sizeof(double *));
        for (int j = 0; j < k; j++)
        {
            updated_centroids[j] = calc_centroid_average(new_centroids[j]);
        }
        int convergence = check_centroid_convergence(updated_centroids, centroids);
        centroids = new_centroids;
        free_matrix(updated_centroids);
        free_clusters(new_centroids);
        if (convergence)
            break;
    }
    return centroids;
}

int main()
{
    // int k = 3;
    // int n = 4;
    // int d = 2;
    // int iter = 3;
    // int rows = 4;
    // int cols = 3;

    // // Dynamically allocate memory for the 2D array
    // double **array = malloc(rows * sizeof(double *));
    // for (int i = 0; i < rows; i++)
    // {
    //     array[i] = malloc(cols * sizeof(double));
    // }
    printf('HI');
    // k_means(k, n, d, iter, array);
    return 0;
}
