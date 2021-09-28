import socket

if __name__ == "__main__":
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM, socket.IPPROTO_TCP)
        
        s.connect(("172.29.3.219", 8080))

        req = "GET /index.html HTTP/1.1\r\nHost: 172.29.3.219:8080\r\n\r\n".encode("utf-8")

        s.send(req)

        data = s.recv(8192)

        print(data.decode("utf-8"))

        s.close()

    except:
        print("error connect")
        s.close()