import xmlrpc.server

class MyServer:
    @staticmethod
    def fac(num):
        factorial=1
        if num < 0:
            print("Sorry, factorial does not exist for negative numbers")
        elif num == 0:
            return 1
        else:
            for i in range(1,num + 1):
                factorial = factorial*i
            return factorial
server = xmlrpc.server.SimpleXMLRPCServer(("localhost", 8001))
server.register_instance(MyServer())
server.serve_forever()