#pragma once

#ifdef _WIN32
#include <windows.h>

LRESULT CALLBACK WindowMessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_CLOSE:
        {
            isCloseRequested = true;
            return 0;
        }
    }
    return (DefWindowProc(hWnd, uMsg, wParam, lParam));
}


#include "../window_handler.hpp"
#include <iostream>

class WindowHandler_Windows : public WindowHandler {
    public:
        void Create(const char* name, int width, int height, int options) override {
            if(!(instanceHandle = GetModuleHandle(0)))
            {
                Log::RuntimeError("glw::create failed to get instance handle!");
                return;
            }
            
            windowClass.cbSize = sizeof(WNDCLASSEX);
            windowClass.style = CS_HREDRAW | CS_VREDRAW;
            windowClass.lpfnWndProc = WindowMessageHandler;
            windowClass.cbClsExtra = 0;
            windowClass.cbWndExtra = 0;
            windowClass.hInstance = instanceHandle;
            windowClass.hCursor = LoadCursor(0,IDC_ARROW);
            windowClass.hIcon = LoadIcon(0, IDI_APPLICATION);
            windowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
            windowClass.lpszClassName = "atomus_window_class";
            windowClass.lpszMenuName = "menu_name";
            windowClass.hIconSm = LoadIcon(0, IDI_APPLICATION);

            if(!RegisterClassEx(&windowClass))
            {
                std::cout << "glw::create failed to register window class!";
            }

            if(!(windowHandle = CreateWindowEx( 0,
                                            "atomus_window_class",
                                            name,
                                            WS_OVERLAPPEDWINDOW,
                                            0,
                                            0,
                                            width,
                                            height,
                                            0,
                                            0,
                                            instanceHandle,
                                            0)))
            {
                Log::RuntimeError("glw::create failed to create window!");
            }

            if(!(deviceContextHandle = GetDC(windowHandle)))
            {
                Log::RuntimeError("glw::create failed to get device context handle!");
            }

            PIXELFORMATDESCRIPTOR pixelFormatDescriptor;
            pixelFormatDescriptor.nSize = sizeof(pixelFormatDescriptor);
            pixelFormatDescriptor.nVersion = 1;
            pixelFormatDescriptor.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
            pixelFormatDescriptor.iPixelType = PFD_TYPE_RGBA;
            pixelFormatDescriptor.cColorBits = 32;
            pixelFormatDescriptor.cDepthBits = 32;
            pixelFormatDescriptor.iLayerType = PFD_MAIN_PLANE;

            unsigned int pixelFormat;

            if(!(pixelFormat = ChoosePixelFormat(deviceContextHandle, &pixelFormatDescriptor)))
            {
                Log::RuntimeError("glw::create failed to find suitable pixel format!");
            }
            if(!SetPixelFormat(deviceContextHandle, pixelFormat, &pixelFormatDescriptor))
            {
                Log::RuntimeError("glw::create failed to set pixel format!");
            }

            /*TODO:
                Write a generic context handler  
            */
            HGLRC temporaryContext = wglCreateContext(deviceContextHandle);

            if(!temporaryContext)
            {
                Log::RuntimeError("glw::create failed to create temporary context!");
            }
            if (!(wglMakeCurrent(deviceContextHandle, temporaryContext)))
            {
                Log::RuntimeError("glw::create failed to activate temporary context!");
            }

            int major;
            int minor;
            //gl::GetIntegerv(gl::MAJOR_VERSION, &major);
            //gl::GetIntegerv(gl::MINOR_VERSION, &minor);
            if(!(major >= 3 && minor >= 3))
            {
                Log::RuntimeError("glw::create opengl 3.3 ist not supported!");
            }

            int attribs[] =
            {
                WGL_CONTEXT_MAJOR_VERSION_ARB, major,
                WGL_CONTEXT_MINOR_VERSION_ARB, minor,
                WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
                WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
                0
            };

            PFNWGLCREATEBUFFERREGIONARBPROC wglCreateContextAttribsARB = (PFNWGLCREATEBUFFERREGIONARBPROC)wglGetProcAddress( "wglCreateContextAttribsARB" );
/* 
            if(!wglCreateContextAttribsARB)
            {
                std::cout << "glw::create failed to find pointer to wglCreateContextAttribsARB function!");
            }
            if(!(openglContextHandle = (HGLRC)wglCreateContextAttribsARB(deviceContextHandle, 0, (UINT)attribs)))
            {
                std::cout << "glw::create failed to create forward compatible context!");
            }

            wglMakeCurrent( NULL, NULL );
            wglDeleteContext( temporaryContext );

            if(!wglMakeCurrent(deviceContextHandle, (HGLRC)openglContextHandle))
            {
                std::cout << "glw::create failed to activate forward compatible context!");
            }
*/
            ShowWindow(windowHandle, SW_SHOW);
            UpdateWindow(windowHandle);

            isCloseRequested = false;
        }
        void Resize(int, int) override {}

        void Update() override {

        }

        void Destroy() override {
            DestroyWindow(windowHandle);
            windowHandle = 0;
            UnregisterClass(windowClass.lpszClassName, instanceHandle);
        }

        void Handler() override {
            MSG message;
            while (PeekMessage (&message, 0, 0, 0, PM_REMOVE) > 0) //Or use an if statement
            {
                TranslateMessage (&message);
                DispatchMessage (&message);
            }
        }
        
        void SwapBuffers() {
            ::SwapBuffers(deviceContextHandle);
        }

        void setClickCallback(std::function<void()>) override {};
        void setDestroyCallback(std::function<void()>) override {}
        void setResizeCallback(std::function<void()>) override {}
    private:
    	
        LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);               // Declaration For WndProc

        HINSTANCE instanceHandle;
        WNDCLASSEX windowClass;
        HWND windowHandle;
        HDC deviceContextHandle;
        HGLRC openglContextHandle;

};

#endif
