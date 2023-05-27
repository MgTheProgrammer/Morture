#include <Color.h>
#include <Arduino.h>
#include <stringUtils/stringUtils.h>
#include <printUtils/formatColor.h>
#include <Hold.h>

#define NUM_ROWS 18
#define NUM_COLS 11
class BoardVisual
{
    public:
    Color board[NUM_ROWS][NUM_COLS];
    /// @brief takes a moonboard hold and puts the color on the board in the matching place
    /// @param hold a hold containing both index and color
    void enterHoldAtIndex(Hold hold);

    /// @brief prints the board nicely
    void printBoard();

    /// @brief clears the board and put Color::none in every position
    void clearBoard();
};