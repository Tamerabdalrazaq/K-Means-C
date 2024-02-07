#include <stdio.h>
#include <stdlib.h>
#include <math.h>

EPSILON = 0.01

    int
    arrayLength(void *arr)
{
    return sizeof(arr) / sizeof(arr[0]);
}

double *createArray(double n)
{
    double *array = (double *)malloc(n * sizeof(double));
    if (array == NULL)
    {
        printf("Memory allocation failed!\n");
        exit(1); // Exit program if memory allocation fails
    }
    return array;
}

double k_means(int K, int N, int d, int iter, int *data)
{
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

void k_means(double k, double n, double iter)
{
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
struct Cluster;
// Now you can declare variables of type struct Cluster pointer
struct Cluster *centroids;
struct Cluster
{
    double *centroid;
    double size;
};

double *find_closest_centroid(struct Cluster *vecs, double v[])
{
    double min_dist = 0.0;
    double *centroid;
    for (int i = 0; i < sizeof(vecs); i++)
    {
        double d = euc_12(v, vecs[i].centroid);
        if (d < min_dist)
        {
            centroid = &vecs[i];
            min_dist = d;
        }
    }
    return centroid;
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
    int *averaged_vector = createArray(n);
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

int check_centroid_convergence(struct Cluster *centroids, struct Cluster *new_centroids)
{
    int convergent_centroids = 0;
    int k = arrayLength(centroids);
    for (int i = 0; i < k; i++)
    {
        struct Cluster cluster_old = centroids[i];
        struct Cluster cluster_new = new_centroids[i];
        double *centroid_old = cluster_old.centroid;
        double *centroid_new = cluster_new.centroid;
        if (euc_l2(centroid_old, centroid_new) <= EPSILON)
            convergent_centroids += 1;
    }
    return (convergent_centroids == k);
}

void add_vector_to_centroid(struct Cluster clus, double vec[])
{
    for (int i; i < sizeof(clus.centroid); i++)
    {
        double updated_entry_i = clus.centroid[i] + vec[i];
        clus.centroid[i] = updated_entry_i;
    }
    clus.size++;
}