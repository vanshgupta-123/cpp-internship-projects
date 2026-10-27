#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <string>
#include <cstring>

#define INPUT_BOX 101
#define GUESS_BUTTON 102
#define NEW_GAME_BUTTON 103
#define RESULT_TEXT 104
#define ATTEMPTS_TEXT 105

HWND hInput;
HWND hResult;
HWND hAttempts;

int secretNumber;
int attempts;

// Start a new game
void newGame()
{
    secretNumber = rand() % 100 + 1;
    attempts = 0;

    SetWindowTextA(hInput, "");

    SetWindowTextA(
        hResult,
        "Guess a number between 1 and 100"
    );

    SetWindowTextA(
        hAttempts,
        "Attempts: 0"
    );

    SetFocus(hInput);
}

// Check the guessed number
void checkGuess()
{
    char buffer[100];

    GetWindowTextA(
        hInput,
        buffer,
        100
    );

    if (strlen(buffer) == 0)
    {
        MessageBoxA(
            NULL,
            "Please enter a number!",
            "Warning",
            MB_OK | MB_ICONWARNING
        );

        return;
    }

    int guess = atoi(buffer);

    if (guess < 1 || guess > 100)
    {
        MessageBoxA(
            NULL,
            "Please enter a number between 1 and 100.",
            "Invalid Number",
            MB_OK | MB_ICONWARNING
        );

        return;
    }

    attempts++;

    std::string message;

    if (guess < secretNumber)
    {
        message = "Too Low! Try a higher number.";
    }
    else if (guess > secretNumber)
    {
        message = "Too High! Try a lower number.";
    }
    else
    {
        message = "Congratulations! You guessed the number!";

        MessageBoxA(
            NULL,
            "You Won!",
            "Congratulations",
            MB_OK | MB_ICONINFORMATION
        );
    }

    SetWindowTextA(
        hResult,
        message.c_str()
    );

    std::string attemptText =
        "Attempts: " + std::to_string(attempts);

    SetWindowTextA(
        hAttempts,
        attemptText.c_str()
    );

    if (guess == secretNumber)
    {
        EnableWindow(hInput, FALSE);

        EnableWindow(
            GetDlgItem(
                GetParent(hInput),
                GUESS_BUTTON
            ),
            FALSE
        );
    }

    SetWindowTextA(hInput, "");
    SetFocus(hInput);
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

            if (id == GUESS_BUTTON)
            {
                checkGuess();
            }
            else if (id == NEW_GAME_BUTTON)
            {
                EnableWindow(hInput, TRUE);

                EnableWindow(
                    GetDlgItem(
                        hwnd,
                        GUESS_BUTTON
                    ),
                    TRUE
                );

                newGame();
            }

            break;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProcA(
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
    srand((unsigned)time(NULL));

    WNDCLASSA wc = {};

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "NumberGuessingGame";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClassA(&wc);

    HWND hwnd = CreateWindowA(
        "NumberGuessingGame",
        "Number Guessing Game",
        WS_OVERLAPPEDWINDOW &
        ~WS_MAXIMIZEBOX &
        ~WS_THICKFRAME,
        500,
        150,
        500,
        500,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    CreateWindowA(
        "STATIC",
        "NUMBER GUESSING GAME",
        WS_VISIBLE |
        WS_CHILD |
        SS_CENTER,
        50,
        30,
        380,
        50,
        hwnd,
        NULL,
        hInstance,
        NULL
    );

    CreateWindowA(
        "STATIC",
        "Guess a number between 1 and 100",
        WS_VISIBLE |
        WS_CHILD |
        SS_CENTER,
        70,
        90,
        340,
        30,
        hwnd,
        NULL,
        hInstance,
        NULL
    );

    hInput = CreateWindowA(
        "EDIT",
        "",
        WS_VISIBLE |
        WS_CHILD |
        WS_BORDER |
        ES_CENTER |
        ES_NUMBER,
        120,
        140,
        240,
        45,
        hwnd,
        (HMENU)INPUT_BOX,
        hInstance,
        NULL
    );

    createButton(
        hwnd,
        "GUESS",
        GUESS_BUTTON,
        120,
        205,
        110,
        45
    );

    createButton(
        hwnd,
        "NEW GAME",
        NEW_GAME_BUTTON,
        250,
        205,
        110,
        45
    );

    hResult = CreateWindowA(
        "STATIC",
        "Guess a number between 1 and 100",
        WS_VISIBLE |
        WS_CHILD |
        SS_CENTER,
        50,
        280,
        380,
        50,
        hwnd,
        (HMENU)RESULT_TEXT,
        hInstance,
        NULL
    );

    hAttempts = CreateWindowA(
        "STATIC",
        "Attempts: 0",
        WS_VISIBLE |
        WS_CHILD |
        SS_CENTER,
        50,
        340,
        380,
        40,
        hwnd,
        (HMENU)ATTEMPTS_TEXT,
        hInstance,
        NULL
    );

    newGame();

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg = {};

    while (GetMessageA(
        &msg,
        NULL,
        0,
        0))
    {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    return 0;
}