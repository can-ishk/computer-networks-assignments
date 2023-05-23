import xmlrpc.server

class MyServer:
    @staticmethod
    def multiply_matrices(X, Y):
        result = [[sum(a*b for a,b in zip(X_row,Y_col)) for Y_col in zip(*Y)] for X_row in X]
        return result
server = xmlrpc.server.SimpleXMLRPCServer(("localhost", 8000))
server.register_instance(MyServer())
server.serve_forever()