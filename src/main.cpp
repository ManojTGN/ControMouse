#include <windows.h>
#include <xinput.h>
#include <stdio.h>
#include <math.h>

#include "TUI.h"

INPUT input;
POINT cursor;
XINPUT_STATE state;

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

    if(KEY_PRESSED.GAMEPAD_START && !ACTION.GAMEPAD_START){
        input.type = INPUT_KEYBOARD;
        input.ki.wScan = 0;
        input.ki.time = 0;
        input.ki.dwExtraInfo = 0;
        input.ki.wVk = VK_LWIN;
        input.ki.dwFlags = 0;

        SendInput(1, &input, sizeof(INPUT));
        ACTION.GAMEPAD_START = true;
    }else if(!KEY_PRESSED.GAMEPAD_START && ACTION.GAMEPAD_START){
        input.type = INPUT_KEYBOARD;
        input.ki.wScan = 0;
        input.ki.time = 0;
        input.ki.dwExtraInfo = 0;
        input.ki.wVk = 0;
        input.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1, &input, sizeof(INPUT));
        ACTION.GAMEPAD_START = false;
    }

    if(KEY_PRESSED.GAMEPAD_DPAD_UP && !ACTION.GAMEPAD_DPAD_UP){
        input.type = INPUT_KEYBOARD;
        input.ki.wScan = 0;
        input.ki.time = 0;
        input.ki.dwExtraInfo = 0;
        input.ki.wVk = VK_UP;
        input.ki.dwFlags = 0;

        SendInput(1, &input, sizeof(INPUT));
        ACTION.GAMEPAD_DPAD_UP = true;
    }else if(!KEY_PRESSED.GAMEPAD_DPAD_UP && ACTION.GAMEPAD_DPAD_UP){
        input.type = INPUT_KEYBOARD;
        input.ki.wScan = 0;
        input.ki.time = 0;
        input.ki.dwExtraInfo = 0;
        input.ki.wVk = 0;
        input.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1, &input, sizeof(INPUT));
        ACTION.GAMEPAD_DPAD_UP = false;
    }

    if(KEY_PRESSED.GAMEPAD_DPAD_DOWN && !ACTION.GAMEPAD_DPAD_DOWN){
        input.type = INPUT_KEYBOARD;
        input.ki.wScan = 0;
        input.ki.time = 0;
        input.ki.dwExtraInfo = 0;
        input.ki.wVk = VK_DOWN;
        input.ki.dwFlags = 0;

        SendInput(1, &input, sizeof(INPUT));
        ACTION.GAMEPAD_DPAD_DOWN = true;
    }else if(!KEY_PRESSED.GAMEPAD_DPAD_DOWN && ACTION.GAMEPAD_DPAD_DOWN){
        input.type = INPUT_KEYBOARD;
        input.ki.wScan = 0;
        input.ki.time = 0;
        input.ki.dwExtraInfo = 0;
        input.ki.wVk = 0;
        input.ki.dwFlags = 0;
        SendInput(1, &input, sizeof(INPUT));
        ACTION.GAMEPAD_DPAD_DOWN = false;
    }

    if(KEY_PRESSED.GAMEPAD_DPAD_LEFT && !ACTION.GAMEPAD_DPAD_LEFT){
        input.type = INPUT_KEYBOARD;
        input.ki.wScan = 0;
        input.ki.time = 0;
        input.ki.dwExtraInfo = 0;
        input.ki.wVk = VK_LEFT;
        input.ki.dwFlags = 0;

        SendInput(1, &input, sizeof(INPUT));
        ACTION.GAMEPAD_DPAD_LEFT = true;
    }else if(!KEY_PRESSED.GAMEPAD_DPAD_LEFT && ACTION.GAMEPAD_DPAD_LEFT){
        input.type = INPUT_KEYBOARD;
        input.ki.wScan = 0;
        input.ki.time = 0;
        input.ki.dwExtraInfo = 0;
        input.ki.wVk = 0;
        input.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1, &input, sizeof(INPUT));
        ACTION.GAMEPAD_DPAD_LEFT = false;
    }

    if(KEY_PRESSED.GAMEPAD_DPAD_RIGHT && !ACTION.GAMEPAD_DPAD_RIGHT){
        input.type = INPUT_KEYBOARD;
        input.ki.wScan = 0;
        input.ki.time = 0;
        input.ki.dwExtraInfo = 0;
        input.ki.wVk = VK_RIGHT;
        input.ki.dwFlags = 0;

        SendInput(1, &input, sizeof(INPUT));
        ACTION.GAMEPAD_DPAD_RIGHT = true;
    }else if(!KEY_PRESSED.GAMEPAD_DPAD_RIGHT && ACTION.GAMEPAD_DPAD_RIGHT){
        input.type = INPUT_KEYBOARD;
        input.ki.wScan = 0;
        input.ki.time = 0;
        input.ki.dwExtraInfo = 0;
        input.ki.wVk = 0;
        input.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1, &input, sizeof(INPUT));
        ACTION.GAMEPAD_DPAD_RIGHT = false;
    }

    if(KEY_PRESSED.GAMEPAD_LEFT_SHOULDER == 1.0f && !ACTION.GAMEPAD_LEFT_SHOULDER){
        input.type = INPUT_KEYBOARD;
        input.ki.wScan = 0;
        input.ki.time = 0;
        input.ki.dwExtraInfo = 0;
        input.ki.wVk = VK_BACK;
        input.ki.dwFlags = 0;

        SendInput(1, &input, sizeof(INPUT));
        ACTION.GAMEPAD_LEFT_SHOULDER = 1.0f;
    }else if(KEY_PRESSED.GAMEPAD_LEFT_SHOULDER != 1.0f && ACTION.GAMEPAD_LEFT_SHOULDER){
        input.type = INPUT_KEYBOARD;
        input.ki.wScan = 0;
        input.ki.time = 0;
        input.ki.dwExtraInfo = 0;
        input.ki.wVk = 0;
        input.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1, &input, sizeof(INPUT));
        ACTION.GAMEPAD_LEFT_SHOULDER = 0.0f;
    }

    if(KEY_PRESSED.GAMEPAD_B && !ACTION.GAMEPAD_B){
        input.type = INPUT_KEYBOARD;
        input.ki.wScan = 0;
        input.ki.time = 0;
        input.ki.dwExtraInfo = 0;
        input.ki.wVk = VK_ESCAPE;
        input.ki.dwFlags = 0;

        SendInput(1, &input, sizeof(INPUT));
        ACTION.GAMEPAD_B = true;
    }else if(!KEY_PRESSED.GAMEPAD_B && ACTION.GAMEPAD_B){
        input.type = INPUT_KEYBOARD;
        input.ki.wScan = 0;
        input.ki.time = 0;
        input.ki.dwExtraInfo = 0;
        input.ki.wVk = 0;
        input.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1, &input, sizeof(INPUT));
        ACTION.GAMEPAD_B = false;
    }

    if(KEY_PRESSED.GAMEPAD_Y && !ACTION.GAMEPAD_Y){
        input.type = INPUT_KEYBOARD;
        input.ki.wScan = 0;
        input.ki.time = 0;
        input.ki.dwExtraInfo = 0;
        input.ki.wVk = VK_RETURN;
        input.ki.dwFlags = 0;

        SendInput(1, &input, sizeof(INPUT));
        ACTION.GAMEPAD_Y = true;
    }else if(!KEY_PRESSED.GAMEPAD_Y && ACTION.GAMEPAD_Y){
        input.type = INPUT_KEYBOARD;
        input.ki.wScan = 0;
        input.ki.time = 0;
        input.ki.dwExtraInfo = 0;
        input.ki.wVk = 0;
        input.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1, &input, sizeof(INPUT));
        ACTION.GAMEPAD_Y = false;
    }

}

int main(){
    
    int ControllerID = getController();
    if(ControllerID == -1){
        printf("No Controller Dectected!");
        return 1;
    }
    
    printf("Controller Dectected [dwUserIndex: %d], (ctrl + c) to stop.\n",ControllerID);

    print("Hello","hi");

    while(true){
        handleMovements(ControllerID);
        handleMouseMove();
        handleMouseClick();
        handleMouseScroll();
        handleKeyboard();

        //debugLog();
        Sleep(10);
    }
        
    return 0;

}
// g++ "src\main.cpp" -lXInput -o main.exe
//cd "c:\_PERSONAL\Coding\GIT\ControMouse\src\" ; if ($?) { g++ main.cpp -lXInput -o main } ; if ($?) { .\main }