#include <windows.h>

const COLORREF MY_COLOR_KEY = RGB( 255, 0, 255 );
bool down = false;
HHOOK hook;
HWND hwnd;

int APIENTRY WinMain(
    HINSTANCE hInstance, HINSTANCE /*hPrevInst*/, LPTSTR /*lpCmdLine*/, int nCmdShow )
{
    WNDCLASSW wc{ sizeof( wc ) };
    wc.hInstance = hInstance;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.hCursor = LoadCursor( nullptr, IDC_ARROW );
    wc.hbrBackground = reinterpret_cast<HBRUSH>( COLOR_BTNFACE + 1 );
    wc.lpszClassName = L"MyTransparentFrame";

    wc.lpfnWndProc = []( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp ) -> LRESULT
    {
        switch( msg )
        {
        case WM_PAINT:
        {

            PAINTSTRUCT ps{};
            HDC hdc = BeginPaint( hwnd, &ps );

            RECT rc{};
            GetClientRect( hwnd, &rc );

            HPEN hPen = CreatePen( PS_SOLID, 1, 0x000000FF );
            HBRUSH hBrush = CreateSolidBrush( MY_COLOR_KEY );
            HGDIOBJ hOldBrush = SelectObject( hdc, hBrush );
            Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
            HGDIOBJ hOldPen = SelectObject( hdc, hPen );
            int midx = (rc.left + rc.right)/2;
            int midy = (rc.top+rc.bottom)/2;
            if(!down)
            {
                Ellipse( hdc, midx-5, midy-5, midx+5, midy+5 );
            }

            if( hOldPen )
                SelectObject( hdc, hOldPen );
            if( hOldBrush )
                SelectObject( hdc, hOldBrush );
            if( hPen )
                DeleteObject( hPen );
            if( hBrush )
                DeleteObject( hBrush );

            EndPaint( hwnd, &ps );
        }
        break;

        case WM_DESTROY:
            PostQuitMessage( 0 );
            break;

        case WM_NCHITTEST:
            return HTCAPTION;  // to be able to drag the window around
            break;

        default:
            return DefWindowProcW( hwnd, msg, wp, lp );
        }

        return 0;
    };

    RegisterClassW( &wc );

    HWND desktopWindow = GetDesktopWindow();
    RECT rect;
    GetClientRect(desktopWindow, &rect);
    HMODULE lib =  LoadLibrary("MooryangHook.dll");
    hook = SetWindowsHookEx(WH_MOUSE, (HOOKPROC) GetProcAddress(lib, "hookfunction"),hInstance,0);
    if(hook == NULL)
        MessageBox(hwnd, "dsaf", "dfa", MB_OK);
    FreeLibrary(lib);
    hwnd = CreateWindowExW( WS_EX_LAYERED | WS_EX_TOPMOST, wc.lpszClassName, L"", WS_POPUP,
                                 rect.left, rect.top, rect.right, rect.bottom, nullptr, nullptr, hInstance, nullptr );

    SetLayeredWindowAttributes( hwnd, MY_COLOR_KEY, 255, LWA_COLORKEY );
    ShowWindow( hwnd, nCmdShow );

    MSG msg;
    while( GetMessage( &msg, nullptr, 0, 0 ) )
    {
        TranslateMessage( &msg );
        DispatchMessage( &msg );
    }

    UnhookWindowsHookEx(hook);
    return (int) msg.wParam;
}
