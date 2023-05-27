from enum import IntEnum
import math
import socket
from Crypto.Cipher import AES
import hmac
import hashlib
from cryptography.hazmat.primitives.asymmetric import x25519

class commandID(IntEnum):
        LOGIN = 0
        UPTIME = 1
        NUM_CONNECTIONS = 2
        NUM_ROUTES = 3
        NUM_TIMES_HOLD_LIT = 4
        NUM_TIMES_COLOR_LIT = 5
        CHANGE_PASSWORD = 11
        DIFFIE_HELLMAN = 95
        WRONG_HMAC = 96
        CONN_NOT_SECURE = 97
        NOT_DEFINED = 98
        ACCESS_DENIED = 99

class color(IntEnum):
    RED = 1
    GREEN = 2
    BLUE = 3

ID_WIDTH = 2
DATA_LEN_WIDTH = 4
HMAC_SIZE = 32
AES_SIZE = 16
AES_KEY_SIZE = 16
HMAC_KEY_SIZE = 16

HOLD_LIT_SEPARATOR = ";"
MOON_COLS = 11
MOON_ROWS = 18

def pad_whitespace(msg):
    paddingSize = AES_SIZE - (len(msg) % AES_SIZE)
    if paddingSize == 0:
        paddingSize = AES_SIZE
    msg += " "*paddingSize
    return msg 

def compose_secure_msg(cmd_ID,data, aes_key, hmac_key):
    msg = compose_msg(cmd_ID,data)
    msg = pad_whitespace(msg)
    h_signature = hmac.new(hmac_key,msg.encode(),hashlib.sha256).hexdigest()
    h_signature = h_signature.upper()
    msg = h_signature+msg
    print("plain msg: ", msg)

    cipher = AES.new(aes_key, AES.MODE_ECB)
    msg=cipher.encrypt(msg.encode("ascii")).hex()
    return msg

def compose_msg(cmd_ID : int,data : str):
    """ creates a message according to protocol

    Args:
        data (str): cmd_id than the data to send separated by whitespace

    Returns:
        [str]: a message according to protocol
        may return None if the data is too lengthy
    """

    len_data = len(data)
    cmd_ID = str(cmd_ID)
    print(cmd_ID)
    print(len_data)
    if len(cmd_ID) < ID_WIDTH:
        cmd_ID = cmd_ID.zfill(ID_WIDTH)
    elif len(cmd_ID) > ID_WIDTH:
        return None

    if len_data > (10**DATA_LEN_WIDTH-1): # larger then 9999
        return None
    len_data = str(len_data).zfill(DATA_LEN_WIDTH)
    return cmd_ID+len_data+data

def decipher_msg(block,key):
    decipher = AES.new(key, AES.MODE_ECB)
    msg_hex = decipher.decrypt(bytes.fromhex(block)).hex()
    return bytes.fromhex(msg_hex).decode("ASCII").rstrip()
    
def get_encrypted_msg(my_socket : socket,aes_key,hmac_key):
    """
    receive message, decrypt, calculates hmac,and extracts data field
    """
    received_hmac = my_socket.recv(HMAC_SIZE*4).decode()
    id_len_block = my_socket.recv(AES_SIZE*2).decode()

    received_hmac = decipher_msg(received_hmac,aes_key)
    id_len_block = decipher_msg(id_len_block,aes_key)


    cmd_ID_str = id_len_block[0:ID_WIDTH]
    data_len_str = id_len_block[ID_WIDTH:ID_WIDTH+DATA_LEN_WIDTH]

    cmd_ID = int(cmd_ID_str)
    data_len = int(data_len_str)
    data_deciphered = id_len_block[ID_WIDTH+DATA_LEN_WIDTH:ID_WIDTH+DATA_LEN_WIDTH+data_len]
    data_len -= (AES_SIZE-ID_WIDTH-DATA_LEN_WIDTH)
    
    data_len = AES_SIZE * math.ceil(data_len / AES_SIZE) # round data len to nearest multiple of KEY SIZE to account for the padded whitespace that aren't counted in data length
    data_len *= 2 # account for hex taking twice the size of ascii
    data = ""
    while True:
        if data_len >1024:
            data += my_socket.recv(1024).decode()
            data_len -= 1024
        else:
            data += my_socket.recv(data_len).decode()
            break

    data = data_deciphered + decipher_msg(data,aes_key)
    msg = cmd_ID_str + data_len_str + data
    calculated_hmac = hmac.new(hmac_key,msg.encode(),hashlib.sha256).hexdigest()
    print(cmd_ID," response: ", data)
    return cmd_ID,data,(calculated_hmac.casefold()==received_hmac.casefold())

def get_not_encrypted_msg(my_socket : socket):
    """
    receive message, and extracts data field
    """
    hmac = my_socket.recv(HMAC_SIZE*2).decode()
    cmd_ID = my_socket.recv(ID_WIDTH).decode()
    data_len = int(my_socket.recv(DATA_LEN_WIDTH).decode())

    data = ""
    while True:
        if data_len >1024:
            data += my_socket.recv(1024).decode()
            data_len -= 1024
        else:
            data += my_socket.recv(data_len).decode()
            return cmd_ID,data
def exchange_keys(conn : socket.socket):
    client_private_key = x25519.X25519PrivateKey.generate()
    client_public_key = client_private_key.public_key()

    msg = compose_msg(95,str(client_public_key.public_bytes_raw().hex()))
    msg = "0"*HMAC_SIZE + msg
    conn.send(msg.encode())

    cmd_ID,server_public_key = get_not_encrypted_msg(conn)
    server_public_key = x25519.X25519PublicKey.from_public_bytes(bytes.fromhex(server_public_key))
    shared_secret = client_private_key.exchange(server_public_key)

    return shared_secret