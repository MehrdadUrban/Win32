#include <windows.h>
#include <cstdio>

HHOOK _hookID = NULL;

char const*  wParam2Button(WPARAM wParam) {
    DWORD fwKeys = GET_KEYSTATE_WPARAM(wParam);
    switch (fwKeys) {
        case WM_LBUTTONDOWN:
            return "WM_LBUTTONDOWN";
        case WM_LBUTTONUP:
            return "WM_LBUTTONUP";
        case WM_MOUSEMOVE:
            return "WM_MOUSEMOVE";
        case WM_MOUSEWHEEL:
            return "WM_MOUSEWHEEL";
        case WM_MOUSEHWHEEL:
            return "WM_MOUSEHWHEEL";
        case WM_RBUTTONDOWN:
            return "WM_RBUTTONDOWN";
        case WM_RBUTTONUP:
            return "WM_RBUTTONUP";
        case WM_MBUTTONDOWN:
            return "WM_MBUTTONDOWN";
        case WM_MBUTTONUP:
            return "WM_MBUTTONUP";
    }
    return "Unknown";
}


LRESULT Callback(int code, WPARAM wParam, LPARAM lParam) {

    
    tagMSLLHOOKSTRUCT* data = (tagMSLLHOOKSTRUCT*) lParam;
    char const* state = wParam2Button(wParam);
    if (wParam == WM_MOUSEWHEEL) {
        if (((int) (data->mouseData)) > 0)
            state = "WM_SBUTTONUP";
        else
            state = "WM_SBUTTONDOWN";
    }
    printf("wParam = %s (%d,%d) time: %d\n",state,data->pt.x,data->pt.y, data->time);

    return CallNextHookEx(_hookID, code, wParam, lParam);
}

DWORD WINAPI runner(LPVOID lpParameter){
    _hookID = SetWindowsHookEx(WH_MOUSE_LL, Callback, NULL, 0);
    MSG msg;
    DWORD hasMessage;
    do {
        hasMessage = GetMessage(&msg, NULL, 0, 0);
        printf("Received a message");
        printf("Message(%d,%d)\n",msg.lParam,msg.wParam);
        if (hasMessage == -1)
            break;
        if (hasMessage == 0) //WM_QUIT
            break;
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    } while (true);
    UnhookWindowsHookEx(_hookID);
    return 0;
};


int main(){
    DWORD ThreadId;
    HANDLE ThreadHandle = CreateThread(NULL,0,runner,NULL,0, &ThreadId);
    //Sleep(5000);
    int result = PostThreadMessage(ThreadId,WM_QUIT,0,0);
    printf("%d,%d",result,GetLastError());
    WaitForSingleObject(ThreadHandle, INFINITE);
}
