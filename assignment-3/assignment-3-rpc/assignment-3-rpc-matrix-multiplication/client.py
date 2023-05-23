import xmlrpc.client
import csv, numpy
server = xmlrpc.client.ServerProxy("http://localhost:8000")
# X = [[12,7,3],
#     [4 ,5,6],
#     [7 ,8,9]]
# Y = [[5,8,1,2],
#     [6,7,3,0],
#     [4,5,9,1]]

def read_matrix(filename):
    with open(filename+".csv", newline='') as file:
        result_list = list(csv.reader(file))
    # print(result_list)
    result = [[int(x) for x in r] for r in result_list]
    # print(result)
    return result

X = read_matrix("matrix1")
Y = read_matrix("matrix2")


print("matrix A: ", X)
print("matrix B: ", Y)
print("making rpc...")
result = server.multiply_matrices(X, Y)
print("AB: ", result)