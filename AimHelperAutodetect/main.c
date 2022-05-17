#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int main()
{
    char name[300];
    printf("캡션명 입력 \n");
    gets(name);
    HWND callOfWindow = FindWindow(NULL, name);
    if(!callOfWindow)
    {
        printf("%s 없다 \n", name);
        return -1;
    }
    RECT rect;
    GetClientRect(callOfWindow, &rect);
    printf("%d %d %d %d\n", rect.left, rect.top, rect.right, rect.bottom);
    int x = (rect.left+rect.right) /2;
    int y = (rect.top + rect.bottom) /2;
    HDC hDC = CreateDC("DISPLAY", NULL, NULL, NULL);
    if (hDC)
    {
        TextOut(hDC, 0, 0, "Hello world!", 12);
        DeleteDC(hDC);
    }
    return 0;
}
