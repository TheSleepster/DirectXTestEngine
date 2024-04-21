#include <windows.h>
#include <d3d11.h>
#include <cstdlib>
#include <cstdint>

#include "HandmadeMath.h"

#define global_variable static
#define local_persist static
#define internal static

typedef uint8_t  uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uin64;

typedef int8_t  int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef float real32;
typedef double real64;

LRESULT WINAPI 
MainWindowCallback(HWND Window,
               UINT Message,
               WPARAM wParam,
               LPARAM lParam) 
{
    LRESULT Result = 0;

    switch(Message) 
    {
        case WM_SIZE: 
        {
        } break;
        
        case WM_DESTROY:
        {
            PostQuitMessage(0);
        } break;

        case WM_CLOSE:
        {
            DestroyWindow(Window);
        } break;

        case WM_ACTIVATEAPP:
        {
        } break;

        case WM_PAINT:
        {
            PAINTSTRUCT Paint;
            HDC DeviceContext = BeginPaint(Window, &Paint);

            int X = Paint.rcPaint.left;
            int Y = Paint.rcPaint.top;
            LONG Width = Paint.rcPaint.right - Paint.rcPaint.left;
            LONG Height = Paint.rcPaint.bottom - Paint.rcPaint.top;
            PatBlt(DeviceContext, X, Y, Width, Height, BLACKNESS);

            EndPaint(Window, &Paint);
        }break;

        default: 
        {
            Result = DefWindowProc(Window, Message, wParam, lParam);
        } break;
    }
    return(Result);
}

int WINAPI 
wWinMain(HINSTANCE hInstance, 
         HINSTANCE hPrevInstance, 
         PWSTR pCmdLine, 
         int nCmdShow) 
{
    WNDCLASS WindowClass = {};
    WindowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    WindowClass.lpfnWndProc = MainWindowCallback;
    WindowClass.hInstance = hInstance;
    WindowClass.lpszClassName = "HandmadeWindow";

    if(RegisterClass(&WindowClass))
    {
        HWND WindowHandle = 
            CreateWindowEx(
                0,
                WindowClass.lpszClassName,
                "Handmade DX11",
                WS_OVERLAPPEDWINDOW |WS_VISIBLE,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                0,
                0,
                hInstance,
                0);  
        if(WindowHandle) 
        {
            MSG Message;
            for(;;) 
            {
                BOOL MessageResult = GetMessage(&Message, 0, 0, 0); 
                if(MessageResult > 0) 
                {
                    TranslateMessage(&Message);
                    DispatchMessage(&Message);
                }
                else 
                {
                    break;
                }
            }
        }
        else 
        {
            // TODO : Create a logging function
        }
    }
    else 
    {
    // TODO : Logging
    }
}
