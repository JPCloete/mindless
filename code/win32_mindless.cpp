#include <windows.h>
#include <gl/GL.h>

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "opengl32.lib")

#define internal static
#define local_persist static
#define global_variable static

// TODO This is global for now
global_variable bool Running;

global_variable BITMAPINFO BitmapInfo;
global_variable void *BitmapMemory;

internal void
Win32ResizeDIBSection(int Width, int Height)
{

  BitmapInfo.bmiHeader.biSize = sizeof(BitmapInfo.bmiHeader);
  BitmapInfo.bmiHeader.biWidth = Width;
  BitmapInfo.bmiHeader.biHeight = Height;
  BitmapInfo.bmiHeader.biPlanes = 1;
  BitmapInfo.bmiHeader.biBitCount = 32;
  BitmapInfo.bmiHeader.biCompression = BI_RGB;
  
}

internal void
Win32UpdateWindow(HDC DeviceContext, int X, int Y, int Width, int Height)
{
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
    BitmapMemory,
    &BitmapInfo,
    DIB_RGB_COLORS,
    SRCCOPY
  );
}

LRESULT CALLBACK MainWindowCallback(
  HWND   Window,
  UINT   Message,
  WPARAM WParam,
  LPARAM LParam)
{
  LRESULT Result = 0;

  switch(Message)
  {
    case WM_SIZE:
    {
      RECT ClientRect;
      GetClientRect(Window, &ClientRect);
      int Width = ClientRect.right - ClientRect.left;
      int Height = ClientRect.bottom - ClientRect.top; 
      Win32ResizeDIBSection(Width, Height);
    } break;

    case WM_DESTROY:
    {
      //TODO Handle as error
      Running = false;
    } break;

    case WM_CLOSE:
    {
      //TODO Handle as message
     Running = false;
    } break;

    case WM_ACTIVATEAPP:
    {
      OutputDebugStringA("WM_ACTIVATEAPP\n");
    } break;

    case WM_PAINT:
    {
      PAINTSTRUCT Paint;
      HDC DeviceContext = BeginPaint(Window, &Paint);
      int X = Paint.rcPaint.left;
      int Y = Paint.rcPaint.top;
      int Width = Paint.rcPaint.right - Paint.rcPaint.left;
      int Height = Paint.rcPaint.bottom - Paint.rcPaint.top;
      Win32UpdateWindow(DeviceContext, X, Y, Width, Height);
      EndPaint(Window, &Paint);
    } break;

    default: 
    {
      OutputDebugStringA("default\n");
      Result = DefWindowProc(Window, Message, WParam, LParam);
    } break;
  }

  return(Result);
}

void
Win32InitOpenGL(HWND Window)
{
  HDC WindowDC = GetDC(Window);

  PIXELFORMATDESCRIPTOR DesiredPixelFormat = {};
  DesiredPixelFormat.nSize = sizeof(DesiredPixelFormat);
  DesiredPixelFormat.nVersion = 1;
  DesiredPixelFormat.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
  DesiredPixelFormat.cColorBits = 24;
  DesiredPixelFormat.cAlphaBits = 8;
  DesiredPixelFormat.iLayerType = PFD_MAIN_PLANE;

  int SuggestedPixelFormatIndex = ChoosePixelFormat(WindowDC, &DesiredPixelFormat);
  PIXELFORMATDESCRIPTOR SuggestedPixelFormat;
  DescribePixelFormat(WindowDC, SuggestedPixelFormatIndex, sizeof(SuggestedPixelFormat), &SuggestedPixelFormat);
  SetPixelFormat(WindowDC, SuggestedPixelFormatIndex, &SuggestedPixelFormat); 

  HGLRC OpenGLRC = wglCreateContext(WindowDC);
  if(wglMakeCurrent(WindowDC, OpenGLRC))
  {
    glClearColor(1.0f, 0.0f, 1.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    SwapBuffers(WindowDC);
  }
  else
  {
   //TODO Error Handling 
  }
  ReleaseDC(Window, WindowDC);
}

int CALLBACK
WinMain(
  HINSTANCE Instance,
  HINSTANCE PrevInstance,
  LPSTR CommandLine,
  int ShowCode)
{
  WNDCLASSA WindowClass = {};
  // TODO Check if HREDRAW/VREDRAW/OWNDC still matter
  WindowClass.style = CS_OWNDC|CS_HREDRAW; //Allocates a unique device context for each window of the class. Device context remembers the state of the window while drawing to the window
  WindowClass.lpfnWndProc = MainWindowCallback;
  WindowClass.hInstance = Instance;
  //WindowClass.hIcon = ;
  //WindowClass.hCursor = ;
  WindowClass.lpszClassName = "MindlessWindowClass";

  if(RegisterClassA(&WindowClass))
  {
    HWND Window = CreateWindowExA
    (
      0,
      WindowClass.lpszClassName,
      "Mindless",
      WS_OVERLAPPEDWINDOW|WS_VISIBLE,
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      0,
      0,
      Instance,
      0
    );
    if(Window)
    {
      Win32InitOpenGL(Window);

      Running = true;
      while(Running)
      {
        MSG Message;
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
      // TODO Logging
    }
  }
  else
  {
    // TODO Logging
  }

  return(0);
}
