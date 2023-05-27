#pragma once
#include <tuple>
#include <string>
#include <config.h>
#include <stringUtils/stringUtils.h>
#include <encryption.h>
#include "protocolConfig.h"

namespace protocol{
    /// @overload
    /// @brief takes a message constructed according to protocol and extracts cmd id and data fields. This version of the functions assumes keys have yet to be exchanged therfore hmac is ignored
    /// @param msg msg constructed according to protocol
    /// @return  cmd id , data field , false. if the proccess fails will return cmdID = -1, data = "NONE"
    std::tuple<int, std::string, bool> extractFromMsg(std::string msg);
    
    /// @brief takes a message constructed according to protocol and extracts cmd id and data fields. In addition will calculate hmac and will compare it to received hmac
    /// @param msg msg constructed according to protocol
    /// @param hmacKey the hmac key which the original hmac was calculated with
    /// @return  cmd id , data field , isHmac correct. if the proccess fails will return cmdID = -1, data = "NONE", isHmac correct = false
    std::tuple<int, std::string, bool> extractFromMsg(std::string msg, unsigned char *hmacKey);

    /// @brief takes cmdId and data field and will construct a message according to protocol (without hmac field)
    /// @param cmdID the id of the command sent agreed upon in the protocol (should have no more digits than ID_FIELD_WIDTH in config file)
    /// @param data the data field to add to the message, (accepted length is no more than the number that can fit inside a DATA_LEN_FIELD_WIDTH e.g. if width is 4 should be shorter than 9999)
    /// @return a msg fitting the protocol (without hmac field)
    std::string composeBareMsg(int cmdID,std::string data);
    
    /// @brief adds hmac to given message and then encrypts the message, making it ready to be sent
    /// @param msg a message already fitted to protocol (without hmac field)
    /// @param hmacKey the key to calculate hmac with
    /// @param aesKey the key to encrypt the message with
    /// @return an encrypted message that is ready to be sent
    std::string composeSecureMsg(std::string msg,unsigned char* hmacKey,unsigned char* aesKey);

}