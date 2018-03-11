#define WINVER 0x0500
#include <windows.h>
#include <stdio.h>
 
int main(int argc, char* argv[])
{

    // we use the window title to make sure the game is open
    char *windowTitle = "Overwatch";
    char *userName = "foo@example.com";
    char *password = "myeasypassword";

    // get the current keyboard layout
    HKL currentKBL = GetKeyboardLayout(0);

    // open the process
    char szPath[] = "C:\\Program Files (x86)\\Overwatch\\Overwatch.exe";
    PROCESS_INFORMATION pif;
    STARTUPINFO si;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);

    BOOL bRet = CreateProcess(
        szPath,
        NULL,
        NULL,
        NULL,
        FALSE,
        0,
        NULL,
        NULL,
        &si,
        &pif
    );
 
    if (bRet == FALSE) {
        fprintf(stderr, "Failed to open %s\n", szPath);
        return 1;
    }

    unsigned int i = 0;
    unsigned int userLen = strlen(userName);
    unsigned int passLen = strlen(password);
    SHORT kc;
    INPUT ip;

    printf("Running ");
    while(1) {
        HWND window = FindWindow(NULL, TEXT("Overwatch"));
        if (window != NULL) {
            printf(" OK\n");

            // FIXME: this is a gross hack. Instead of making sure it's finally loaded
            // we're just waiting 8 seconds. I actually hate this.
            Sleep(8000);

            // send Overwatch to the foreground if it's not already
            SetForegroundWindow(window);

            // enter the email
            for (i = 0; i < userLen; i++) {
                kc = VkKeyScanEx(userName[i], currentKBL);
                // if it's an @ symbol we need to do some weird shit
                if (userName[i] == '@') {
                    // Press the "Ctrl" key
                    ip.ki.wVk = VK_SHIFT;
                    ip.ki.dwFlags = 0; // 0 for key press
                    SendInput(1, &ip, sizeof(INPUT));
             
                    // Press the "V" key
                    ip.ki.wVk = kc;
                    ip.ki.dwFlags = 0; // 0 for key press
                    SendInput(1, &ip, sizeof(INPUT));
             
                    // Release the "V" key
                    ip.ki.wVk = kc;
                    ip.ki.dwFlags = KEYEVENTF_KEYUP;
                    SendInput(1, &ip, sizeof(INPUT));
             
                    // Release the "Ctrl" key
                    ip.ki.wVk = VK_SHIFT;
                    ip.ki.dwFlags = KEYEVENTF_KEYUP;
                    SendInput(1, &ip, sizeof(INPUT));
                } else {
                    ip.type = INPUT_KEYBOARD;
                    ip.ki.wVk = kc;
                    ip.ki.wScan = 0;
                    ip.ki.dwFlags = 0;
                    ip.ki.time = 0;
                    ip.ki.dwExtraInfo = 0;
                    SendInput(1, &ip, sizeof(INPUT));
                }
                Sleep(100);
            }

            // tab then enter password
            ip.type = INPUT_KEYBOARD;
            ip.ki.wVk = VK_TAB;
            ip.ki.wScan = 0;
            ip.ki.dwFlags = 0;
            ip.ki.time = 0;
            ip.ki.dwExtraInfo = 0;
            SendInput(1, &ip, sizeof(INPUT));
            
            for (i = 0; i < passLen; i++) {
                kc = VkKeyScanEx(password[i], currentKBL);
                ip.type = INPUT_KEYBOARD;
                ip.ki.wVk = kc;
                ip.ki.wScan = 0;
                ip.ki.dwFlags = 0;
                ip.ki.time = 0;
                ip.ki.dwExtraInfo = 0;
                SendInput(1, &ip, sizeof(INPUT));
            }

            break;
        }

        printf(".");
        Sleep(1000);
    }

    // cleanup
    CloseHandle(pif.hProcess);
    CloseHandle(pif.hThread);
    return 0;
}