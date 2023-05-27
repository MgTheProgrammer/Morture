#pragma once
#include <Arduino.h>
#include <string>
#include "Color.h"
#include "Hold.h"
#include <optional>

class CommandParser
{
private:
    QueueHandle_t holdsQueue;

    /// @brief takes a string describing one hold and returns the matching index of led to lit according to moonboard way of connecting led
    /// @param holdString string describing one hold, e.g "503130"
    /// @return the index of led to lit
    /// @note for example "503130" which indicates A11(B) on the Moonboard will return 10 as A11 is the 11th hold on the strip of leds
    int HoldStringToIndex(std::string holdString);

    /// @brief takes a string describing one hold and returns the matching color
    /// @param holdString a string describing one hold, e.g "503130"
    /// @return the Color corresponding with the string
    /// @note for example "503130" will return blue as the prefix is equal to 50 which indicates blue
    Color HoldStringToColor(std::string holdString);

    /// @brief pushes a hold object to queue after finding the right color and index from string
    /// @param holdString a string describing one hold, e.g "503130"
    void HandleHold(std::string holdString);

    /// @brief gets a string describing a whole moonboard route and checks if prefix and suffix present
    /// @param routeString a string describing a whole moonboard route
    /// @return true if prefix and suffix present, false otherwise
    bool IsValidRoute(std::string routeString);
    


public:
    const std::string msgPrefix; // the string indicating the start of a route
    const std::string msgSuffix; // the string indicating the end of a route
    const std::string msgHoldSeperator; // the string separating between two holds

    /// @brief a constructor, will create the queue.
    /// @param numHolds the maximum number of holds in one route (will be used for creating the queue)
    // / @param prefix the string indicating the start of a route, will default to "6c23"
    /// @param suffix the string indicating the end of a route, will default to "23"
    /// @param seperator the string separating between two holds, will default to "2c"
    CommandParser(int numHolds = 14, std::string prefix = "6c23", std::string suffix = "23", std::string seperator = "2c");

    /// @brief tries to get a hold from the queue
    /// @return an optional<Hold> if present in the queue, std::nullopt if there is no more holds on queue
    std::optional<Hold> getHold();
    
    /// @brief handles a whole moonboard route, prepares the queue to be read from, and finally returns the number of holds the route has
    /// @param routeString a string describing a whole moonboard route
    /// @return the number of holds the route has
    int CreateHoldQueue(std::string routeString);
};