#include <board.h>

void BoardVisual::enterHoldAtIndex(Hold hold)
{
    int index = hold.index;
    int col = (index / NUM_ROWS);
    int row = index % NUM_ROWS;
    if (col % 2 == 0)
    {
        row = NUM_ROWS - row - 1;
    }
    board[row][col] = hold.color;
}

void BoardVisual::printBoard()
{
    Serial.println();
    // print column legend
    for (int i = 0; i < NUM_COLS; i++)
    {
        char colLabel = (char)(i + 65);
        Serial.print(formatColor::formatPurple(std::string(1, colLabel)).c_str());
        Serial.print(" ");
    }
    Serial.println();
    // print board
    for (int i = 0; i < NUM_ROWS; i++)
    {
        for (int j = 0; j < NUM_COLS; j++)
        {
            std::string c;
            switch (board[i][j])
            {
            case Color::red:
                c = formatColor::formatRed("r");
                break;
            case Color::green:
                c = formatColor::formatGreen("g");
                break;
            case Color::blue:
                c = formatColor::formatBlue("b");
                break;
            default:
                c = "0";
                break;
            }
            Serial.print(c.c_str());
            Serial.print(" ");
        }
        // print row legend
        std::string rowLabel = std::to_string(NUM_ROWS - i);
        stringUtils::padStringTo(rowLabel, 2);
        Serial.print(formatColor::formatPurple(rowLabel).c_str());
        Serial.println();
    }
}
void BoardVisual::clearBoard()
{
    for (int i = 0; i < NUM_ROWS; i++)
    {
        for (int j = 0; j < NUM_COLS; j++)
        {
            board[i][j] = Color::none;
        }
    }
}

