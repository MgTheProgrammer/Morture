#include <Arduino.h>
#include "../lib/CommandParser/CommandParser.h"
#include <unity.h>
#include "../lib/CommandParser/Hold.h"
#include <optional>

void test_create_hold_queue(std::string packet,int size,Hold expectedHolds[]){
    CommandParser parser = CommandParser();
    parser.CreateHoldQueue(packet);
    for (size_t i = 0; i < size; i++)
    {
        UnityPrint("Hold ");
        UnityPrint(std::to_string(i+1).c_str());
        std::optional<Hold> recivedHold = parser.getHold();
        TEST_ASSERT_EQUAL(expectedHolds[i].holdColor,recivedHold.value().holdColor);
        TEST_ASSERT_EQUAL(expectedHolds[i].holdIndex,recivedHold.value().holdIndex);

    }
}
void create_hold_main(){
    Hold questForPower[6] = {Hold(Color::green,148),Hold(Color::green,104),Hold(Color::blue,192),Hold(Color::blue,159),Hold(Color::blue,135),Hold(Color::red,126)};
    test_create_hold_queue("6c23533134382c533130342c503139322c503135392c503133352c4531323623",sizeof(questForPower)/sizeof(Hold), questForPower);
}
void setup()
{
    Serial.begin(115200);
    UNITY_BEGIN();
    RUN_TEST(create_hold_main);
    UNITY_END();
}
void loop()
{
    
}
