import xmlrpc.client
import sys
server = xmlrpc.client.ServerProxy("http://localhost:8001")

n = sys.argv[1]
print("making rpc...")
result = server.fac(int(n))
print("factorial of", n, "is", result)