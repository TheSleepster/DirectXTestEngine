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

global_variable bool Running;

global_variable BITMAPINFO BitmapInfo;
global_variable void *BitmapMemory;
global_variable int BitmapWidth;
global_variable int BitmapHeight;
global_variable int BytesPerPixel = 4;

internal void
RenderWeirdGradient(int XOffset, int YOffset) 
{
    int Width = BitmapWidth;
    int Height = BitmapHeight;

    int Pitch = Width*BytesPerPixel;
    uint8 *Row = (uint8*)BitmapMemory;

    for(int Y = 0; Y < BitmapHeight; Y++)
    {
        uint32 *Pixel = (uint32*)Row;
        for(int X = 0; X < BitmapWidth; X++)
        {
            uint8 Blue = (uint8)(X + XOffset);
            uint8 Green = (uint8)(Y + YOffset);

            *Pixel++ = (Blue << 8 | Green);
        }
        Row += Pitch;
    }
}

internal void 
Win32ResizeDIBSection(int Width, int Height) 
{
    if(BitmapMemory) 
    {
     VirtualFree(BitmapMemory, 0, MEM_RELEASE);   
    }
    BitmapWidth = Width;
    BitmapHeight = Height;

    int BitmapMemorySize =(BitmapWidth*BitmapHeight)*BytesPerPixel;
    BitmapMemory = VirtualAlloc(0, BitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);

    BitmapInfo.bmiHeader.biSize = sizeof(BitmapInfo.bmiHeader);
    BitmapInfo.bmiHeader.biWidth = Width;
    BitmapInfo.bmiHeader.biHeight = Height;
    BitmapInfo.bmiHeader.biPlanes = 1;
    BitmapInfo.bmiHeader.biBitCount = 32;
    BitmapInfo.bmiHeader.biCompression = BI_RGB;
}

internal void 
Win32UpdateWindow(HDC DeviceContext, RECT *WindowRect, int X, int Y, int Width, int Height) 
{
    int WindowWidth = WindowRect->right - WindowRect->left;
    int WindowHeight = WindowRect->bottom - WindowRect->top;
    StretchDIBits(DeviceContext, 
                  0, 0, BitmapWidth, BitmapHeight,
                  0, 0, WindowWidth, WindowHeight,
                  BitmapMemory,
                  &BitmapInfo,
                  DIB_RGB_COLORS, SRCCOPY);
}

LRESULT WINAPI 
Win32MainWindowCallback(HWND Window,
                        UINT Message,
                        WPARAM wParam,
                        LPARAM lParam) 
{
    LRESULT Result = 0;

    switch(Message) 
    {
        case WM_SIZE: 
        {
            RECT ClientRect;
            GetClientRect(Window, &ClientRect); 
            LONG Width = ClientRect.right - ClientRect.left;
            LONG Height = ClientRect.bottom - ClientRect.top;
            Win32ResizeDIBSection(Width, Height);
        } break;
        
        case WM_DESTROY:
        {
            // TODO : Error handling?
            Running = false;
        } break;

        case WM_CLOSE:
        {
            // TODO : message?
            Running = false;
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

            RECT ClientRect;
            GetClientRect(Window, &ClientRect); 

            Win32UpdateWindow(DeviceContext, &ClientRect, X, Y, Width, Height);
            EndPaint(Window, &Paint);
        }break;

        case WM_KEYDOWN: 
        {   
            switch(wParam) 
            {
                case VK_ESCAPE:
                {
                    Running = false;
                }break;
            }
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
    WindowClass.lpfnWndProc = Win32MainWindowCallback;
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
            int XOffset = 0;
            int YOffset = 0;
            Running = true;
            while(Running) 
            {
                MSG Message;
                while(PeekMessageA(&Message, 0, 0, 0, PM_REMOVE)) 
                {
                    if(Message.message == WM_QUIT) 
                    {
                        Running = false;
                    }
                    TranslateMessage(&Message);
                    DispatchMessage(&Message);
                }
                RenderWeirdGradient(XOffset, YOffset);

                HDC DeviceContext = GetDC(WindowHandle);
                RECT ClientRect;
                GetClientRect(WindowHandle, &ClientRect); 
                LONG WindowWidth = ClientRect.right - ClientRect.left;
                LONG WindowHeight = ClientRect.bottom - ClientRect.top;
                Win32UpdateWindow(DeviceContext, &ClientRect, 0, 0, WindowWidth, WindowHeight);
                ReleaseDC(WindowHandle, DeviceContext);

                ++XOffset;
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
