#pragma once
#include <Color.h>
#include <Preferences.h>
#include "storageConfig.h"
#include <stringUtils/stringUtils.h>
#include <encryption.h>

namespace storage{
    /// @brief checks if the storage space in which passwords are stored has already been initialised. if not, will initialise said storage space with default password
    void setupPasswordStorage();
    
    /// @brief checks if the storage space in which passwords are stored has already been initialised. if not, will initialise said storage space with default values of the stats
    void setupGeneralStatsStorage();

    /// @brief increases the saved number of ble connections by one
    void addConnection();
    /// @brief increases the saved number of routes ever lit by one
    void addRoute();
    
    /// @brief increase the saved number of times a specific hold was lit by one
    /// @param index the moonboard index of the hold to increase
    void addHold(int index);
    
    /// @brief increase the saved number of times holds were lit in a specific color by one
    /// @param holdColor the color to increase its stats
    void addColor(Color holdColor);

    /// @return the saved number of ble connections to ever happen
    uint32_t getNumConns();
    
    /// @return the saved number of routes ever lit
    uint32_t getNumRoutes();
    
    /// @return the saved number of times a hold was lit in red
    uint32_t getNumRed();
    /// @return the saved number of times a hold was lit in green
    uint32_t getNumGreen();
    /// @return the saved number of times a hold was lit in blue
    uint32_t getNumBlue();
    /// @brief retrieves the saved string indicating the number of times each hold was ever lit, the string will be something like "0;6;4;10;" where each number is the total amount of times the matching hold was lit, so following the example the 0 index hold was never lit, the 1st index was lit 6 times and so on.
    /// @return the saved string indicating the number of times each hold was ever lit
    std::string getHoldsLit();
}