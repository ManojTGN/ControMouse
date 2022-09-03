#include <windows.h>
#include <xinput.h>
#include <stdio.h>
#include <math.h>

int main(){
    
    DWORD dwResult;
    XINPUT_STATE state;
    for (DWORD dwUserIndex=0; dwUserIndex< XUSER_MAX_COUNT; dwUserIndex++ ){
        
        ZeroMemory( &state, sizeof(XINPUT_STATE) );
        dwResult = XInputGetState( dwUserIndex, &state );

        if( dwResult == ERROR_SUCCESS ){
            printf("Controller Detected [%d]\n",dwUserIndex);

            float LX, LY;
            while(true){
                XInputGetState( dwUserIndex, &state );
                LX = fmaxf(-1, (float) state.Gamepad.sThumbLX / 32767);
                LY = fmaxf(-1, (float) state.Gamepad.sThumbLY / 32767);
                printf("[%f,%f]          \r",LX,LY);
            }
        }
    }

    return 0;

}
// g++ "src\main.cpp" -lXInput -o main.exe