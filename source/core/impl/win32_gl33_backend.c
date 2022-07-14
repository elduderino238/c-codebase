#include <Windows.h>

// From gl_functions.h
typedef i64 long_func();
typedef long_func* loader_func(const char* name);
void __LoadGLFunctions(loader_func* load_proc);

#define WGL_NUMBER_PIXEL_FORMATS_ARB 0x2000
#define WGL_DRAW_TO_WINDOW_ARB 0x2001
#define WGL_DRAW_TO_BITMAP_ARB 0x2002
#define WGL_ACCELERATION_ARB 0x2003
#define WGL_NEED_PALETTE_ARB 0x2004
#define WGL_NEED_SYSTEM_PALETTE_ARB 0x2005
#define WGL_SWAP_LAYER_BUFFERS_ARB 0x2006
#define WGL_SWAP_METHOD_ARB 0x2007
#define WGL_NUMBER_OVERLAYS_ARB 0x2008
#define WGL_NUMBER_UNDERLAYS_ARB 0x2009
#define WGL_TRANSPARENT_ARB 0x200A
#define WGL_TRANSPARENT_RED_VALUE_ARB 0x2037
#define WGL_TRANSPARENT_GREEN_VALUE_ARB 0x2038
#define WGL_TRANSPARENT_BLUE_VALUE_ARB 0x2039
#define WGL_TRANSPARENT_ALPHA_VALUE_ARB 0x203A
#define WGL_TRANSPARENT_INDEX_VALUE_ARB 0x203B
#define WGL_SHARE_DEPTH_ARB 0x200C
#define WGL_SHARE_STENCIL_ARB 0x200D
#define WGL_SHARE_ACCUM_ARB 0x200E
#define WGL_SUPPORT_GDI_ARB 0x200F
#define WGL_SUPPORT_OPENGL_ARB 0x2010
#define WGL_DOUBLE_BUFFER_ARB 0x2011
#define WGL_STEREO_ARB 0x2012
#define WGL_PIXEL_TYPE_ARB 0x2013
#define WGL_COLOR_BITS_ARB 0x2014
#define WGL_RED_BITS_ARB 0x2015
#define WGL_RED_SHIFT_ARB 0x2016
#define WGL_GREEN_BITS_ARB 0x2017
#define WGL_GREEN_SHIFT_ARB 0x2018
#define WGL_BLUE_BITS_ARB 0x2019
#define WGL_BLUE_SHIFT_ARB 0x201A
#define WGL_ALPHA_BITS_ARB 0x201B
#define WGL_ALPHA_SHIFT_ARB 0x201C
#define WGL_ACCUM_BITS_ARB 0x201D
#define WGL_ACCUM_RED_BITS_ARB 0x201E
#define WGL_ACCUM_GREEN_BITS_ARB 0x201F
#define WGL_ACCUM_BLUE_BITS_ARB 0x2020
#define WGL_ACCUM_ALPHA_BITS_ARB 0x2021
#define WGL_DEPTH_BITS_ARB 0x2022
#define WGL_STENCIL_BITS_ARB 0x2023
#define WGL_AUX_BUFFERS_ARB 0x2024
#define WGL_NO_ACCELERATION_ARB 0x2025
#define WGL_GENERIC_ACCELERATION_ARB 0x2026
#define WGL_FULL_ACCELERATION_ARB 0x2027
#define WGL_SWAP_EXCHANGE_ARB 0x2028
#define WGL_SWAP_COPY_ARB 0x2029
#define WGL_SWAP_UNDEFINED_ARB 0x202A
#define WGL_TYPE_RGBA_ARB 0x202B
#define WGL_TYPE_COLORINDEX_ARB 0x202C

#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#define WGL_CONTEXT_LAYER_PLANE_ARB 0x2093
#define WGL_CONTEXT_FLAGS_ARB 0x2094
#define WGL_CONTEXT_PROFILE_MASK_ARB 0x9126
#define WGL_CONTEXT_DEBUG_BIT_ARB 0x0001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB 0x0002
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002
#define ERROR_INVALID_VERSION_ARB 0x2095
#define ERROR_INVALID_PROFILE_ARB 0x2096

typedef HGLRC WINAPI W32_wglCreateContext(HDC);
typedef BOOL  WINAPI W32_wglDeleteContext(HGLRC);
typedef BOOL  WINAPI W32_wglMakeCurrent(HDC, HGLRC);
typedef PROC  WINAPI W32_wglGetProcAddress(LPCSTR);

typedef BOOL WINAPI W32_wglChoosePixelFormatARB(HDC hdc,
												const int* piAttribIList,
												const FLOAT* pfAttribFList,
												UINT nMaxFormats,
												int* piFormats,
												UINT* nNumFormats);
typedef HGLRC WINAPI W32_wglCreateContextAttribsARB(HDC hdc, HGLRC hShareContext,
													const int* attribList);
typedef HGLRC WINAPI W32_wglCreateContextAttribsARB(HDC hdc, HGLRC hShareContext,
													const int* attribList);
typedef BOOL WINAPI W32_wglSwapLayerBuffers(HDC hdc, UINT plane);
W32_wglSwapLayerBuffers* v_wglSwapLayerBuffers;

typedef struct W32_Window {
	u32 width;
	u32 height;
	string title;
	HWND handle;
} W32_Window;

static LRESULT CALLBACK Win32Proc(HWND window, UINT msg, WPARAM wparam, LPARAM lparam) {
    LRESULT result = 0;
    
    switch (msg) {
        case WM_CLOSE:
        case WM_DESTROY: {
            PostQuitMessage(0);
        } break;
        
		default: {
            result = DefWindowProcA(window, msg, wparam, lparam);
        } break;
    }
    
    return result;
}

void B_BackendInit(OS_Window* _window) {
	W32_Window* window = (W32_Window*) _window;
	
	//- Load Preliminary WGL functions 
	HINSTANCE hinstance = GetModuleHandle(0);
	HGLRC real_context;
	
	HMODULE opengl_module = LoadLibraryA("opengl32.dll");
	
	W32_wglCreateContext* v_wglCreateContext = (W32_wglCreateContext*) GetProcAddress(opengl_module, "wglCreateContext");
	if (!v_wglCreateContext) {
		LogReturn(, "Win32 OpenGL WGL Function Loading: Loading wglCreateContext failed");
	}
	W32_wglDeleteContext* v_wglDeleteContext = (W32_wglDeleteContext*) GetProcAddress(opengl_module, "wglDeleteContext");
	if (!v_wglDeleteContext) {
		LogReturn(, "Win32 OpenGL WGL Function Loading: Loading wglDeleteContext failed");
	}
	W32_wglMakeCurrent* v_wglMakeCurrent = (W32_wglMakeCurrent*) GetProcAddress(opengl_module, "wglMakeCurrent");
	if (!v_wglMakeCurrent) {
		LogReturn(, "Win32 OpenGL WGL Function Loading: Loading wglMakeCurrent failed");
	}
	W32_wglGetProcAddress* v_wglGetProcAddress = (W32_wglGetProcAddress*) GetProcAddress(opengl_module, "wglGetProcAddress");
	if (!v_wglGetProcAddress) {
		LogReturn(, "Win32 OpenGL WGL Function Loading: Loading wglGetProcAddress failed");
	}
	W32_wglSwapLayerBuffers* v_wglSwapLayerBuffers = (W32_wglSwapLayerBuffers*) GetProcAddress(opengl_module, "wglSwapLayerBuffers");
	if (!v_wglSwapLayerBuffers) {
		LogReturn(, "Win32 OpenGL WGL Function Loading: Loading wglSwapLayerBuffers failed");
	}
	W32_wglChoosePixelFormatARB* v_wglChoosePixelFormatARB;
	W32_wglCreateContextAttribsARB* v_wglCreateContextAttribsARB;
	
	//- Create Dummy OpenGL Context 
	{
		WNDCLASSA wc = {
			.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC,
			.lpfnWndProc = DefWindowProcA,
			.hInstance = GetModuleHandle(0),
			.lpszClassName = "Dummy_WGL_weee",
		};
		if (!RegisterClassA(&wc)) {
			LogReturn(, "Win32 OpenGL Window: New Class Registration failed");
		}
		
		HWND bootstrap_window = CreateWindowExA(
												0,
												wc.lpszClassName,
												"Dummy OpenGL Window",
												0,
												CW_USEDEFAULT,
												CW_USEDEFAULT,
												CW_USEDEFAULT,
												CW_USEDEFAULT,
												0,
												0,
												wc.hInstance,
												0);
		if (!bootstrap_window) {
			LogReturn(, "Win32 OpenGL Bootstrap: Window Creation Failed");
		}
		
		HDC dc = GetDC(bootstrap_window);
		
		PIXELFORMATDESCRIPTOR format_desc = {
			.nSize = sizeof(PIXELFORMATDESCRIPTOR),
			.nVersion = 1,
			.iPixelType = PFD_TYPE_RGBA,
			.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
			.cColorBits = 32,
			.cAlphaBits = 8,
			.iLayerType = PFD_MAIN_PLANE,
			.cDepthBits = 24,
			.cStencilBits = 8,
		};
		
		i32 format_idx = ChoosePixelFormat(dc, &format_desc);
		if (!format_idx) {
			ReleaseDC(bootstrap_window, dc);
			LogReturn(, "Win32 OpenGL Bootstrap: Choosing appropriate PixelFormat Failed");
		}
		if (!SetPixelFormat(dc, format_idx, &format_desc)) {
			ReleaseDC(bootstrap_window, dc);
			LogReturn(, "Win32 OpenGL Bootstrap: Setting the PixelFormat Failed");
		}
		
		HGLRC bootstrap_context = v_wglCreateContext(dc);
		if (!bootstrap_context) {
			ReleaseDC(bootstrap_window, dc);
			LogReturn(, "Win32 OpenGL Bootstrap: Context Creation Failed");
		}
		if (!v_wglMakeCurrent(dc, bootstrap_context)) {
			ReleaseDC(window->handle, dc);
			LogReturn(, "Win32 OpenGL Bootstrap: Context Activation Failed");
		}
		
		//- Load New ARB WGL functions using Dummy Context 
		v_wglChoosePixelFormatARB = (W32_wglChoosePixelFormatARB*) v_wglGetProcAddress("wglChoosePixelFormatARB");
		if (!v_wglChoosePixelFormatARB) {
			ReleaseDC(bootstrap_window, dc);
			LogReturn(, "Win32 OpenGL WGL Function Loading: Loading wglChoosePixelFormatARB failed");
		}
		v_wglCreateContextAttribsARB = (W32_wglCreateContextAttribsARB*) v_wglGetProcAddress("wglCreateContextAttribsARB");
		if (!v_wglCreateContextAttribsARB) {
			ReleaseDC(bootstrap_window, dc);
			LogReturn(, "Win32 OpenGL WGL Function Loading: Loading wglCreateContextAttribsARB failed");
		}
		
		//- Delete Bootstrapping Stuff 
		v_wglMakeCurrent(dc, 0);
		BOOL delete_context = v_wglDeleteContext(bootstrap_context);
		if (!delete_context) {
			LogReturn(, "Win32 OpenGL Bootstrap Window Destruction: Context Deletion Failed");
		}
		ReleaseDC(bootstrap_window, dc);
		if (!DestroyWindow(bootstrap_window)) {
			LogReturn(, "Win32 OpenGL Bootstrap Window Destruction: Window Deletion Failed");
		}
		UnregisterClassA("Dummy_WGL_weee", hinstance);
	}
	
	//- Create Actual Context 
	{
		HDC dc = GetDC(window->handle);
		
		int format_attribs_i[] = {
			WGL_DRAW_TO_WINDOW_ARB,     true,
			WGL_SUPPORT_OPENGL_ARB,     true,
			WGL_DOUBLE_BUFFER_ARB,      true,
			WGL_ACCELERATION_ARB,       WGL_FULL_ACCELERATION_ARB,
			WGL_PIXEL_TYPE_ARB,         WGL_TYPE_RGBA_ARB,
			WGL_COLOR_BITS_ARB,         32,
			WGL_DEPTH_BITS_ARB,         24,
			WGL_STENCIL_BITS_ARB,       8,
			0
		};
		
		int format_idx;
		UINT num_formats;
		v_wglChoosePixelFormatARB(dc, format_attribs_i, 0, 1, &format_idx, &num_formats);
		if (!num_formats) {
			ReleaseDC(window->handle, dc);
			LogReturn(, "Win32 OpenGL Window: Context Choosing Pixel Format failed");
		}
		
		PIXELFORMATDESCRIPTOR format_desc = {0};
		DescribePixelFormat(dc, format_idx, sizeof(PIXELFORMATDESCRIPTOR), &format_desc);
		if (!SetPixelFormat(dc, format_idx, &format_desc)) {
			ReleaseDC(window->handle, dc);
			LogReturn(, "Win32 OpenGL Window: Context SetPixelFormat failed");
		}
		
		int context_attribs[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
			WGL_CONTEXT_MINOR_VERSION_ARB, 3,
			WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
			0,
		};
		
		real_context = v_wglCreateContextAttribsARB(dc, 0, context_attribs);
		if (!real_context) {
			ReleaseDC(window->handle, dc);
			LogReturn(, "Win32 OpenGL Window: Context Creation Failed");
		}
		if (!v_wglMakeCurrent(dc, real_context)) {
			ReleaseDC(window->handle, dc);
			LogReturn(, "Win32 OpenGL Window: Context Activation Failed");
		}
		
		__LoadGLFunctions(v_wglGetProcAddress);
		
		ReleaseDC(window->handle, dc);
	}
}

void B_BackendSwapchainNext(OS_Window* _window) {
	W32_Window* window = (W32_Window*) _window;
	HDC dc = GetDC(window->handle);
	SwapBuffers(dc);
	ReleaseDC(window->handle, dc);
}
