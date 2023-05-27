#include "handleRequests.h"
namespace handleRequests
{

  std::string cmdUptime()
  {
    int64_t microseconds = esp_timer_get_time();
    std::string data = std::to_string(microseconds);
    return protocol::composeBareMsg(UPTIME, data);
  }

  std::string cmdNumConns()
  {
    uint32_t numConns = storage::getNumConns();
    return protocol::composeBareMsg(NUM_CONNECTIONS, std::to_string(numConns));
  }

  std::string cmdNumRoutes()
  {
    uint32_t numRoutes = storage::getNumRoutes();
    return protocol::composeBareMsg(NUM_ROUTES, std::to_string(numRoutes));
  }

  std::string cmdNumHoldLit(std::string data)
  {
    std::string holdsLitString = storage::getHoldsLit();
    return protocol::composeBareMsg(NUM_TIMES_HOLD_LIT, holdsLitString);
  }

  std::string cmdNumColorLit(std::string data)
  {
    char color = data.at(0);
    int numLit;
    switch (color)
    {
    case 'R':
      numLit = storage::getNumRed();
      break;
    case 'G':
      numLit = storage::getNumGreen();
      break;
    case 'B':
      numLit = storage::getNumBlue();
      break;
    default:
      numLit = -1;
      break;
    }
    return protocol::composeBareMsg(NUM_TIMES_COLOR_LIT, std::to_string(numLit));
  }

  std::string cmdDiffieHellman(std::string clientPublicKey, uint8_t *output, size_t outlen)
  {
    uint8_t serverPublicKey[SHARED_KEY_SIZE];
    uint8_t serverPrivateKey[SHARED_KEY_SIZE];

    Curve25519::dh1(serverPublicKey, serverPrivateKey); // generates a private key and subsequently calculates a public key
    char serverPublicKeyHex[sizeof(serverPublicKey) * 2 + 1];

    hexUtils::uint8ToHex(serverPublicKeyHex, sizeof(serverPublicKeyHex) / sizeof(char), serverPublicKey, sizeof(serverPublicKey) / sizeof(uint8_t));

    Serial.println("server key:");
    Serial.println(serverPublicKeyHex);
    std::string msg = protocol::composeBareMsg(DIFFIE_HELLMAN, std::string(serverPublicKeyHex));

    Serial.println("client public key:");
    Serial.println(clientPublicKey.c_str());
    uint8_t clientPublicKeyBytes[SHARED_KEY_SIZE];
    hexUtils::hexToUint8(clientPublicKeyBytes, SHARED_KEY_SIZE, clientPublicKey.c_str());

    Curve25519::dh2(clientPublicKeyBytes, serverPrivateKey); // calculates the shared secret and saves it in clientPublicKey

    memmove(output, clientPublicKeyBytes, min(sizeof(clientPublicKeyBytes), outlen)); // moves the shared secret into output

    return (std::string(HASH_SIZE * 2, HMAC_PLACEHOLDER) + msg);
  }
  std::pair<std::string, bool> cmdLogin(std::string dataField)
  {
    bool success = false;
    try
    {
      Preferences preferences;
      preferences.begin(PASSWORD_NAMESPACE);

      int usernameLen = std::stoi(dataField.substr(0, USERNAME_LEN_FIELD_WIDTH));
      dataField.erase(0, USERNAME_LEN_FIELD_WIDTH);

      std::string username = dataField.substr(0, usernameLen);
      dataField.erase(0, usernameLen);

      std::string password = dataField;

      if (preferences.isKey(username.c_str()))
      {
        std::string savedHash = std::string(preferences.getString(username.c_str()).c_str());
        success = isPasswordCorrect(password, savedHash);
      }
      preferences.end();
    }
    catch (const std::exception &e)
    {
    }

    std::string dataToSend = success ? "SUCCESS" : "FAIL";
    return {protocol::composeBareMsg(LOGIN, dataToSend), success};
  }
  std::string cmdChangePassword(std::string dataField)
  {
    bool success = false;
    try
    {
      Preferences preferences;
      preferences.begin(PASSWORD_NAMESPACE);

      int usernameLen = std::stoi(dataField.substr(0, USERNAME_LEN_FIELD_WIDTH));
      dataField.erase(0, USERNAME_LEN_FIELD_WIDTH);

      int passwordLen = std::stoi(dataField.substr(0, PASSWORD_LEN_FIELD_WIDTH));
      dataField.erase(0, PASSWORD_LEN_FIELD_WIDTH);

      std::string username = dataField.substr(0, usernameLen);
      dataField.erase(0, usernameLen);

      std::string oldPassword = dataField.substr(0, passwordLen);
      dataField.erase(0, passwordLen);

      std::string newPassword = dataField;

      if (preferences.isKey(username.c_str()))
      {
        std::string savedHash = std::string(preferences.getString(username.c_str()).c_str());
        if (isPasswordCorrect(oldPassword, savedHash))
        {
          preferences.putString(username.c_str(), encryption::hashAndSalt(newPassword).c_str());
          success = true;
        }
      }
      preferences.end();
    }
    catch (const std::exception &e)
    {
    }
    std::string dataToSend = success ? "SUCCESS" : "FAIL";
    return protocol::composeBareMsg(CHANGE_PASSWORD, dataToSend);
  }

  bool isPasswordCorrect(std::string password, std::string savedHashAndSalt)
  {
    std::string savedHash = savedHashAndSalt.substr(0, HASH_SIZE * 2);
    std::string savedSalt = savedHashAndSalt.substr(HASH_SIZE * 2);
    std::string calculatedHash = encryption::hash(password + savedSalt);

    return (savedHash == calculatedHash);
  }
}