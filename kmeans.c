#include <stdio.h>
#include <stdlib.h>
#include <math.h>

EPSILON = 0.01;

struct Cluster
{
    double *centroid;
    int size;
};
typedef struct Cluster Cluster;

int arrayLength(void *arr)
{
    return sizeof(arr) / sizeof(arr[0]);
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

double k_means(int K, int N, int d, int iter, int **data)
{
    struct Cluster *clusters = createArray(N, sizeof(struct Cluster));

    // create an array of k elements, initiated as the first k vektors
    // initiate a kx(d+1) matrix by the name centroids that represents the k centroids, each cell includes [k_1,...,k_d] and |K|
    // initiate another kxd matrix by the name prevCentroids that holds the previous centroids at the end of the iteration to compare the delta means
    // repeat:
    // for every vector from 1 till N calculate the euclidean distance and find the minimum distance among all of the k clusters
    // update the centroid as follows:copy the current centroid to the suitable cell in prevCentroids matrix and calculate the weighted mean of the current mean and the new vector added to the cluster and update the suitable cell in centroid matrix
    // at the end of each iteration we initiate a counter=0 ,go over the two matrices Centroids and prevCentroids and calculate the delta, if its smaller than epselon we add 1 to the counter.
    // before we start a new iteration we check if the counter equalls K then we break, else we continue with the loop until we reach the limiter iter value
    return 0;
}

int main(int argc, char *argv[])
{
    // argc returns the count of arguments passed by the user including the program name
    // argv is the array of strings after splitting the command line into arguments, including the program name

    return 0;
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

// def k_means(k,n,d,iter,data):
//     centroids = [(vector) for vector in data[0:k]]
//     for i in range(0, iter):
//         new_centroids = [[[0] * d, 0] for centroid in centroids]
//         for x in data:
//             closest_centroid_index = find_closest_centroid_index(centroids, x)
//             add_vector_to_centroid(new_centroids[closest_centroid_index], x)
//         new_centroids =  [calc_centroid_average(cent) for cent in new_centroids]
//         if check_centroid_convergence(centroids, new_centroids):
//             centroids = new_centroids
//             break
//         centroids = new_centroids

void k_means(int k, int n, int d, int iter, double **data)
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

double euc_12(double *v1, double *v2)
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
        double d = euc_12(v, centroids[i]);
        if (d < min_dist)
        {
            index = i;
            min_dist = d;
        }
    }
    return index;
}

// def calc_centroid_average(centroid_size_tuple):
//     centroid = centroid_size_tuple[0]
//     size = centroid_size_tuple[1]
//     return [x/size for x in centroid]

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