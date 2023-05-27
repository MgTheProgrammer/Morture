#pragma once
#include <WiFi.h>
#include <AsyncTCP.h>
#include <string>
#include <set>
#include <map>

#include "wifiConfig.h"
#include <handleRequests/handleRequests.h>
#include <hexUtils/hexUtils.h>
#include <storage.h>



static void handleError(void *arg, AsyncClient *client, int8_t error);
static void handleData(void *arg, AsyncClient *client, void *raw_msg, size_t len);
static void handleDisconnect(void *arg, AsyncClient *client);
static void handleTimeOut(void *arg, AsyncClient *client, uint32_t time);
static void handleNewClient(void *arg, AsyncClient *client);
void wifiServerMain();
// void wifiServerLoop(void * pvParameters);
