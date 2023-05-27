#include <wifiServer.h>

std::set<AsyncClient *> loggedInClients;
std::map<AsyncClient *, std::string> sharedSecretMap;

/* clients events */
static void handleError(void *arg, AsyncClient *client, int8_t error)
{
    Serial.printf("\n connection error %s from client %s \n", client->errorToString(error), client->remoteIP().toString().c_str());
}

static void handleData(void *arg, AsyncClient *client, void *raw_msg, size_t len)
{
    Serial.printf("\ndata received from client: %s \n", client->remoteIP().toString().c_str());
    Serial.write((uint8_t *)raw_msg, len);
    Serial.println();

    std::string msg = (char *)raw_msg;
    std::string msgToSend;

    auto iterator = sharedSecretMap.find(client);
    // shared key hasn't been established yet
    if (iterator == sharedSecretMap.end())
    {

        auto [cmdID, data, hmacFlag] = protocol::extractFromMsg(msg);

        if (cmdID != handleRequests::DIFFIE_HELLMAN)
        {
            msgToSend = handleRequests::cmdConnNotSecure;
            client->add(msgToSend.c_str(), msgToSend.size());
            client->send();
            return;
        }

        uint8_t sharedSecret[SHARED_KEY_SIZE];
        msgToSend = handleRequests::cmdDiffieHellman(data, sharedSecret, SHARED_KEY_SIZE);


        client->add(msgToSend.c_str(), msgToSend.size());
        client->send();

        char sharedSecretHex[SHARED_KEY_SIZE * 2 + 1];
        hexUtils::uint8ToHex(sharedSecretHex, sizeof(sharedSecretHex) / sizeof(char), sharedSecret, sizeof(sharedSecret) / sizeof(uint8_t));
        Serial.printf("shared secret: %s\n", std::string(sharedSecretHex).c_str());
        sharedSecretMap.insert({client, std::string(sharedSecretHex)});
        return;
    }
    // shared secret has been established previously and keys can be derived from it
    std::string sharedSecretHex = iterator->second;
    Serial.printf("shared secret:%s\n", sharedSecretHex.c_str());

    unsigned char sharedsecret[SHARED_KEY_SIZE];
    hexUtils::hexToUint8(sharedsecret, SHARED_KEY_SIZE, sharedSecretHex.c_str());
    unsigned char aesKey[AES_KEY_SIZE];
    unsigned char hmacKey[HMAC_KEY_SIZE];

    memcpy(aesKey, sharedsecret, AES_KEY_SIZE);
    memcpy(hmacKey, &sharedsecret[AES_KEY_SIZE], HMAC_KEY_SIZE);

    msg = encryption::decryptMsg(msg, aesKey);
    auto [cmdID, data, hmacFlag] = protocol::extractFromMsg(msg, hmacKey);
    Serial.printf("deciphered msg: %s\n", msg.c_str());
    Serial.printf("cmdID: %d\ndata: %s\nhmacFlag: %s\n", cmdID, data.c_str(), hmacFlag ? "true" : "false");
    if (!hmacFlag)
    {
        msgToSend = handleRequests::cmdWrongHmac;
        msgToSend = protocol::composeSecureMsg(msgToSend, hmacKey, aesKey);
        client->add(msgToSend.c_str(), msgToSend.size());
        client->send();
        return;
    }

    // user isn't logged in, and isn't logging in
    if (loggedInClients.find(client) == loggedInClients.end() && cmdID != handleRequests::LOGIN)
    {
        msgToSend = handleRequests::cmdAccessDenied;
        msgToSend = protocol::composeSecureMsg(msgToSend, hmacKey, aesKey);
        client->add(msgToSend.c_str(), msgToSend.size());
        client->send();
        return;
    }

    switch (cmdID)
    {
    // TODO: implement the commands and then uncomment
    case handleRequests::LOGIN:
        bool success;
        std::tie(msgToSend, success) = handleRequests::cmdLogin(data);
        if (success)
        {
            loggedInClients.insert(client);
        }
        break;

    case handleRequests::CHANGE_PASSWORD:
        msgToSend = handleRequests::cmdChangePassword(data);
        break;

    case handleRequests::UPTIME:
        msgToSend = handleRequests::cmdUptime();
        break;
        case handleRequests::NUM_CONNECTIONS:
        	msgToSend = handleRequests::cmdNumConns();
        	break;
        case handleRequests::NUM_ROUTES:
        	msgToSend = handleRequests::cmdNumRoutes();
        	break;
        case handleRequests::NUM_TIMES_HOLD_LIT:
        	msgToSend = handleRequests::cmdNumHoldLit(data);
        	break;
        case handleRequests::NUM_TIMES_COLOR_LIT:
        	msgToSend = handleRequests::cmdNumColorLit(data);
        break;
    case handleRequests::DIFFIE_HELLMAN:
        break;
    case handleRequests::NOT_DEFINED:
        msgToSend = handleRequests::cmdAccessDenied;
        break;
    default:
        msgToSend = handleRequests::cmdNotDefined;
        break;
    }
    Serial.printf("msg to send: %s\n",msgToSend.c_str());
    msgToSend = protocol::composeSecureMsg(msgToSend, hmacKey, aesKey);
    client->add(msgToSend.c_str(), msgToSend.size());
    client->send();
}

static void handleDisconnect(void *arg, AsyncClient *client)
{
    Serial.printf("\n client %s disconnected \n", client->remoteIP().toString().c_str());
    loggedInClients.erase(client);
    client->close(true);
    client->free();
    delete client;
}

static void handleTimeOut(void *arg, AsyncClient *client, uint32_t time)
{
    Serial.printf("\n client ACK timeout ip: %s \n", client->remoteIP().toString().c_str());
}

/* server events */
static void handleNewClient(void *arg, AsyncClient *client)
{
    Serial.printf("\n new client has been connected to server, ip: %s", client->remoteIP().toString().c_str());

    // register events
    client->onData(&handleData, NULL);
    client->onError(&handleError, NULL);
    client->onDisconnect(&handleDisconnect, NULL);
    client->onTimeout(&handleTimeOut, NULL);
}

void wifiServerMain()
{
    Serial.begin(115200);
    delay(20);

    // create access point
    while (!WiFi.softAP(SSID, PASSWORD, 6, false, 15))
    {
        delay(500);
    }


    storage::setupGeneralStatsStorage();
    storage::setupPasswordStorage();
    
    AsyncServer *server = new AsyncServer(TCP_PORT); // start listening on tcp port 7050
    server->onClient(&handleNewClient, server);
    server->begin();
}

// void wifiServerLoop(void * pvParameters)
// {
//     while (1)
//     {
//         Serial.println("loop");
//         delay(100);
//     }
    
// }