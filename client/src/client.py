import protocol
import socket
import util

class my_client():
    def __init__(self,IP,PORT):
        self.my_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.my_socket.connect((IP, PORT))

        self.shared_secret = protocol.exchange_keys(self.my_socket)
        self.aesKey = self.shared_secret[0:protocol.AES_KEY_SIZE]
        self.hmacKey = self.shared_secret[protocol.AES_SIZE:protocol.AES_SIZE+protocol.HMAC_KEY_SIZE]
    
    def login(self,username,password):
        username_len = str(len(username)).zfill(2)
        data_field = username_len+username+password
        login_msg = protocol.compose_secure_msg(protocol.commandID.LOGIN.value,data_field,self.aesKey,self.hmacKey)
        self.my_socket.send(login_msg.encode())
        while True:
            cmd_ID,data,hmacFlag = protocol.get_encrypted_msg(self.my_socket,self.aesKey,self.hmacKey)
            if hmacFlag:
                break
        if data == "SUCCESS":
            self.username = username
            return True
        else:
            return False
    
    def uptime(self):
        uptime_msg = protocol.compose_secure_msg(protocol.commandID.UPTIME.value,"",self.aesKey,self.hmacKey)
        self.my_socket.send(uptime_msg.encode())
        cmd_ID,data,hmacFlag = protocol.get_encrypted_msg(self.my_socket,self.aesKey,self.hmacKey)
        if not hmacFlag:
            return None
        if cmd_ID != protocol.commandID.UPTIME.value:
            return None
        return data
    
    def num_conns(self):
        num_conns_msg = protocol.compose_secure_msg(protocol.commandID.NUM_CONNECTIONS.value,"",self.aesKey,self.hmacKey)
        self.my_socket.send(num_conns_msg.encode())
        cmd_ID,data,hmacFlag = protocol.get_encrypted_msg(self.my_socket,self.aesKey,self.hmacKey)
        if not hmacFlag:
            return None
        if cmd_ID != protocol.commandID.NUM_CONNECTIONS.value:
            return None
        return data
    
    def num_routes(self):
        num_routes_msg = protocol.compose_secure_msg(protocol.commandID.NUM_ROUTES.value,"",self.aesKey,self.hmacKey)
        self.my_socket.send(num_routes_msg.encode())
        cmd_ID,data,hmacFlag = protocol.get_encrypted_msg(self.my_socket,self.aesKey,self.hmacKey)
        if not hmacFlag:
            return None
        if cmd_ID != protocol.commandID.NUM_ROUTES.value:
            return None
        return data
    
    def num_times_holds_lit(self):
        num_times_holds_lit_msg = protocol.compose_secure_msg(protocol.commandID.NUM_TIMES_HOLD_LIT.value,"",self.aesKey,self.hmacKey)
        self.my_socket.send(num_times_holds_lit_msg.encode())
        cmd_ID,data,hmacFlag = protocol.get_encrypted_msg(self.my_socket,self.aesKey,self.hmacKey)
        if not hmacFlag:
            return None
        if cmd_ID != protocol.commandID.NUM_TIMES_HOLD_LIT.value:
            return None
        board = data.split(protocol.HOLD_LIT_SEPARATOR)[:-1]
        board = util.moonboard_1d_to_2d(board)
        return board
    
    def num_color_lit(self,color : protocol.color):
        data = ""
        if color == protocol.color.RED:
            data = "R"
        elif color == protocol.color.GREEN:
            data = "G"
        elif color == protocol.color.BLUE:
            data = "B"
        else:
            raise ValueError()
        num_color_lit_msg = protocol.compose_secure_msg(protocol.commandID.NUM_TIMES_COLOR_LIT.value,data,self.aesKey,self.hmacKey)
        self.my_socket.send(num_color_lit_msg.encode())
        cmd_ID,data,hmacFlag = protocol.get_encrypted_msg(self.my_socket,self.aesKey,self.hmacKey)
        if not hmacFlag:
            return None
        if cmd_ID != protocol.commandID.NUM_TIMES_COLOR_LIT.value:
            return None
        return data
    def change_password(self,username,original_password,new_password):
        username_len = str(len(username)).zfill(2)
        original_password_len = str(len(original_password)).zfill(2)

        data_field = username_len+original_password_len+username+original_password+new_password
        change_password_msg = protocol.compose_secure_msg(protocol.commandID.CHANGE_PASSWORD.value,data_field,self.aesKey,self.hmacKey)
        self.my_socket.send(change_password_msg.encode())
        while True:
            cmd_ID,data,hmacFlag = protocol.get_encrypted_msg(self.my_socket,self.aesKey,self.hmacKey)
            if hmacFlag:
                break
        if data == "SUCCESS":
            return True
        else:
            return False