#include <windows.h>
#include <string>
#include <sstream>
#include <iomanip>

#define DISPLAY 100
#define NUM_0 200
#define NUM_1 201
#define NUM_2 202
#define NUM_3 203
#define NUM_4 204
#define NUM_5 205
#define NUM_6 206
#define NUM_7 207
#define NUM_8 208
#define NUM_9 209

#define BTN_ADD 300
#define BTN_SUB 301
#define BTN_MUL 302
#define BTN_DIV 303
#define BTN_EQUAL 304
#define BTN_CLEAR 305
#define BTN_DOT 306

HWND hDisplay;

double firstNumber = 0;
double secondNumber = 0;
char operation = '\0';
bool newNumber = true;

// Get text from display
std::string getDisplay()
{
    char buffer[256];
    GetWindowTextA(hDisplay, buffer, 256);
    return std::string(buffer);
}

// Set display text
void setDisplay(const std::string& text)
{
    SetWindowTextA(hDisplay, text.c_str());
}

// Calculate result
void calculate()
{
    secondNumber = std::stod(getDisplay());

    double result = 0;

    switch (operation)
    {
        case '+':
            result = firstNumber + secondNumber;
            break;

        case '-':
            result = firstNumber - secondNumber;
            break;

        case '*':
            result = firstNumber * secondNumber;
            break;

        case '/':
            if (secondNumber == 0)
            {
                setDisplay("Error");
                newNumber = true;
                return;
            }
            result = firstNumber / secondNumber;
            break;
    }

    std::ostringstream output;
    output << std::setprecision(12) << result;

    setDisplay(output.str());

    firstNumber = result;
    newNumber = true;
}

// Window procedure
LRESULT CALLBACK WindowProc(
    HWND hwnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_COMMAND:
        {
            int id = LOWORD(wParam);

            // Number buttons
            if (id >= NUM_0 && id <= NUM_9)
            {
                int number = id - NUM_0;

                std::string current = getDisplay();

                if (newNumber || current == "0" || current == "Error")
                {
                    current = std::to_string(number);
                    newNumber = false;
                }
                else
                {
                    current += std::to_string(number);
                }

                setDisplay(current);
            }

            // Decimal point
            else if (id == BTN_DOT)
            {
                std::string current = getDisplay();

                if (newNumber)
                {
                    setDisplay("0.");
                    newNumber = false;
                }
                else if (current.find('.') == std::string::npos)
                {
                    current += ".";
                    setDisplay(current);
                }
            }

            // Clear
            else if (id == BTN_CLEAR)
            {
                setDisplay("0");
                firstNumber = 0;
                secondNumber = 0;
                operation = '\0';
                newNumber = true;
            }

            // Operators
            else if (
                id == BTN_ADD ||
                id == BTN_SUB ||
                id == BTN_MUL ||
                id == BTN_DIV)
            {
                firstNumber = std::stod(getDisplay());

                if (id == BTN_ADD)
                    operation = '+';

                else if (id == BTN_SUB)
                    operation = '-';

                else if (id == BTN_MUL)
                    operation = '*';

                else if (id == BTN_DIV)
                    operation = '/';

                newNumber = true;
            }

            // Equal
            else if (id == BTN_EQUAL)
            {
                if (operation != '\0')
                {
                    calculate();
                    operation = '\0';
                }
            }

            break;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


// Create button helper
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
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
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
    wc.lpszClassName = "CalculatorWindow";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClassA(&wc);

    // Main window
    HWND hwnd = CreateWindowA(
        "CalculatorWindow",
        "Simple C++ Calculator",
        WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX,
        500,
        150,
        380,
        520,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    // Display
    hDisplay = CreateWindowA(
        "EDIT",
        "0",
        WS_VISIBLE |
        WS_CHILD |
        WS_BORDER |
        ES_RIGHT |
        ES_AUTOHSCROLL,
        30,
        30,
        300,
        60,
        hwnd,
        (HMENU)DISPLAY,
        hInstance,
        NULL
    );

    // Make display font bigger
    HFONT hFont = CreateFontA(
        28,
        0,
        0,
        0,
        FW_NORMAL,
        FALSE,
        FALSE,
        FALSE,
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,
        DEFAULT_PITCH,
        "Arial"
    );

    SendMessage(
        hDisplay,
        WM_SETFONT,
        (WPARAM)hFont,
        TRUE
    );

    // Buttons
    int x = 30;
    int y = 110;
    int w = 70;
    int h = 55;
    int gap = 8;

    // Row 1
    createButton(hwnd, "7", NUM_7, x, y, w, h);
    createButton(hwnd, "8", NUM_8, x + 78, y, w, h);
    createButton(hwnd, "9", NUM_9, x + 156, y, w, h);
    createButton(hwnd, "/", BTN_DIV, x + 234, y, w, h);

    // Row 2
    y += h + gap;

    createButton(hwnd, "4", NUM_4, x, y, w, h);
    createButton(hwnd, "5", NUM_5, x + 78, y, w, h);
    createButton(hwnd, "6", NUM_6, x + 156, y, w, h);
    createButton(hwnd, "*", BTN_MUL, x + 234, y, w, h);

    // Row 3
    y += h + gap;

    createButton(hwnd, "1", NUM_1, x, y, w, h);
    createButton(hwnd, "2", NUM_2, x + 78, y, w, h);
    createButton(hwnd, "3", NUM_3, x + 156, y, w, h);
    createButton(hwnd, "-", BTN_SUB, x + 234, y, w, h);

    // Row 4
    y += h + gap;

    createButton(hwnd, "0", NUM_0, x, y, w, h);
    createButton(hwnd, ".", BTN_DOT, x + 78, y, w, h);
    createButton(hwnd, "=", BTN_EQUAL, x + 156, y, w, h);
    createButton(hwnd, "+", BTN_ADD, x + 234, y, w, h);

    // Clear button
    y += h + gap;

    createButton(
        hwnd,
        "CLEAR",
        BTN_CLEAR,
        x,
        y,
        312,
        h
    );

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Message loop
    MSG msg = {};

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}