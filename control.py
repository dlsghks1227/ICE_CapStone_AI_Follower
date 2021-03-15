import socket

ADDRESS = "127.0.0.1"
PORT = 6215

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((ADDRESS, PORT))

while True:
    data, addr = sock.recvfrom(1024)
    if data.decode() is not None:
        splitData = data.decode().split('|')
        if len(splitData) >= 2:
            if splitData[0] == 'left':
                print(f"Direction: {splitData[0]} speed: {splitData[1]}")
            elif splitData[0] == 'right':
                print(f"Direction: {splitData[0]} speed: {splitData[1]}")
            elif splitData[0] == 'front':
                print(f"Direction: {splitData[0]} speed: {splitData[1]}")
            else:
                print("stop")
sock.close()