#include <windows.h>
#include <windowsx.h>
#include <profileapi.h>

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "gdi32.lib")

#include <gl/GL.h>

#include "../headers/opengl/wglext.h"
#include "../headers/mindless_entities.h"
#include "../headers/mindless_renderer.h"
#include "../headers/mindless_matrix.h"
#include "../headers/mindless_memory.h"

#define internal static
#define int64 __int64

struct metaData {
    BITMAPINFO BitmapInfo;
    void * BitmapMemory;
    int WindowWidth;
    int WindowHeight;
    bool Running;

    float MousePositionX;
    float MousePositionY;

} MetaData;

HWND
Win32InitOpenGL(HINSTANCE hInstance) {
    HWND FakeWND = CreateWindowExA(
        0,
        "MindlessWindowClass",
        "Mindless",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        0,
        0,
        hInstance,
        0
    );

    HDC FakeDC = GetDC(FakeWND);

    PIXELFORMATDESCRIPTOR DesiredPixelFormat = {};
    DesiredPixelFormat.nSize = sizeof(DesiredPixelFormat);
    DesiredPixelFormat.nVersion = 1;
    DesiredPixelFormat.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
    DesiredPixelFormat.cColorBits = 24;
    DesiredPixelFormat.cAlphaBits = 8;
    DesiredPixelFormat.iLayerType = PFD_MAIN_PLANE;

    int SuggestedPixelFormatIndex = ChoosePixelFormat(FakeDC, &DesiredPixelFormat);
    PIXELFORMATDESCRIPTOR SuggestedPixelFormat;
    DescribePixelFormat(FakeDC, SuggestedPixelFormatIndex, sizeof(SuggestedPixelFormat), &SuggestedPixelFormat);
    SetPixelFormat(FakeDC, SuggestedPixelFormatIndex, &SuggestedPixelFormat);

    HGLRC FakeRC = wglCreateContext(FakeDC);
    wglMakeCurrent(FakeDC, FakeRC);

    PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
    wglChoosePixelFormatARB = reinterpret_cast < PFNWGLCHOOSEPIXELFORMATARBPROC > (wglGetProcAddress("wglChoosePixelFormatARB"));

    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
    wglCreateContextAttribsARB = reinterpret_cast < PFNWGLCREATECONTEXTATTRIBSARBPROC > (wglGetProcAddress("wglCreateContextAttribsARB"));

    HWND Window = CreateWindowExA(
        0,
        "MindlessWindowClass",
        "Mindless",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        0,
        0,
        hInstance,
        0
    );

    HDC DC = GetDC(Window);

    const int pixelAttribs[] = {
        WGL_DRAW_TO_WINDOW_ARB,
        GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB,
        GL_TRUE,
        WGL_DOUBLE_BUFFER_ARB,
        GL_TRUE,
        WGL_PIXEL_TYPE_ARB,
        WGL_TYPE_RGBA_ARB,
        WGL_ACCELERATION_ARB,
        WGL_FULL_ACCELERATION_ARB,
        WGL_COLOR_BITS_ARB,
        32,
        WGL_ALPHA_BITS_ARB,
        8,
        WGL_DEPTH_BITS_ARB,
        24,
        WGL_STENCIL_BITS_ARB,
        8,
        WGL_SAMPLE_BUFFERS_ARB,
        GL_TRUE,
        WGL_SAMPLES_ARB,
        4,
        0
    };

    int pixelFormatID;
    UINT numFormats;
    bool status = wglChoosePixelFormatARB(DC, pixelAttribs, 0, 1, &pixelFormatID, &numFormats);

    PIXELFORMATDESCRIPTOR PFD;
    DescribePixelFormat(DC, pixelFormatID, sizeof(PFD), &PFD);
    SetPixelFormat(DC, pixelFormatID, &PFD);

    const int major_min = 4,
        minor_min = 5;
    int contextAttribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB,
        major_min,
        WGL_CONTEXT_MINOR_VERSION_ARB,
        minor_min,
        WGL_CONTEXT_PROFILE_MASK_ARB,
        WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    };

    HGLRC RC = wglCreateContextAttribsARB(DC, 0, contextAttribs);

    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(FakeRC);
    ReleaseDC(FakeWND, FakeDC);
    DestroyWindow(FakeWND);
    wglMakeCurrent(DC, RC);
    return Window;
}

internal void
Win32ResizeDIBSection(int Width, int Height) {

    MetaData.BitmapInfo.bmiHeader.biSize = sizeof(MetaData.BitmapInfo.bmiHeader);
    MetaData.BitmapInfo.bmiHeader.biWidth = Width;
    MetaData.BitmapInfo.bmiHeader.biHeight = Height;
    MetaData.BitmapInfo.bmiHeader.biPlanes = 1;
    MetaData.BitmapInfo.bmiHeader.biBitCount = 32;
    MetaData.BitmapInfo.bmiHeader.biCompression = BI_RGB;
}

internal void
Win32UpdateWindow(HDC DeviceContext, int X, int Y, int Width, int Height) {
    StretchDIBits(
        DeviceContext,
        X,
        Y,
        Width,
        Height,
        X,
        Y,
        Width,
        Height,
        MetaData.BitmapMemory, 
        &MetaData.BitmapInfo,
        DIB_RGB_COLORS,
        SRCCOPY
    );
}

LRESULT CALLBACK MainWindowCallback(
    HWND Window,
    UINT Message,
    WPARAM WParam,
    LPARAM LParam) {
    LRESULT Result = 0;

    switch (Message) {
        case WM_SIZE: {
            RECT ClientRect;
            GetClientRect(Window,&ClientRect);
            int Width = ClientRect.right - ClientRect.left;
            MetaData.WindowWidth = Width;
            int Height = ClientRect.bottom - ClientRect.top;
            MetaData.WindowHeight = Height;
            Win32ResizeDIBSection(Width, Height);
            glResizeWindow(Width, Height);
            HDC WindowDC = GetDC(Window);
        }
        break;

        //WM_MOUSEMOVE Handles player's blackhole position
        case WM_MOUSEMOVE: {
            //Normalizes mouse position(screen px) to OpenGl Coordinates
            MetaData.MousePositionX = (float) GET_X_LPARAM(LParam);
            MetaData.MousePositionY  = (float) GET_Y_LPARAM(LParam);
        }
        break;

        case WM_DESTROY: {
            //TODO Handle as error
            MetaData.Running = false;
        }
        break;

        case WM_CLOSE: {
            //TODO Handle as message
            MetaData.Running = false;
        }
        break;

        case WM_ACTIVATEAPP: {
            OutputDebugStringA("WM_ACTIVATEAPP\n");
        }
        break;

        case WM_PAINT: {
            PAINTSTRUCT Paint;
            HDC DeviceContext = BeginPaint(Window,&Paint);
            int X = Paint.rcPaint.left;
            int Y = Paint.rcPaint.top;
            int Width = Paint.rcPaint.right - Paint.rcPaint.left;
            int Height = Paint.rcPaint.bottom - Paint.rcPaint.top;
            Win32UpdateWindow(DeviceContext, X, Y, Width, Height);
            EndPaint(Window,&Paint);
        }
        break;

        case WM_SETCURSOR: {
            SetCursor(NULL);
        }
        break;
        
        default: {
            OutputDebugStringA("default\n");
            Result = DefWindowProc(Window, Message, WParam, LParam);
        }
        break;
    }

    return (Result);
}

//void DetectKeystrokes()
//{
//    if(GetKeyState(119) & 0x8000 || GetKeyState(87) & 0x8000) {
//        MetaData.UEntities->Player.PlayerAttr.MaintainingVelocity = true;
//        
//        if(GetKeyState(16) & 0x8000 )
//        {
//            MetaData.UEntities->Player.PlayerAttr.AcceleratingForward = true;
//        }
//    }
//    if(GetKeyState(97) & 0x8000 || GetKeyState(65) & 0x8000) {
//        MetaData.UEntities->Player.PlayerAttr.AcceleratingLeft = true;
//    }
//    else if(GetKeyState(100) & 0x8000 || GetKeyState(68) & 0x8000) {
//        MetaData.UEntities->Player.PlayerAttr.AcceleratingRight = true;
//    }
//}


int CALLBACK
WinMain(
    HINSTANCE Instance,
    HINSTANCE PrevInstance,
    LPSTR CommandLine,
    int ShowCode) {
    WNDCLASSA WindowClass = {};
    // TODO Check if HREDRAW/VREDRAW/OWNDC still matter
    WindowClass.style = CS_OWNDC | CS_HREDRAW; //Allocates a unique device context for each window of the class. Device context remembers the state of the window while drawing to the window
    WindowClass.lpfnWndProc = MainWindowCallback;
    WindowClass.hInstance = Instance;
    //WindowClass.hIcon = ;
    //WindowClass.hCursor = ;
    WindowClass.lpszClassName = "MindlessWindowClass";

    if (RegisterClassA(&WindowClass)) {

        HWND Window = Win32InitOpenGL(Instance);
        HDC WindowDC = GetDC(Window);

        MetaData.Running = true;

        arenaMetadata arenaMetadata;

        AllocHeadArena(arenaMetadata);

        InitOpenGlFunctions();

        InitBuffers_3D();

        InitOpenGlShaders();
        //InitEntities();

        //initialize game loop
        int DesiredTimePerFrame = 1000 / 60;
        LARGE_INTEGER BeginCounter;
        QueryPerformanceCounter(&BeginCounter);

        LARGE_INTEGER TickFrequencyResult;
        QueryPerformanceFrequency(&TickFrequencyResult);
        int64 TickFrequency = TickFrequencyResult.QuadPart;

        while (MetaData.Running) {
            MSG Message;
            if (PeekMessage(&Message, Window, 0, 0, PM_REMOVE)) {
                TranslateMessage(&Message);
                DispatchMessage(&Message);
            } else {

                /*DetectKeystrokes();*/
                //UpdateEntities();
    
                //Calculates new mouseposition
                float XPosMdpt = (float) MetaData.WindowWidth / 2;
                float YPosMdpt = (float) MetaData.WindowHeight / 2;
                //MetaData.UEntities->BlackHole.Position.X = (MetaData.MousePositionX - XPosMdpt) / XPosMdpt;
                //MetaData.UEntities->BlackHole.Position.Y = (MetaData.MousePositionY  - YPosMdpt) / (-YPosMdpt);

                //UpdateBuffers(MetaData.UEntities, EntityBuffer);
                Render(WindowDC);

                //Enforce 60fps
                LARGE_INTEGER EndCounter;
                QueryPerformanceCounter(&EndCounter);

                int64 ElapsedTime = EndCounter.QuadPart - BeginCounter.QuadPart;
                DWORD MSPerFrame = (DWORD)(1000.0f * ElapsedTime) / TickFrequency;

                if (MSPerFrame < DesiredTimePerFrame) {
                    DWORD SleepTime = DesiredTimePerFrame - (DWORD) MSPerFrame;
                    Sleep(SleepTime);
                }
                BeginCounter = EndCounter;
            }
        }
    }
    return (0);
}