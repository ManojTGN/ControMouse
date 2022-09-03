#include <windows.h>
#include <xinput.h>
#include <stdio.h>
#include <math.h>


XINPUT_STATE state;
struct GAMEPAD{

    bool GAMEPAD_DPAD_UP          = false;
    bool GAMEPAD_DPAD_DOWN        = false;
    bool GAMEPAD_DPAD_LEFT        = false;
    bool GAMEPAD_DPAD_RIGHT       = false;

    bool GAMEPAD_START            = false;
    bool GAMEPAD_BACK             = false;

    bool GAMEPAD_A                = false;
    bool GAMEPAD_B                = false;
    bool GAMEPAD_X                = false;
    bool GAMEPAD_Y                = false;

    float GAMEPAD_LEFT_THUMB_X    = 0.0f;
    float GAMEPAD_LEFT_THUMB_Y    = 0.0f;
    float GAMEPAD_RIGHT_THUMB_X   = 0.0f;
    float GAMEPAD_RIGHT_THUMB_Y   = 0.0f;

    float GAMEPAD_LEFT_SHOULDER   = 0.0f;
    float GAMEPAD_RIGHT_SHOULDER  = 0.0f;
  
}XINPUT_BUTTONS;

int getController(){

    DWORD dwResult;
    for (DWORD dwUserIndex=0; dwUserIndex< XUSER_MAX_COUNT; dwUserIndex++ ){
        
        ZeroMemory( &state, sizeof(XINPUT_STATE) );
        dwResult = XInputGetState( dwUserIndex, &state );
        
        if( dwResult == ERROR_SUCCESS ){
            return dwUserIndex;
        }

    }

    return -1;

}

void handleMovements(int ControllerID = -1){

    XInputGetState( ControllerID, &state );

    XINPUT_BUTTONS.GAMEPAD_LEFT_THUMB_X = fmaxf(-1, (float) state.Gamepad.sThumbLX / 32767);
    XINPUT_BUTTONS.GAMEPAD_LEFT_THUMB_Y = fmaxf(-1, (float) state.Gamepad.sThumbLY / 32767);

    XINPUT_BUTTONS.GAMEPAD_RIGHT_THUMB_X = fmaxf(-1, (float) state.Gamepad.sThumbRX / 32767);
    XINPUT_BUTTONS.GAMEPAD_RIGHT_THUMB_Y = fmaxf(-1, (float) state.Gamepad.sThumbRY / 32767);

    XINPUT_BUTTONS.GAMEPAD_LEFT_SHOULDER = (float) state.Gamepad.bLeftTrigger / 255;
    XINPUT_BUTTONS.GAMEPAD_RIGHT_SHOULDER  = (float) state.Gamepad.bRightTrigger / 255;

    XINPUT_BUTTONS.GAMEPAD_DPAD_UP = (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) != 0;
    XINPUT_BUTTONS.GAMEPAD_DPAD_DOWN = (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) != 0;
    XINPUT_BUTTONS.GAMEPAD_DPAD_LEFT = (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) != 0;
    XINPUT_BUTTONS.GAMEPAD_DPAD_RIGHT = (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0;

    XINPUT_BUTTONS.GAMEPAD_A = (state.Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0;
    XINPUT_BUTTONS.GAMEPAD_B = (state.Gamepad.wButtons & XINPUT_GAMEPAD_B) != 0;
    XINPUT_BUTTONS.GAMEPAD_X = (state.Gamepad.wButtons & XINPUT_GAMEPAD_X) != 0;
    XINPUT_BUTTONS.GAMEPAD_Y = (state.Gamepad.wButtons & XINPUT_GAMEPAD_Y) != 0;

    XINPUT_BUTTONS.GAMEPAD_START= (state.Gamepad.wButtons & XINPUT_GAMEPAD_START) != 0;
    XINPUT_BUTTONS.GAMEPAD_BACK = (state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) != 0;
}

void debugLog(){

    printf(
        "LeftStick: [%f, %f]\nRightStick: [%f, %f]\nShoulders: [%f, %f]\n\n UP: %d\nDown: %d\nLeft: %d\nRight: %d\n\nA: %d\nB: %d\nX: %d\nY: %d\n\nStart: %d\nBack: %d",
        XINPUT_BUTTONS.GAMEPAD_LEFT_THUMB_X,XINPUT_BUTTONS.GAMEPAD_LEFT_THUMB_Y,
        XINPUT_BUTTONS.GAMEPAD_RIGHT_THUMB_X,XINPUT_BUTTONS.GAMEPAD_RIGHT_THUMB_Y,
        XINPUT_BUTTONS.GAMEPAD_LEFT_SHOULDER,XINPUT_BUTTONS.GAMEPAD_RIGHT_SHOULDER,
        XINPUT_BUTTONS.GAMEPAD_DPAD_UP,XINPUT_BUTTONS.GAMEPAD_DPAD_DOWN,XINPUT_BUTTONS.GAMEPAD_DPAD_LEFT,XINPUT_BUTTONS.GAMEPAD_DPAD_RIGHT,
        XINPUT_BUTTONS.GAMEPAD_A,XINPUT_BUTTONS.GAMEPAD_B,XINPUT_BUTTONS.GAMEPAD_X,XINPUT_BUTTONS.GAMEPAD_Y,
        XINPUT_BUTTONS.GAMEPAD_START,XINPUT_BUTTONS.GAMEPAD_BACK
    );

}

int main(){
    
    int ControllerID = getController();

    while(true){
        handleMovements(ControllerID);
        debugLog();

        Sleep(10);
        system("cls"); 
    }
        
    return 0;

}
// g++ "src\main.cpp" -lXInput -o main.exe