#include "CommandParser.h"

using namespace std;

/// @brief takes a string describing one hold and returns the matching index of led to lit according to moonboard way of connecting led
/// @param holdString string describing one hold, e.g "503130"
/// @return the index of led to lit, -1 if the string is broken
/// @note for example "503130" which indicates A11(B) on the Moonboard will return 10 as A11 is the 11th hold on the strip of leds
int CommandParser::HoldStringToIndex(string holdString)
{
    holdString.erase(0, 2); // removes the first two chars describing the color
    string new_string = "";

    for (int i = 0; i < holdString.length(); i++)
    { // The string by this point is something like this 313230 and im interested in every second digit
        if (i % 2 == 1)
        {
            new_string += holdString[i];
        }
    }
    int index =0;
    try{
        index = stoi(new_string);
    }
    catch(exception &err) {
        index = -1;
    }
    return index;
}

/// @brief takes a string describing one hold and returns the matching color
/// @param holdString a string describing one hold, e.g "503130"
/// @return the Color corresponding with the string
/// @note for example "503130" will return blue as the prefix is equal to 50 which indicates blue
Color CommandParser::HoldStringToColor(string holdString)
{
    int colorFlag = stoi(holdString.substr(0, 2)); // Takes the first two chars as they are the part that indicates color in moonboard's protocol
    switch (colorFlag)
    {
    case red:
        return Color::red;
    case green:
        return Color::green;
    case blue:
        return Color::blue;
    default:
        break;
    }
    return Color::none;
}

/// @brief pushes a hold object to queue after finding the right color and index from string
/// @param holdString a string describing one hold, e.g "503130"
void CommandParser::HandleHold(string holdString)
{
    Color color = HoldStringToColor(holdString);
    int index = HoldStringToIndex(holdString);
    xQueueSend(holdsQueue, new Hold(color, index), portMAX_DELAY);
}

/// @brief handles a whole moonboard route, prepares the queue to be read from, and finally returns the number of holds the route has
/// @param routeString a string describing a whole moonboard route
/// @return the number of holds the route has, if the route isn't valid will return -1
int CommandParser::CreateHoldQueue(string routeString)
{
    int holdCounter =  0;
    if (!IsValidRoute(routeString))
    {
        return -1;
    }
    // erase prefix and suffix before splitting the string into holds
    routeString.erase(0, msgPrefix.size());
    routeString.erase(routeString.size() - msgSuffix.size(), msgSuffix.size());

    // split the string by the separator and handle one hold at a time
    size_t pos = 0;
    string hold;
    while ((pos = routeString.find(msgHoldSeperator)) != string::npos)
    {
        hold = routeString.substr(0, pos);
        HandleHold(hold);
        routeString.erase(0, pos + msgHoldSeperator.length());
        holdCounter++;
    }
    HandleHold(routeString);
    return ++holdCounter;
}

/// @brief tries to get a hold from the queue
/// @return an optional<Hold> if present in the queue, std::nullopt if there is no more holds on queue
optional<Hold> CommandParser::getHold()
{
    Hold lastHold;
    if (xQueueReceive(holdsQueue, &lastHold, portMAX_DELAY) == pdTRUE)
    {
        return lastHold;
    }
    return nullopt;
}

CommandParser::CommandParser(int numHolds, string prefix, string suffix, string seperator) : msgPrefix(prefix), msgSuffix(suffix), msgHoldSeperator(seperator)
{
    holdsQueue = xQueueCreate(numHolds, sizeof(struct Hold));
}

/// @brief gets a string describing a whole moonboard route and checks if prefix and suffix present
/// @param routeString a string describing a whole moonboard route
/// @return true if prefix and suffix present, false otherwise
bool CommandParser::IsValidRoute(string routeString)
{
    if (routeString.substr(0, msgPrefix.size()) != msgPrefix)
    { // starts with
        return false;
    }
    if (routeString.substr(routeString.size() - msgSuffix.size(), msgSuffix.size()) != msgSuffix)
    { // ends with
        return false;
    }
    return true;
}