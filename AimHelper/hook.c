#include <windows.h>
static BOOL down;

void * fuc
__declspec(dllexport) LRESULT hookfunction(_In_ int    nCode, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
    //if(nCode < 0)
    if(nCode >= 0 && nCode != HC_NOREMOVE)
    {
        switch (wParam)
        {
        case WM_RBUTTONDOWN:
        {
            down = TRUE;
            InvalidateRect(hwnd, NULL, TRUE);
            break;
        }
        case WM_RBUTTONUP:
            down = FALSE;
            InvalidateRect(hwnd, NULL, TRUE);
            break;
        }

    }
    return CallNextHookEx(hook, nCode, wParam, lParam);

}
