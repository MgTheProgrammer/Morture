// #include <Arduino.h>
// #include "../lib/CommandParser/Hold.h"
// #include "../lib/CommandParser/CommandParser.h"
// #include <optional>

// void test_create_hold_queue(std::string packet, int size, Hold expectedHolds[])
// {
//     CommandParser parser = CommandParser();
//     parser.CreateHoldQueue(packet);
//     for (size_t i = 0; i < size; i++)
//     {
//         Serial.print("hold ");
//         Serial.println(i+1);
//         std::optional<Hold> recivedHold = parser.getHold();
//         if (expectedHolds[i].holdColor == recivedHold.value().holdColor)
//         {
//             Serial.println("Color OK");
//         }
//         else
//         {
//             Serial.println("Color BAD");
//             Serial.print("expected: ");

//             Serial.println(expectedHolds[i].holdColor);
//             Serial.print(" recived: ");
//             Serial.print(recivedHold.value().holdColor);
//         }
//         if (expectedHolds[i].holdIndex == recivedHold.value().holdIndex)
//         {
//             Serial.println("Index OK");
//         }
//         else
//         {
//             Serial.println("Index BAD");
//             Serial.print("expected: ");
//             Serial.print(expectedHolds[i].holdIndex);
//             Serial.print(" recived: ");
//             Serial.print(recivedHold.value().holdIndex);
//         }
//         Serial.println();
//     }
//     Serial.println("Test ended");
// }
// void setup()
// {
//     Serial.begin(115200);
    
//     Hold questForPower[6] = {Hold(Color::green,148),Hold(Color::green,104),Hold(Color::blue,192),Hold(Color::blue,159),Hold(Color::blue,135),Hold(Color::red,126)};
//     test_create_hold_queue("6c23533134382c533130342c503139322c503135392c503133352c4531323623",sizeof(questForPower)/sizeof(Hold), questForPower);
// }

// void loop()
// {
// }