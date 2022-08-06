import socket
host = '127.0.0.1'
n = 2
ports = [7875, 7876, 7877, 7878]

emojis = ['😍', '😡', '😹', '💩']

import time

import sys
id = int(sys.argv[1])
prev = (id - 1) % n
next = (id + 1) % n

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((host, ports[id]))

if id == 0:
    print('Sending {} to {}'.format(emojis[id], next))
    sock.sendto(str.encode(emojis[id]), 0, (host, ports[next]))

while True:
    data, addr = sock.recvfrom(1024)

    print('Received {} from {}'.format(data.decode(), prev))

    time.sleep(1)

    print('Sending {} to {}'.format(emojis[id], next))
    sock.sendto(str.encode(emojis[id]), 0, (host, ports[next]))


sock.close()