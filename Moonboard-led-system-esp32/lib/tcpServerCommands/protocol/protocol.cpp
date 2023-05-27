#include "protocol.h"

namespace protocol{
    std::tuple<int, std::string, bool> extractFromMsg(std::string msg)
  {
    std::string data;
    std::string receivedHmac;
    int cmdID;
    int dataLen;
    try
    {
      receivedHmac = msg.substr(0, HASH_SIZE);
      msg.erase(0, HASH_SIZE);

      cmdID = std::stoi(msg.substr(0, ID_FIELD_WIDTH));
      msg.erase(0, ID_FIELD_WIDTH);

      dataLen = std::stoi(msg.substr(0, DATA_LEN_FIELD_WIDTH));
      msg.erase(0, DATA_LEN_FIELD_WIDTH);

      data = msg.substr(0, dataLen);
    }
    catch (const std::exception &e)
    {
      cmdID = -1;
      data = std::string("NONE");
    }

    return {cmdID, data, false};
  }
  std::tuple<int, std::string, bool> extractFromMsg(std::string msg, unsigned char *hmacKey)
  {
    std::string data;
    std::string receivedHmac;
    int cmdID;
    int dataLen;
    std::string calculatedHmac;
    bool isHmacCorrect;
    try
    {
      receivedHmac = msg.substr(0, HASH_SIZE * 2);
      msg.erase(0, HASH_SIZE * 2);
      calculatedHmac = encryption::hmac(msg, hmacKey);

      cmdID = std::stoi(msg.substr(0, ID_FIELD_WIDTH));
      msg.erase(0, ID_FIELD_WIDTH);

      dataLen = std::stoi(msg.substr(0, DATA_LEN_FIELD_WIDTH));
      msg.erase(0, DATA_LEN_FIELD_WIDTH);

      data = msg.substr(0, dataLen);

      transform(calculatedHmac.begin(), calculatedHmac.end(), calculatedHmac.begin(), ::tolower);
      transform(receivedHmac.begin(), receivedHmac.end(), receivedHmac.begin(), ::tolower);
      isHmacCorrect = (calculatedHmac == receivedHmac);
    }
    catch (const std::exception &e)
    {
      cmdID = -1;
      data = std::string("NONE");
      isHmacCorrect = false;
    }

    return {cmdID, data, isHmacCorrect};
  }

  std::string composeBareMsg(int cmdID, std::string data)
  {
    std::string id = std::to_string(cmdID);
    std::string dataLen = std::to_string(data.size());

    stringUtils::padStringTo(id, ID_FIELD_WIDTH);
    stringUtils::padStringTo(dataLen, DATA_LEN_FIELD_WIDTH);

    return (id + dataLen + data);
  }
  std::string composeSecureMsg(std::string msg, unsigned char *hmacKey, unsigned char *aesKey)
  {
    std::string toSend = (encryption::hmac(msg, hmacKey) + msg);
    return encryption::encryptMsg(toSend, aesKey);
  }
}