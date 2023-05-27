#pragma once
#include <Arduino.h>
#include <string>
#include "handleRequestsConfig.h"
#include <Curve25519.h>

#include <Preferences.h>
#include <hexUtils/hexUtils.h>
#include <encryption.h>
#include <protocol/protocol.h>
#include "storage.h"
namespace handleRequests
{
    enum {
        LOGIN = 0, UPTIME = 1, NUM_CONNECTIONS = 2, NUM_ROUTES = 3, NUM_TIMES_HOLD_LIT = 4, NUM_TIMES_COLOR_LIT = 5,CHANGE_PASSWORD = 11, DIFFIE_HELLMAN = 95,WRONG_HMAC = 96,CONN_NOT_SECURE = 97,NOT_DEFINED = 98 ,ACCESS_DENIED = 99
    };

   
    /// @brief will take the data field of a login request and check if login credentials are saved and correct. will return a response as described in the protocol (without hmac field) matching the findings (either FAIL or SUCCESS response)
    /// @param dataField the data field from the login request
    /// @return (a bare response to the client (without hmac and yet to be encrypted),is login successful)
    std::pair<std::string,bool> cmdLogin(std::string dataField);

    /// @brief generates a bare (without hmac field and yet to be encrypted) response to the UPTIME command specified in the protocol.
    /// @return a bare UPTIME response
    std::string cmdUptime();
    
    /// @brief generates a bare (without hmac field and yet to be encrypted) response to the NUM_CONNECTIONS command specified in the protocol.
    /// @return a bare NUM_CONNECTIONS response
    std::string cmdNumConns();
    
    /// @brief generates a bare (without hmac field and yet to be encrypted) response to the NUM_ROUTES command specified in the protocol.
    /// @return a bare NUM_ROUTES response
    std::string cmdNumRoutes();

    /// @brief generates a bare (without hmac field and yet to be encrypted) response to the NUM_TIMES_HOLD_LIT command specified in the protocol.
    /// @param data the index of the hold to check
    /// @return a bare NUM_TIMES_HOLD_LIT response
    std::string cmdNumHoldLit(std::string data);

    /// @brief generates a bare (without hmac field and yet to be encrypted) response to the NUM_TIMES_COLOR_LIT command specified in the protocol.
    /// @param data the color to check.
    /// @return a bare NUM_TIMES_COLOR_LIT response
    std::string cmdNumColorLit(std::string data);
    
    /// @brief will take the data field of a change password request and check if login credentials are saved and correct. if they are, will change the password saved to the new password. will return a response as described in the protocol (either FAIL or SUCCESS response) (without hmac field)
    /// @param dataField the datafield from the change password request
    /// @return a bare response to the client (without hmac and yet to be encrypted) stating whether the process succeeded
    std::string cmdChangePassword(std::string dataField);
    
    /// @brief gets a client public key and will preform the steps of ECDHE needed to generate a shared secret additionally will compose a response to the client (the msg is not encrypted and ready to be sent), the shared secret will be saved into output. 
    /// @param clientPublicKey a hex string representation of the client public key
    /// @param output the buffer to store the shared secret into
    /// @param outlen the length of the buffer
    /// @return the response to the client (the msg is not encrypted and ready to be sent)
    std::string cmdDiffieHellman(std::string clientPublicKey, uint8_t *output, size_t outlen);
    
    /// @brief a bare WRONG_HMAC response according to protocol
    const std::string cmdWrongHmac = protocol::composeBareMsg(WRONG_HMAC,"WRONG_HMAC");
    /// @brief a bare CONN_NOT_SECURE response according to protocol
    const std::string cmdConnNotSecure = protocol::composeBareMsg(CONN_NOT_SECURE,"CONN_NOT_SECURE");
    /// @brief a bare NOT_DEFINED response according to protocol
    const std::string cmdNotDefined = protocol::composeBareMsg(NOT_DEFINED,"NOT_DEFINED");
    /// @brief a bare ACCESS_DENIED response according to protocol
    const std::string cmdAccessDenied = protocol::composeBareMsg(ACCESS_DENIED,"ACCESS_DENIED");

    /// @brief checks whether given password is the same as the password that was hashed and salted
    /// @param password the password to check
    /// @param savedHashAndSalt the saved hash (and salt) of the corresponding password that is being checked
    /// @return true if the given password is the same as the one saved, false otherwise
    bool isPasswordCorrect(std::string password, std::string savedHashAndSalt);
    
}