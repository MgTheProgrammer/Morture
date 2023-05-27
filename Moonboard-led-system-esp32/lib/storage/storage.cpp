#include "storage.h"
namespace storage
{
    void setupPasswordStorage()
    {
        Preferences preferences;
        preferences.begin(PASSWORD_NAMESPACE); // opens (or creates) a namespace in which i can store values permanently
        if (!preferences.isKey(DEFAULT_USERNAME))
        {
            // the namespace have never been initialised before thus initialize the master account
            preferences.putString(DEFAULT_USERNAME, encryption::hashAndSalt(DEFAULT_PASSWORD).c_str());
        }
        preferences.end();
    }

    void setupGeneralStatsStorage()
    {
        Preferences preferences;
        preferences.begin(STATS_NAMESPACE);
        if (!preferences.isKey(NUM_CONNS_KEY))
        {
            preferences.putUInt(NUM_CONNS_KEY, 0);
            preferences.putUInt(NUM_ROUTES_KEY, 0);
            preferences.putUInt(NUM_RED_KEY, 0);
            preferences.putUInt(NUM_GREEN_KEY, 0);
            preferences.putUInt(NUM_BLUE_KEY, 0);

            std::string numHoldLitString = "";
            for (size_t i = 0; i < MOONBOARD_NUM_HOLDS; i++)
            {
                numHoldLitString += '0';
                numHoldLitString += HOLD_LIT_SEPARATOR;
            }

            preferences.putString(NUM_HOLD_KEY, numHoldLitString.c_str());
        }
        preferences.end();
    }
    void addConnection()
    {
        Preferences preferences;
        preferences.begin(STATS_NAMESPACE);
        uint32_t count = preferences.getUInt(NUM_CONNS_KEY);
        preferences.putUInt(NUM_CONNS_KEY, ++count);
        preferences.end();
    }
    void addRoute()
    {
        Preferences preferences;
        preferences.begin(STATS_NAMESPACE);
        uint32_t count = preferences.getUInt(NUM_ROUTES_KEY);
        preferences.putUInt(NUM_ROUTES_KEY, ++count);
        preferences.end();
    }
    void addHold(int index)
    {
        Preferences preferences;
        preferences.begin(STATS_NAMESPACE);
        std::string savedData = preferences.getString(NUM_HOLD_KEY).c_str();
        savedData = stringUtils::increaseAtIndex(savedData, index, HOLD_LIT_SEPARATOR);
        preferences.putString(NUM_HOLD_KEY, savedData.c_str());
        preferences.end();
    }
    void addColor(Color holdColor)
    {
        Preferences preferences;
        preferences.begin(STATS_NAMESPACE);
        uint32_t count;
        switch (holdColor)
        {
        case Color::red:
            count = preferences.getUInt(NUM_RED_KEY);
            preferences.putUInt(NUM_RED_KEY, ++count);
            break;
        case Color::green:
            count = preferences.getUInt(NUM_GREEN_KEY);
            preferences.putUInt(NUM_GREEN_KEY, ++count);
            break;
        case Color::blue:
            count = preferences.getUInt(NUM_BLUE_KEY);
            preferences.putUInt(NUM_BLUE_KEY, ++count);
            break;
        default:
            break;
        }
        preferences.end();
    }
    uint32_t getNumConns()
    {
        Preferences preferences;
        preferences.begin(STATS_NAMESPACE, true);

        uint32_t count = preferences.getUInt(NUM_CONNS_KEY);
        preferences.end();
        return count;
    }
    uint32_t getNumRoutes()
    {
        Preferences preferences;
        preferences.begin(STATS_NAMESPACE, true);

        uint32_t count = preferences.getUInt(NUM_ROUTES_KEY);
        preferences.end();
        return count;
    }
    uint32_t getNumRed()
    {
        Preferences preferences;
        preferences.begin(STATS_NAMESPACE, true);

        uint32_t count = preferences.getUInt(NUM_RED_KEY);
        preferences.end();
        Serial.printf("num lit %d\n",count);
        return count;
    }
    uint32_t getNumGreen()
    {
        Preferences preferences;
        preferences.begin(STATS_NAMESPACE, true);

        uint32_t count = preferences.getUInt(NUM_GREEN_KEY);
        preferences.end();
        return count;
    }
    uint32_t getNumBlue()
    {
        Preferences preferences;
        preferences.begin(STATS_NAMESPACE, true);

        uint32_t count = preferences.getUInt(NUM_BLUE_KEY);
        preferences.end();
        return count;
    }
    std::string getHoldsLit()
    {
        Preferences preferences;
        preferences.begin(STATS_NAMESPACE, true);

        const char *storedDataC = preferences.getString(NUM_HOLD_KEY).c_str();
        preferences.end();
        return storedDataC;
    }
}