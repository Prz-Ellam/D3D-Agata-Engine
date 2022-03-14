#include "pch.h"
#include "Window.h"
#include "Renderer.h"

namespace Agata {

    bool Window::s_WindowClassRegistered = false;
    const std::string Window::s_WindowClassName = "Agata Window";
    HINSTANCE Window::s_Instance = GetModuleHandle(nullptr);
    uint32_t Window::s_WindowsCount = 0u;

    Window::Window() : m_Width(640u), m_Height(480u), m_Title("Default Title"), m_VSync(false) {

        InitWindow();
        SetDefaultHandlers();

    }

    Window::Window(const WindowParams& params) : m_Width(params.Width), m_Height(params.Height),
    m_Title(params.Title), m_VSync(false) {

        InitWindow();
        SetDefaultHandlers();
        InitJoystick();

    }

    Window::Window(uint32_t width, uint32_t height, const std::string& title)
        : m_Width(width), m_Height(height), m_Title(title), m_VSync(false) {

        InitWindow();
        SetDefaultHandlers();
        InitJoystick();

    }

    Window::~Window() {

        ReleaseWindow();

    }

    void Window::Show() {

        ShowWindow(m_Handler, SW_SHOWDEFAULT);

    }

    void Window::Hide() {

        ShowWindow(m_Handler, SW_HIDE);

    }

    void Window::Update() {

        MSG msg = {};
        while (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE) > 0) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        Renderer::m_SwapChain->Present(static_cast<UINT>(m_VSync), 0u);

    }

    HWND Window::GetHandler() const {

        return m_Handler;

    }

    uint32_t Window::GetWidth() const {

        return m_Width;

    }

    uint32_t Window::GetHeight() const {

        return m_Height;

    }

    std::string Window::GetTitle() const {

        return m_Title;

    }

    void Window::SetTitle(const std::string& title) {

        SetWindowText(m_Handler, title.c_str());
        m_Title = title;

    }

    void Window::SetIcon(const std::string& path) {

        HICON hIcon = (HICON)LoadImage(nullptr, path.c_str(), IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
        SendMessage(m_Handler, WM_SETICON, ICON_BIG, (LPARAM)hIcon);

    }

    void Window::ShowCursor() {

        ::ShowCursor(true);
        m_CursorVisible = true;

    }


    void Window::HideCursor() {

        ::ShowCursor(false);
        m_CursorVisible = false;

    }

    bool Window::GetVSync() const {

        return m_VSync;

    }

    void Window::SetVSync(bool vSync) {

        m_VSync = vSync;

    }

    bool Window::RegisterWindowClass() {

        if (!s_WindowClassRegistered) {

            WNDCLASSEX wndClass = { 0 };
            wndClass.cbSize = sizeof(WNDCLASSEX);
            wndClass.style = CS_HREDRAW | CS_VREDRAW;
            wndClass.lpfnWndProc = StaticMessageHandler;
            wndClass.cbClsExtra = 0;
            wndClass.cbWndExtra = 0;
            wndClass.hInstance = s_Instance;
            wndClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
            wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
            wndClass.hbrBackground = (HBRUSH)WHITE_BRUSH;
            wndClass.lpszMenuName = nullptr;
            wndClass.lpszClassName = s_WindowClassName.c_str();
            wndClass.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

            ATOM result = RegisterClassEx(&wndClass);

            if (result) {
                s_WindowClassRegistered = true;
            }

            return result;

        }

        return true;

    }

    bool Window::InitWindow() {

        if (!RegisterWindowClass()) {
            return false;
        }

        RECT rc = { 0, 0, static_cast<LONG>(m_Width), static_cast<LONG>(m_Height) };
        AdjustWindowRectEx(&rc, WS_OVERLAPPEDWINDOW, FALSE, 0u);

        m_Handler = CreateWindowEx(0u, 
            s_WindowClassName.c_str(), 
            m_Title.c_str(), 
            WS_OVERLAPPEDWINDOW,
            GetSystemMetrics(SM_CXSCREEN) / 2 - (rc.right - rc.left) / 2, 
            GetSystemMetrics(SM_CYSCREEN) / 2 - (rc.bottom - rc.top) / 2, 
            rc.right - rc.left, 
            rc.bottom - rc.top, 
            nullptr, 
            nullptr,
            s_Instance, 
            nullptr);

        if (!m_Handler) {
            return false;
        }

        s_WindowsCount++;
        SetWindowLongPtr(m_Handler, GWLP_USERDATA, reinterpret_cast<DWORD_PTR>(this));

        m_MouseButtons[0x0001] = KeyCode::LeftButton;
        m_MouseButtons[0x0002] = KeyCode::RightButton;
        m_MouseButtons[0x0010] = KeyCode::MiddleButton;
        /*m_MouseButtons[0x0020] = KeyCode::X1Button;
        m_MouseButtons[0x0040] = KeyCode::X2Button;*/
        
        return true;

    }

    void Window::ReleaseWindow() {

        DestroyWindow(m_Handler);
        if (--s_WindowsCount == 0u) {
            UnregisterClass(s_WindowClassName.c_str(), s_Instance);
        }

    }

    void Window::SetDefaultHandlers() {

        m_WindowResizeEventHandler = [](WindowResizeEvent e) {};
        m_WindowMoveEventHandler = [](WindowMoveEvent e) {};
        m_MouseMoveEventHandler = [](MouseMoveEvent e) {};
        m_MouseScrollEventHandler = [](MouseScrollEvent e) {};
        m_WindowCloseEventHandler = [](WindowCloseEvent e) {};
        m_KeyEventHandler = [](KeyEvent e) {};
        m_MouseButtonEventHandler = [](MouseButtonEvent e) {};
        m_CharEventHandler = [](CharEvent e) {};
        m_JoyStickEventHandler = [](JoyStickEvent e) {};

    }

    void Window::InitJoystick() {

        JOYINFO joyinfo; 
        UINT wNumDevs, wDeviceID; 
        BOOL bDev1Attached, bDev2Attached; 

        if ((wNumDevs = joyGetNumDevs()) == 0) {
            return;
        }

        bDev1Attached = joyGetPos(JOYSTICKID1, &joyinfo) != JOYERR_UNPLUGGED; 
        bDev2Attached = wNumDevs == 2 && joyGetPos(JOYSTICKID2, &joyinfo) != JOYERR_UNPLUGGED; 

        if (bDev1Attached || bDev2Attached) {
            wDeviceID = bDev1Attached ? JOYSTICKID1 : JOYSTICKID2;
            joySetCapture(m_Handler, JOYSTICKID1, NULL, FALSE);
        }
        else {
            return;
        }

    }

    LRESULT Window::StaticMessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

        Window* window = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

        if (window) {
            window->MessageHandler(hWnd, uMsg, wParam, lParam);
        }

        return DefWindowProc(hWnd, uMsg, wParam, lParam);

    }

    LRESULT Window::MessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

        switch (uMsg) {
            case WM_PAINT: {
                PAINTSTRUCT ps;
                HDC hDC = BeginPaint(hWnd, &ps);
                FillRect(hDC, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
                EndPaint(hWnd, &ps);
                break;
            }
            case WM_KEYUP:
            case WM_KEYDOWN: {

                // 0 is Up /// 1 is Down
                int ps = (lParam & (1ll << 30ll)) >> 30;
                int state = ((lParam & (1ll << 31ll)) == 0);

                KeyAction keyAction = static_cast<KeyAction>((ps + state) * state);
                KeyCode keyCode = static_cast<KeyCode>(wParam);

                m_KeyEventHandler(KeyEvent(keyCode, keyAction));
                break;
            }
            case WM_CHAR: {
                KeyCode keyCode = static_cast<KeyCode>(wParam);

                m_CharEventHandler(CharEvent(keyCode));
                break;
            }
            case WM_SIZE: {
                m_Width = LOWORD(lParam);
                m_Height = HIWORD(lParam);
                m_WindowResizeEventHandler(WindowResizeEvent(m_Width, m_Height));
                break;
            }
            case WM_MOVE: {
                POINTS pt = MAKEPOINTS(lParam);
                m_WindowMoveEventHandler(WindowMoveEvent(pt.x, pt.y));
                break;
            }
            case WM_MOUSEMOVE: {
                int x = GET_X_LPARAM(lParam);
                int y = GET_Y_LPARAM(lParam);
                m_MouseMoveEventHandler(MouseMoveEvent(x, y));
                break;
            }
            case WM_RBUTTONDOWN:
            case WM_MBUTTONDOWN:
            case WM_LBUTTONDOWN:
            /*case WM_XBUTTONDOWN:*/ {
                int x = GET_X_LPARAM(lParam);
                int y = GET_Y_LPARAM(lParam);
                KeyAction keyAction = KeyAction::Press;
                KeyCode keyCode = m_MouseButtons[static_cast<uint32_t>(wParam)];
                break;
            }
            case WM_RBUTTONUP:
            case WM_MBUTTONUP:
            case WM_LBUTTONUP:
            case WM_XBUTTONUP: {
                int dummy = -1;
                break;
            }
            case WM_MOUSEWHEEL: {
                int x = GET_X_LPARAM(lParam);
                int y = GET_Y_LPARAM(lParam);
                int delta = GET_WHEEL_DELTA_WPARAM(wParam);
                m_MouseScrollEventHandler(MouseScrollEvent(x, y, delta));
                break;
            }
            case MM_JOY1MOVE: {

                // Devuelve un valor entre 0 a 65535
                int xPos = LOWORD(lParam); 
                int yPos = HIWORD(lParam); 

                // Es convertido del rango [0 - 65535] a [-65535 a 65535]
                xPos = xPos * 2 - UINT16_MAX;
                yPos = yPos * 2 - UINT16_MAX;

                // Se normaliza para que este en un rango de [-1 a 1]
                float x = static_cast<float>(xPos) / UINT16_MAX;
                float y = static_cast<float>(yPos) / UINT16_MAX;

                if (wParam == JOY_BUTTON4)
                int dummy = -1;
                break;
            }
            case MM_JOY1BUTTONDOWN: {
                int dummy = -1;
                break;
            }
            case MM_JOY1BUTTONUP: {
                int dummy = -1;
                break;
            }
            case WM_CLOSE: {
                m_WindowCloseEventHandler(WindowCloseEvent());
                return FALSE;
            }
        }

        return FALSE;

    }

    void Window::SetWindowResizeEventHandler(WindowResizeEventFn handler) {

        m_WindowResizeEventHandler = handler;

    }

    void Window::SetWindowMoveEventHandler(WindowMoveEventFn handler) {

        m_WindowMoveEventHandler = handler;

    }

    void Window::SetMouseMoveEventHandler(MouseMoveEventFn handler) {

        m_MouseMoveEventHandler = handler;

    }

    void Window::SetMouseScrollEventHandler(MouseScrollEventFn handler) {

        m_MouseScrollEventHandler = handler;

    }

    void Window::SetWindowCloseEventHandler(WindowCloseEventFn handler) {

        m_WindowCloseEventHandler = handler;

    }

    void Window::SetKeyEventHandler(KeyEventFn handler) {

        m_KeyEventHandler = handler;

    }

    void Window::SetMouseButtonEventHandler(MouseButtonEventFn handler) {

        m_MouseButtonEventHandler = handler;

    }

    void Window::SetCharEventHandler(CharEventFn handler) {

        m_CharEventHandler = handler;

    }

    void Window::SetJoyStickEventHandler(JoyStickEventFn handler) {

        m_JoyStickEventHandler = handler;

    }

}