from kmeans import k_means
array = [
    [1, 1],
    [2, 0],
    [2, 1],
    [4, 0]
]
result_5_2 = k_means(2, 4, 2, 3, array)
print("Result for:")
print(result_5_2)

#  (1,1) (2,2,) (3,3)
# ((1,1)) ((2,2)) (4,4)
# ((1,1)) (2.5, 2.5) (4.5,4.5)
# ((1,1)) (2.5, 2.5) (4.5,4.5)