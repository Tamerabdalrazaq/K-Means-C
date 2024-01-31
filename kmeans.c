#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

int main()
{
    return 0;
}

double euc_12(double* v1, double* v2){
    double dist = 0;
    for (i=0 ; i<sizeof(v1) ;i++)
        dist+=pow(v1[i]-v2[i],2.0)
    return sqrt(dist) 
}

//struct tuple_of_centXdist

struct Cluster {
    double* centroid;
    int size;
}

double* find_closest_centroid(struct Cluster *vecs , double v[]){
    double min_dist = 0.0;
    double *centroid;
    for(i=0; i<sizeof(vecs); i++)
        double d = euc_12(v,vecs[i]);
        if (d < min_dist)
            centroid = &vecs[i];
            min_dist = d;
return centroid
    


    
    
        
