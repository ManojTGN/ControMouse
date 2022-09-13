#include <windows.h>
#include <xinput.h>
#include <stdio.h>
#include <iostream>
#include <conio.h>
#include <fcntl.h>
#include <future>
#include <math.h>
#include <io.h>

INPUT input;
POINT cursor;
XINPUT_STATE state;

int ControllerID;
int MenuSelected;
int OptionSelected;

int CursorMoveSpeed = 8.0f;
int CursorScrollSpeed = 16.0f;

struct GAMEPAD{

    bool GAMEPAD_DPAD_UP             = false;
    bool GAMEPAD_DPAD_DOWN           = false;
    bool GAMEPAD_DPAD_LEFT           = false;
    bool GAMEPAD_DPAD_RIGHT          = false;

    bool GAMEPAD_START               = false;
    bool GAMEPAD_BACK                = false;

    bool GAMEPAD_A                   = false;
    bool GAMEPAD_B                   = false;
    bool GAMEPAD_X                   = false;
    bool GAMEPAD_Y                   = false;

    bool GAMEPAD_LEFT_THUMB_TRIGGER  = false;
    bool GAMEPAD_RIGHT_THUMB_TRIGGER = false;

    float GAMEPAD_LEFT_THUMB_X       = 0.0f;
    float GAMEPAD_LEFT_THUMB_Y       = 0.0f;
    float GAMEPAD_RIGHT_THUMB_X      = 0.0f;
    float GAMEPAD_RIGHT_THUMB_Y      = 0.0f;

    float GAMEPAD_LEFT_SHOULDER      = 0.0f;
    float GAMEPAD_RIGHT_SHOULDER     = 0.0f;
  
}KEY_PRESSED, ACTION;

int getController(){

    DWORD dwResult;
    for (DWORD dwUserIndex=0; dwUserIndex< XUSER_MAX_COUNT; dwUserIndex++ ){
        
        ZeroMemory( &state, sizeof(XINPUT_STATE) );
        dwResult = XInputGetState( dwUserIndex, &state );
        if( dwResult == ERROR_SUCCESS ) return dwUserIndex;
        
    }

    return -1;

}

void debugLog(){
    system("cls");
    printf(
        "LeftStick: [%f, %f]\nRightStick: [%f, %f]\nShoulders: [%f, %f]\n\n UP: %d\nDown: %d\nLeft: %d\nRight: %d\n\nA: %d\nB: %d\nX: %d\nY: %d\n\nStart: %d\nBack: %d\n\nLeftTrigger: %d\nRightTrigger: %d",
        KEY_PRESSED.GAMEPAD_LEFT_THUMB_X,KEY_PRESSED.GAMEPAD_LEFT_THUMB_Y,
        KEY_PRESSED.GAMEPAD_RIGHT_THUMB_X,KEY_PRESSED.GAMEPAD_RIGHT_THUMB_Y,
        KEY_PRESSED.GAMEPAD_LEFT_SHOULDER,KEY_PRESSED.GAMEPAD_RIGHT_SHOULDER,
        KEY_PRESSED.GAMEPAD_DPAD_UP,KEY_PRESSED.GAMEPAD_DPAD_DOWN,KEY_PRESSED.GAMEPAD_DPAD_LEFT,KEY_PRESSED.GAMEPAD_DPAD_RIGHT,
        KEY_PRESSED.GAMEPAD_A,KEY_PRESSED.GAMEPAD_B,KEY_PRESSED.GAMEPAD_X,KEY_PRESSED.GAMEPAD_Y,
        KEY_PRESSED.GAMEPAD_START,KEY_PRESSED.GAMEPAD_BACK,
        KEY_PRESSED.GAMEPAD_LEFT_THUMB_TRIGGER,KEY_PRESSED.GAMEPAD_RIGHT_THUMB_TRIGGER
    );

}

void handleMovements(int ControllerID = -1){

    XInputGetState( ControllerID, &state );

    KEY_PRESSED.GAMEPAD_LEFT_THUMB_X = fmaxf(-1, (float) state.Gamepad.sThumbLX / 32767);
    KEY_PRESSED.GAMEPAD_LEFT_THUMB_Y = fmaxf(-1, (float) state.Gamepad.sThumbLY / 32767);

    KEY_PRESSED.GAMEPAD_RIGHT_THUMB_X = fmaxf(-1, (float) state.Gamepad.sThumbRX / 32767);
    KEY_PRESSED.GAMEPAD_RIGHT_THUMB_Y = fmaxf(-1, (float) state.Gamepad.sThumbRY / 32767);

    KEY_PRESSED.GAMEPAD_LEFT_SHOULDER  = (float) state.Gamepad.bLeftTrigger / 255;
    KEY_PRESSED.GAMEPAD_RIGHT_SHOULDER = (float) state.Gamepad.bRightTrigger / 255;


    KEY_PRESSED.GAMEPAD_DPAD_UP    = (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) != 0;
    KEY_PRESSED.GAMEPAD_DPAD_DOWN  = (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) != 0;
    KEY_PRESSED.GAMEPAD_DPAD_LEFT  = (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) != 0;
    KEY_PRESSED.GAMEPAD_DPAD_RIGHT = (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0;

    KEY_PRESSED.GAMEPAD_A = (state.Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0;
    KEY_PRESSED.GAMEPAD_B = (state.Gamepad.wButtons & XINPUT_GAMEPAD_B) != 0;
    KEY_PRESSED.GAMEPAD_X = (state.Gamepad.wButtons & XINPUT_GAMEPAD_X) != 0;
    KEY_PRESSED.GAMEPAD_Y = (state.Gamepad.wButtons & XINPUT_GAMEPAD_Y) != 0;

    KEY_PRESSED.GAMEPAD_START = (state.Gamepad.wButtons & XINPUT_GAMEPAD_START) != 0;
    KEY_PRESSED.GAMEPAD_BACK  = (state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) != 0;

    KEY_PRESSED.GAMEPAD_LEFT_THUMB_TRIGGER  = (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) != 0;
    KEY_PRESSED.GAMEPAD_RIGHT_THUMB_TRIGGER = (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) != 0;
    
}

void handleMouseMove(){

    GetCursorPos(&cursor);

    float dx = KEY_PRESSED.GAMEPAD_LEFT_THUMB_X * CursorMoveSpeed;
    float dy = KEY_PRESSED.GAMEPAD_LEFT_THUMB_Y * CursorMoveSpeed;

    float x = cursor.x + dx  ;
    float y = cursor.y - dy  ;

    SetCursorPos((int)x, (int)y);
}

void handleMouseClick(){


    if(KEY_PRESSED.GAMEPAD_A && !ACTION.GAMEPAD_A){
        input.mi.time = 0;
        input.type = INPUT_MOUSE;
        input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
        SendInput(1, &input, sizeof(INPUT));
        ACTION.GAMEPAD_A = true;
    }else if(!KEY_PRESSED.GAMEPAD_A && ACTION.GAMEPAD_A){
        input.mi.time = 0;
        input.type = INPUT_MOUSE;
        input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
        SendInput(1, &input, sizeof(INPUT));
        ACTION.GAMEPAD_A = false;
    }

    if(KEY_PRESSED.GAMEPAD_X && !ACTION.GAMEPAD_X){
        input.mi.time = 0;
        input.type = INPUT_MOUSE;
        input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
        SendInput(1, &input, sizeof(INPUT));
        ACTION.GAMEPAD_X = true;
    }else if(!KEY_PRESSED.GAMEPAD_X && ACTION.GAMEPAD_X){
        input.mi.time = 0;
        input.type = INPUT_MOUSE;
        input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
        SendInput(1, &input, sizeof(INPUT));
        ACTION.GAMEPAD_X = false;
    }

    if(KEY_PRESSED.GAMEPAD_LEFT_THUMB_TRIGGER && !ACTION.GAMEPAD_LEFT_THUMB_TRIGGER){
        input.mi.time = 0;
        input.type = INPUT_MOUSE;
        input.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
        SendInput(1, &input, sizeof(INPUT));
        ACTION.GAMEPAD_LEFT_THUMB_TRIGGER = true;
    }else if(!KEY_PRESSED.GAMEPAD_LEFT_THUMB_TRIGGER && ACTION.GAMEPAD_LEFT_THUMB_TRIGGER){
        input.mi.time = 0;
        input.type = INPUT_MOUSE;
        input.mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
        SendInput(1, &input, sizeof(INPUT));
        ACTION.GAMEPAD_LEFT_THUMB_TRIGGER = false;
    }

}

void handleMouseScroll(){

    float dx = KEY_PRESSED.GAMEPAD_RIGHT_THUMB_X * CursorScrollSpeed;
    float dy = KEY_PRESSED.GAMEPAD_RIGHT_THUMB_Y * CursorScrollSpeed;

    input.mi.time = 0;
    input.type = INPUT_MOUSE;
    input.mi.mouseData = dx;
    input.mi.dwFlags = MOUSEEVENTF_HWHEEL;
    SendInput(1, &input, sizeof(INPUT));

    input.mi.mouseData = dy;
    input.mi.dwFlags = MOUSEEVENTF_WHEEL;
    SendInput(1, &input, sizeof(INPUT));

}

void handleKeyboard(){
    input.type = INPUT_KEYBOARD;
    input.ki.wScan = 0;
    input.ki.time = 0;
    input.ki.dwExtraInfo = 0;
    input.ki.wVk = 0;
    input.ki.dwFlags = 0;
    if(KEY_PRESSED.GAMEPAD_START && !ACTION.GAMEPAD_START){
        input.ki.wVk = VK_LWIN;
        SendInput(1, &input, sizeof(INPUT));
        ACTION.GAMEPAD_START = true;
    }else if(!KEY_PRESSED.GAMEPAD_START && ACTION.GAMEPAD_START){
        input.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1, &input, sizeof(INPUT));
        ACTION.GAMEPAD_START = false;
    }

    if(KEY_PRESSED.GAMEPAD_DPAD_UP ){
        input.ki.wVk = VK_UP;
        SendInput(1, &input, sizeof(INPUT));
        ACTION.GAMEPAD_DPAD_UP = true;
    }else if(!KEY_PRESSED.GAMEPAD_DPAD_UP){
        input.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1, &input, sizeof(INPUT));
        ACTION.GAMEPAD_DPAD_UP = false;
    }

    if(KEY_PRESSED.GAMEPAD_DPAD_DOWN && !ACTION.GAMEPAD_DPAD_DOWN){
        input.ki.wVk = VK_DOWN;
        SendInput(1, &input, sizeof(INPUT));
        ACTION.GAMEPAD_DPAD_DOWN = true;
    }else if(!KEY_PRESSED.GAMEPAD_DPAD_DOWN && ACTION.GAMEPAD_DPAD_DOWN){
        SendInput(1, &input, sizeof(INPUT));
        ACTION.GAMEPAD_DPAD_DOWN = false;
    }

    if(KEY_PRESSED.GAMEPAD_DPAD_LEFT && !ACTION.GAMEPAD_DPAD_LEFT){
        input.ki.wVk = VK_LEFT;
        SendInput(1, &input, sizeof(INPUT));
        ACTION.GAMEPAD_DPAD_LEFT = true;
    }else if(!KEY_PRESSED.GAMEPAD_DPAD_LEFT && ACTION.GAMEPAD_DPAD_LEFT){
        input.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1, &input, sizeof(INPUT));
        ACTION.GAMEPAD_DPAD_LEFT = false;
    }

    if(KEY_PRESSED.GAMEPAD_DPAD_RIGHT && !ACTION.GAMEPAD_DPAD_RIGHT){
        input.ki.wVk = VK_RIGHT;
        SendInput(1, &input, sizeof(INPUT));
        ACTION.GAMEPAD_DPAD_RIGHT = true;
    }else if(!KEY_PRESSED.GAMEPAD_DPAD_RIGHT && ACTION.GAMEPAD_DPAD_RIGHT){
        input.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1, &input, sizeof(INPUT));
        ACTION.GAMEPAD_DPAD_RIGHT = false;
    }

    if(KEY_PRESSED.GAMEPAD_LEFT_SHOULDER == 1.0f && !ACTION.GAMEPAD_LEFT_SHOULDER){
        input.ki.wVk = VK_BACK;
        SendInput(1, &input, sizeof(INPUT));
        ACTION.GAMEPAD_LEFT_SHOULDER = 1.0f;
    }else if(KEY_PRESSED.GAMEPAD_LEFT_SHOULDER != 1.0f && ACTION.GAMEPAD_LEFT_SHOULDER){
        input.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1, &input, sizeof(INPUT));
        ACTION.GAMEPAD_LEFT_SHOULDER = 0.0f;
    }

    if(KEY_PRESSED.GAMEPAD_B && !ACTION.GAMEPAD_B){
        input.ki.wVk = VK_ESCAPE;
        SendInput(1, &input, sizeof(INPUT));
        ACTION.GAMEPAD_B = true;
    }else if(!KEY_PRESSED.GAMEPAD_B && ACTION.GAMEPAD_B){
        input.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1, &input, sizeof(INPUT));
        ACTION.GAMEPAD_B = false;
    }

    if(KEY_PRESSED.GAMEPAD_Y && !ACTION.GAMEPAD_Y){
        input.ki.wVk = VK_RETURN;
        SendInput(1, &input, sizeof(INPUT));
        ACTION.GAMEPAD_Y = true;
    }else if(!KEY_PRESSED.GAMEPAD_Y && ACTION.GAMEPAD_Y){
        input.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1, &input, sizeof(INPUT));
        ACTION.GAMEPAD_Y = false;
    }

}

void showMenu(int menu = 1,int option = 1,int controllerid = -1){
    
    system ("CLS");
    _setmode(_fileno(stdout), _O_U16TEXT);
    //Width: 64  Height: 20
    //SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 55);

    if(menu == 1){
        std::wcout << L"\
┌──────────────────────────────────────────────────────────────┐\n\
│                                                              │\n\
│                         \e[1mControMouse\e[0m                          │\n\
│                                                              │\n\
│──────────────────────────────────────────────────────────────│\n\
│                                                              │\n\
│                                                              │\n\
│                                                              │\n\
│              ";if(option==1){SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);}std::wcout <<"1. Settings";if(option==1){SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);}std::wcout <<L"                                     │\n\
│              ";if(option==2){SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);}std::wcout <<"2. About Us";if(option==2){SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);}std::wcout <<L"                                     │\n\
│                                                              │\n\
│              ";if(option==3){SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);}std::wcout <<"3. Exit  :(";if(option==3){SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);}std::wcout <<L"                                     │\n\
│                                                              │\n\
│                                                              │\n\
│                                                              │\n\
│                                                              │\n\
│                                                  use keyboard│\n\
│──────────────────────────────────────────────────────────────│\n\
│Build: v0.0.1                          ";if(controllerid != -1){std::wcout << L"    ";}if(controllerid==-1){SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);}std::wcout << L"Controller ";if(controllerid == -1){std::wcout<<"Not ";}std::wcout<<"Detected";if(controllerid == -1){SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);}std::wcout<<L"│\n\
└──────────────────────────────────────────────────────────────┘\n";
    }

   MenuSelected = menu;
   OptionSelected = option;
   ControllerID = controllerid;
}


void handleMenu(int *menu, int *option){

    //if(GetConsoleWindow() == GetForegroundWindow()){
       
        if(GetKeyState(VK_RETURN) & 0x8000){
            if(*menu == 1 && *option == 3) exit(0);
            *menu = *option;
            *option = 1;
        }else if(GetKeyState(VK_UP) & 0x8000){
            *option = *option > 1 ? *option-1 : *option;
        }else if(GetKeyState(VK_DOWN) & 0x8000){
            *option = *option < 3 ? *option+1 : *option;
        }

    //}
}

int main(){

    int controllerid =  -1;
    int menuselected =   1;
    int optionselected = 1;

    while(true){

        controllerid = getController();
        
        if(menuselected != MenuSelected || optionselected != OptionSelected || controllerid != ControllerID) 
            showMenu(menuselected,optionselected,controllerid);

        if(controllerid != -1){
            handleMovements(controllerid);
            handleMouseMove();
            handleMouseClick();
            handleMouseScroll();
            handleKeyboard();
            //debugLog();
        }
        
        handleMenu(&menuselected,&optionselected);
        Sleep(10);
    }
        
    return 0;

}
// g++ "src\main.cpp" -lXInput -o main.exe
//cd "c:\_PERSONAL\Coding\GIT\ControMouse\src\" ; if ($?) { g++ main.cpp -lXInput -o "..\build\main" } ; if ($?) { ..\build\main }