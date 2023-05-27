# Message structure
| FIELD | hmac       | cmd id  | data length | data           |
|-------|------------|---------|-------------|----------------|
| SIZE  | 64 chars   | 2 chars | 4 chars     | 0 - 9999 chars |
| TYPE  | hex string | int     | int         | string         |
# Commands
  - [LOGIN \[00\]](#login-00)
  - [UPTIME \[01\]](#uptime-01)
  - [NUM_CONNECTIONS \[02\]](#num_connections-02)
  - [NUM_ROUTES \[03\]](#num_routes-03)
  - [NUM_TIMES_HOLD_LIT \[04\]](#num_times_hold_lit-04)
  - [NUM_TIMES_COLOR_LIT \[05\]](#num_times_color_lit-05)
  - [CHANGE_PASSWORD \[11\]](#change_password-11)
  - [DIFFIE_HELLMAN \[95\]](#diffie_hellman-95)
  - [WRONG_HMAC \[96\]](#wrong_hmac-96)
  - [CONN_NOT_SECURE \[97\]](#conn_not_secure-97)
  - [NOT_DEFINED \[98\]](#not_defined-98)
  - [ACCESS_DENIED \[99\]](#access_denied-99)

## LOGIN [00]
### Description:
The server will check whether the username and password are registered, if so will send "SUCCESS" otherwise "FAIL"
### Client message:
| hmac               | CMD_ID | DATA_LENGTH        | DATA                                        |
|--------------------|--------|--------------------|---------------------------------------------|
| will be calculated | 00     | will be calculated | LENGTH_USERNAME( 2chars ) USERNAME PASSWORD |
### Server message:
| hmac               | CMD_ID | DATA_LENGTH | DATA           |
|--------------------|--------|-------------|----------------|
| will be calculated | 00     | 0007 / 0004 | SUCCESS / FAIL |


## UPTIME [01]
### Description:
The server will send the number of microseconds passed since the last reboot.
### Client message:
| hmac               | CMD_ID | DATA_LENGTH | DATA |
|--------------------|--------|-------------|------|
| will be calculated | 01     | 0000        | NONE |
### Server message:
| hmac               | CMD_ID | DATA_LENGTH        | DATA                             |
|--------------------|--------|--------------------|----------------------------------|
| will be calculated | 01     | will be calculated | num of microseconds since reboot |

## NUM_CONNECTIONS [02]
### Description:
The server will send the number of ble connections ever happened.
### Client message:
| hmac               | CMD_ID | DATA_LENGTH | DATA |
|--------------------|--------|-------------|------|
| will be calculated | 02     | 0000        | NONE |
### Server message:
| hmac               | CMD_ID | DATA_LENGTH        | DATA                   |
|--------------------|--------|--------------------|------------------------|
| will be calculated | 02     | will be calculated | num of ble connections |

## NUM_ROUTES [03]
### Description:
The server will send the number of routes ever lit (the count goes up every time a user lit a route doesn't matter if the route was already lit before).
### Client message:
| hmac               | CMD_ID | DATA_LENGTH | DATA |
|--------------------|--------|-------------|------|
| will be calculated | 03     | 0000        | NONE |
### Server message:
| hmac               | CMD_ID | DATA_LENGTH        | DATA                   |
|--------------------|--------|--------------------|------------------------|
| will be calculated | 03     | will be calculated | num of routes ever lit |

## NUM_TIMES_HOLD_LIT [04]
### Description:
The server will send the number of times a specific hold was ever lit.
### Client message:
| hmac               | CMD_ID | DATA_LENGTH        | DATA                                    |
|--------------------|--------|--------------------|-----------------------------------------|
| will be calculated | 04     | will be calculated | index of the hold (the moonboard index) |
### Server message:
| hmac               | CMD_ID | DATA_LENGTH        | DATA                                      |
|--------------------|--------|--------------------|-------------------------------------------|
| will be calculated | 04     | will be calculated | num of times a specific hold was ever lit |

## NUM_TIMES_COLOR_LIT [05]
### Description:
The server will send the number of times holds were lit in a specific color (green, blue, red).
### Client message:
| hmac               | CMD_ID | DATA_LENGTH | DATA                  |
|--------------------|--------|-------------|-----------------------|
| will be calculated | 05     | 0001        | the color (G , B , R) |
### Server message:
| hmac               | CMD_ID | DATA_LENGTH        | DATA                           |
|--------------------|--------|--------------------|--------------------------------|
| will be calculated | 05     | will be calculated | num of holds ever lit by color |

## CHANGE_PASSWORD [11]
### Description:
The server will confirm once again the credentials and will change the saved password for said username.
### Client message:
| hmac               | CMD_ID | DATA_LENGTH        | DATA                                                                                       |
|--------------------|--------|--------------------|--------------------------------------------------------------------------------------------|
| will be calculated | 11     | will be calculated | LENGTH_USERNAME( 2chars ) LENGTH_ORIGINAL_PASSWORD (2chars) USERNAME ORIGINAL_PASSWORD NEW_PASSWORD |
### Server message:
| hmac               | CMD_ID | DATA_LENGTH | DATA           |
|--------------------|--------|-------------|----------------|
| will be calculated | 11     | 0007 / 0004 | SUCCESS / FAIL |

## DIFFIE_HELLMAN [95]
### Description:
A special command to coordinate key exchange.
### Client message:
| hmac       | CMD_ID | DATA_LENGTH | DATA                   |
|------------|--------|-------------|------------------------|
| 0*HMACSIZE | 95     | KEY_LENGTH  | The clients public key |
### Server message:
| hmac       | CMD_ID | DATA_LENGTH | DATA                   |
|------------|--------|-------------|------------------------|
| 0*HMACSIZE | 95     | KEY_LENGTH  | The servers public key |

## WRONG_HMAC [96]
### Description:
A special command to indicate a wrong hmac was received.
### Client message:
| hmac               | CMD_ID | DATA_LENGTH | DATA       |
|--------------------|--------|-------------|------------|
| will be calculated | 96     | 0010        | WRONG_HMAC |
### Server message:
| hmac               | CMD_ID | DATA_LENGTH | DATA       |
|--------------------|--------|-------------|------------|
| will be calculated | 96     | 0010        | WRONG_HMAC |

## CONN_NOT_SECURE [97]
### Description:
A special response the server will send if the user haven't initiated key exchange yet
### Client message:
Mayn't send this command
### Server message:
| hmac       | CMD_ID | DATA_LENGTH | DATA            |
|------------|--------|-------------|-----------------|
| 0*HMACSIZE | 97     | 0015        | CONN_NOT_SECURE |

## NOT_DEFINED [98]
### Description:
A special response the server will send if the user sent a command that isn't defined
### Client message:
Mayn't send this command
### Server message:
| hmac               | CMD_ID | DATA_LENGTH | DATA        |
|--------------------|--------|-------------|-------------|
| will be calculated | 98     | 0011        | NOT_DEFINED |

## ACCESS_DENIED [99]
### Description:
A special response the server may send if the client isn't authorized to do an action (e.g. he isn't logged in) 
### Client message:
Mayn't send this command
### Server message:
| hmac               | CMD_ID | DATA_LENGTH | DATA          |
|--------------------|--------|-------------|---------------|
| will be calculated | 99     | 0013        | ACCESS_DENIED |