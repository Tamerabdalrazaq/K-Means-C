import math

EPSILON = 0.01

def main():
    return 0

def k_means(k,n,d,iter,data):
    centroids = [(vector, 1) for vector in data[0:k]]
    for i in range(0, iter):
        prev_centroids = [(centroid[0].copy(), centroid[1]) for centroid in centroids]
        for x in data:
            closest_centroid = find_closest_centroid(centroids, x)
            add_vector_to_centroid(closest_centroid[0], x)
        if check_centroid_convergence(centroids, prev_centroids):
            break
    return centroids

def find_closest_centroid(centroids, v):
    closest_centroid = (None, math.inf)
    for centroid in centroids:
        distance = euc_l2(centroid[0], v)
        if (distance < closest_centroid[1]):
            closest_centroid = (centroid, distance)
    return closest_centroid


def euc_l2(v1, v2):
    dist = 0
    for i in range(len(v1)):
        dist += math.pow(v1[i]-v2[i], 2)
    return math.sqrt(dist)

def add_vector_to_centroid(centroid_tuple, v):
    updated_vector = []
    centroid_size = centroid_tuple[1] + 1
    for i in range(len(centroid_tuple[0])):
        updated_entry_i = centroid_tuple[0][i] + (v[i]-centroid_tuple[0][i])* (1/centroid_size)
        updated_vector.append(updated_entry_i)
    return updated_vector


def check_centroid_convergence(centroids, prev_centroids):
    convergent_centroids = 0
    for i in range(len(centroids)):
        if euc_l2(centroids[i][0], prev_centroids[i][0]) <= EPSILON:
            convergent_centroids += 1
    return convergent_centroids == len(centroids)