#!/usr/bin/env python

import socket
import sys
if len(sys.argv) < 3:
    print("Expected params: [server_port] [user_id]")
    exit(1)

TCP_IP = 'localhost'
TCP_PORT = int(sys.argv[1])
user_id = int(sys.argv[2])

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((TCP_IP, TCP_PORT))
s.send(user_id.to_bytes(8, byteorder='little'))
while True:
    buffer = s.recv(1024)
    if not buffer:
        s.close()
        break
    else:
        print(buffer.decode('utf8'))
print("Finished")