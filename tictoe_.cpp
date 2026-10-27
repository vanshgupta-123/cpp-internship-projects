#include <windows.h>
#include <string>

#define BTN_START 100
#define BTN_RESET 101

HWND buttons[3][3];

char board[3][3];
bool playerXTurn = true;
bool gameOver = false;

// Initialize board
void initializeBoard()
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            board[i][j] = ' ';
        }
    }

    playerXTurn = true;
    gameOver = false;
}

// Check winner
bool checkWinner(char player)
{
    // Rows
    for (int i = 0; i < 3; i++)
    {
        if (board[i][0] == player &&
            board[i][1] == player &&
            board[i][2] == player)
        {
            return true;
        }
    }

    // Columns
    for (int i = 0; i < 3; i++)
    {
        if (board[0][i] == player &&
            board[1][i] == player &&
            board[2][i] == player)
        {
            return true;
        }
    }

    // Diagonal
    if (board[0][0] == player &&
        board[1][1] == player &&
        board[2][2] == player)
    {
        return true;
    }

    if (board[0][2] == player &&
        board[1][1] == player &&
        board[2][0] == player)
    {
        return true;
    }

    return false;
}

// Check draw
bool checkDraw()
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (board[i][j] == ' ')
            {
                return false;
            }
        }
    }

    return true;
}

// Reset game
void resetGame()
{
    initializeBoard();

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            SetWindowTextA(buttons[i][j], "");
        }
    }
}

// Handle button click
void playMove(int row, int col)
{
    if (gameOver || board[row][col] != ' ')
    {
        return;
    }

    char currentPlayer;

    if (playerXTurn)
        currentPlayer = 'X';
    else
        currentPlayer = 'O';

    board[row][col] = currentPlayer;

    std::string text(1, currentPlayer);

    SetWindowTextA(buttons[row][col], text.c_str());

    // Check winner
    if (checkWinner(currentPlayer))
    {
        std::string message;

        if (currentPlayer == 'X')
            message = "Player X Wins!";
        else
            message = "Player O Wins!";

        MessageBoxA(
            NULL,
            message.c_str(),
            "Game Over",
            MB_OK | MB_ICONINFORMATION
        );

        gameOver = true;
        return;
    }

    // Check draw
    if (checkDraw())
    {
        MessageBoxA(
            NULL,
            "Game Draw!",
            "Game Over",
            MB_OK | MB_ICONINFORMATION
        );

        gameOver = true;
        return;
    }

    // Change player
    playerXTurn = !playerXTurn;
}

// Window procedure
LRESULT CALLBACK WindowProc(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam)
{
    switch (msg)
    {
        case WM_COMMAND:
        {
            int id = LOWORD(wParam);

            // Game buttons
            if (id >= 1 && id <= 9)
            {
                int position = id - 1;

                int row = position / 3;
                int col = position % 3;

                playMove(row, col);
            }

            // Reset button
            if (id == BTN_RESET)
            {
                resetGame();
            }

            break;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(
        hwnd,
        msg,
        wParam,
        lParam
    );
}

// Create button
HWND createButton(
    HWND parent,
    const char* text,
    int id,
    int x,
    int y,
    int width,
    int height)
{
    return CreateWindowA(
        "BUTTON",
        text,
        WS_VISIBLE |
        WS_CHILD |
        BS_PUSHBUTTON,
        x,
        y,
        width,
        height,
        parent,
        (HMENU)id,
        GetModuleHandle(NULL),
        NULL
    );
}

// Main function
int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
    // Window class
    WNDCLASSA wc = {};

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "TicTacToeWindow";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClassA(&wc);

    // Main window
    HWND hwnd = CreateWindowA(
        "TicTacToeWindow",
        "Tic-Tac-Toe Game",
        WS_OVERLAPPEDWINDOW &
        ~WS_MAXIMIZEBOX &
        ~WS_THICKFRAME,
        500,
        150,
        430,
        550,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    // Title
    CreateWindowA(
        "STATIC",
        "TIC - TAC - TOE",
        WS_VISIBLE | WS_CHILD | SS_CENTER,
        50,
        20,
        320,
        50,
        hwnd,
        NULL,
        hInstance,
        NULL
    );

    // Create game buttons
    int startX = 50;
    int startY = 90;

    int buttonSize = 90;
    int gap = 10;

    int id = 1;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            buttons[i][j] = createButton(
                hwnd,
                "",
                id,
                startX + j * (buttonSize + gap),
                startY + i * (buttonSize + gap),
                buttonSize,
                buttonSize
            );

            id++;
        }
    }

    // Reset button
    createButton(
        hwnd,
        "RESET GAME",
        BTN_RESET,
        90,
        410,
        220,
        50
    );

    initializeBoard();

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Message loop
    MSG msg = {};

    while (GetMessage(
        &msg,
        NULL,
        0,
        0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}