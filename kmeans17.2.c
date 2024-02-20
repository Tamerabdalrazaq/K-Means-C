#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#define EPSILON 0.01
#define ITER 200
#define ERROR_K "Invalid number of clusters!"
#define ERROR_N "Invalid number of points!"
#define ERROR_d "Invalid dimension of point!"
#define ERROR_iter "Invalid maximum iteration!"


int isStringDigit(const char *str)
{
    // Iterate through each character in the string
    for (int i = 0; str[i] != '\0'; i++)
    {
        // Check if the character is not a digit
        if (!isdigit(str[i]))
        {
            // If any character is not a digit, return false
            return 0;
        }
    }
    // If all characters are digits, return true
    return 1;
}

#if DEBUG
void printArray(double *arr, int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("%f ", (arr[i]));
    }
    printf("\n");
}
#endif

#if DEBUG
void print2DArray(double **array, int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            printf("%lf ", (array[i][j]));
        }
        printf("\n");
    }
}
#endif

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

#if DEBUG
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
#endif

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
        // printf("Sub_matrix_k \n");
        sub_array[i] = (double *)createArray(d, sizeof(double));
        for (int j = 0; j < d; j++)
        {
            sub_array[i][j] = matrix[i][j];
            //printf("%lf ", sub_array[i][j]);
        }
    }
    // printf("out\n");
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

double euc_l2(double *v1, double *v2, int d)
{
    double dist = 0.0;
    for (int i = 0; i < d; i++)
    {
        // printf("d_i: %f\n", v1[i] - v2[i]);
        dist += pow(v1[i] - v2[i], 2.0);
    }
    // printf("Dist is: sqrt(%f): %f End;\n", dist, sqrt(dist));
    return sqrt(dist);
}

int find_closest_centroid_index(double **centroids, double *v, int k, int d)
{
    double min_dist = INFINITY;
    int index;
    for (int i = 0; i < k; i++)
    {
        double dist = euc_l2(v, centroids[i], d);
        if (dist < min_dist)
        {
            index = i;
            min_dist = dist;
        }
    }
    return index;
}

double *calc_centroid_average(Cluster cluster, int d)
{
    double *centroid = cluster.centroid;
    int size = cluster.size;
    double *averaged_vector = calloc(d, sizeof(double));
    if (size == 0)
    {
        return averaged_vector;
    }
    for (int i = 0; i < d; i++)
    {
        averaged_vector[i] = (double)centroid[i] / size;
    }
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

void add_vector_to_centroid(Cluster *clus, double vec[], int d)
{
    for (int i = 0; i < d; i++)
    {
        double updated_entry_i = clus->centroid[i] + vec[i];
        clus->centroid[i] = updated_entry_i;
    }
    clus->size = clus->size + 1;
}

double **k_means(int k, int n, int d, int iter, double **data)
{
    struct Cluster Cl;
    double **centroids = sub_matrix_k(data, k, d);
    for (int i = 0; i < iter; i++)
    {
        // printf("Centroids Matrix:\n");
        // print2DArray(centroids, k, d);
        struct Cluster *new_centroids = (struct Cluster *)createArray(k, sizeof(Cl));
        int j;
        for (j = 0; j < k; j++)
        {
            new_centroids[j].size = 0;
            new_centroids[j].centroid = calloc(d, sizeof(double));
        }
        // printf("Iterating through data:\n\n");
        for (int data_i = 0; data_i < n; data_i++)
        {
            double *x = data[data_i];
            // printf("current x:\n");
            // printArray(data[data_i], d);
            int closest_centroid_index = find_closest_centroid_index(centroids, x, k, d);
            // printf("***************\n");
            // printf("x is closest to\n");
            // printArray(centroids[closest_centroid_index], d);
            // printf("***************");
            add_vector_to_centroid(&new_centroids[closest_centroid_index], x, d);
            // printf("new cluster size is: %d\n", new_centroids[closest_centroid_index].size);
        }
        double **updated_centroids = (double **)createArray(k, sizeof(double *));
        for (j = 0; j < k; j++)
        {
            updated_centroids[j] = calc_centroid_average(new_centroids[j], d);
        }
        int convergence = check_centroid_convergence(updated_centroids, centroids, k, d);
        double **temp = centroids;
        centroids = updated_centroids;
        // free_matrix(temp, k);
        // free_clusters(new_centroids);
        if (convergence)
            break;
        // printf("\n\n\nEND OF LOOP\n\n");
    }
    //print2DArray(centroids, k, d);
    return centroids;
}

int main(int argc, char *argv[])
{
    if (isStringDigit((char *)argv[3]) == 0)
    {
        printf(ERROR_d);
        return 0;
    }
    if (isStringDigit((char *)argv[1]) == 0)
    {
        printf(ERROR_K);
        return 0;
    }
    if (isStringDigit((char *)argv[2]) == 0)
    {
        printf(ERROR_N);
        return 0;
    }
    int iter;
    int K = atoi((char *)argv[1]);
    int n = atoi((char *)argv[2]);
    int d = atoi((char *)argv[3]);
    double **data = (double **)createArray(n, sizeof(double *));
    int i, j;
    for (i = 0; i < n; i++)
    {
        data[i] = (double *)createArray(d, sizeof(double));
    }
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < d; j++)
        {
            char c;
            double d;
            scanf("%lf%c", &d, &c);
            data[i][j] = d;
        }
    }
    if (argc >= 6)
    {
        if (isStringDigit((char *)argv[4]) == 0)
        {
            printf(ERROR_iter);
            return 0;
        }
        else
        {
            int iter = atoi((char *)argv[4]);
        }
    }
    else
    {
        iter = ITER;
    }

    if (n <= 1)
    {
        printf(ERROR_N);
        return 0;
    }
    if (K < 1 || n < K)
    {
        printf(ERROR_K);
        return 0;
    }
    if (iter < 1 || iter > 1000)
    {
        printf(ERROR_iter);
        return 0;
    }

    //print2DArray(data, n, d);
    //printf("**********************************************\n");
    double **output = k_means(K, n, d, iter, data);

    for (int i = 0; i < K; i++)
    {
        for (int j = 0; j < d; j++)
        {
            printf("%.4f ", output[i][j]);
        }
        printf("\n");
    }
    return 0;
}

/*int main()
{
    int k = 4;
    int n = 6;
    int d = 2;
    int iter = 3;
    int rows = 6;
    int cols = 2;
    printf("1111111111111111111111 \n");
    // Dynamically allocate memory for the 2D array
    //double **array = malloc(rows * sizeof(double *));
    //for (int i = 0; i < rows; i++)
    //{
    //    array[i] = malloc(cols * sizeof(double));
    //}

    double **array = (double **)createArray(rows, sizeof(double *));
    int i;
    for (i = 0; i < rows; i++)
    {
        array[i] = (double *)createArray(cols, sizeof(double));
    }
    //[(1, 2), (5, 8), (1, 5), (6, 9), (2, 2), (8, 1)]
    array[0][0] = 1;
    array[0][1] = 2;
    array[1][0] = 5;
    array[1][1] = 8;
    array[2][0] = 1;
    array[2][1] = 5;
    array[3][0] = 6;
    array[3][1] = 9;
    array[4][0] = 2;
    array[4][1] = 2;
    array[5][0] = 8;
    array[5][1] = 1;

    //print2DArray(array, rows, cols);

    printf("hiiiiiiiiiiiarraiiiiiiiiiiiiiiiiiiiiii\n");
    k_means(k, n, d, iter, array);
    printf("3333333333333333\n");
    return 0;
}
*/
