#include "autologin.h"

void chomp(char *s)
{
    while(*s && *s != '\n' && *s != '\r') s++;
 
    *s = 0;
}

void pressKey(SHORT key)
{
    ip.type = INPUT_KEYBOARD;
    ip.ki.wVk = key;
    ip.ki.wScan = 0;
    ip.ki.dwFlags = 0;
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;
    SendInput(1, &ip, sizeof(INPUT));
}

config_t get_config(char *filename) 
{
        config_t configstruct;
        FILE *file = fopen (filename, "r");

        if (file != NULL) {
            char line[MAXBUF];
            int i = 0;

            while(fgets(line, sizeof(line), file) != NULL) {
                char *cfline;
                // YOU SHALL NOT PASS, NEWLINE
                chomp(cfline);
                cfline = strstr((char *)line,DELIM);
                cfline = cfline + strlen(DELIM);

                if (i == 0) {
                    memcpy(configstruct.userName,cfline,strlen(cfline));
                } else if (i == 1) {
                    memcpy(configstruct.password,cfline,strlen(cfline));
                } else if (i == 2) {
                    memcpy(configstruct.path, cfline, strlen(cfline));
                }
                
                i++;
        }
        fclose(file);
    }
    
    return configstruct;
}

int main(int argc, char* argv[])
{

    // we use the window title to make sure the game is open
    char *windowTitle = "Overwatch";
    config_t configstruct;
    configstruct = get_config(CONFIG_PATH);

    // get the current keyboard layout
    HKL currentKBL = GetKeyboardLayout(0);

    PROCESS_INFORMATION pif;
    STARTUPINFO si;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);

    // open the process
    BOOL bRet = CreateProcess(
        configstruct.path,
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
        fprintf(stderr, "Failed to open %s\n", configstruct.path);
        return 1;
    }

    unsigned int i = 0;
    unsigned int userLen = strlen(configstruct.userName);
    unsigned int passLen = strlen(configstruct.password);
    SHORT kc;

    printf("Running ");
    while(1) {
        HWND window = FindWindow(NULL, windowTitle);
        if (window != NULL) {
            printf(" OK\n");

            // FIXME: this is a gross hack. Instead of making sure it's finally loaded
            // we're just waiting 8 seconds. I actually hate this.
            Sleep(8000);

            // send Overwatch to the foreground if it's not already
            SetForegroundWindow(window);

            // enter the email
            for (i = 0; i < userLen; i++) {
                kc = VkKeyScanEx(configstruct.userName[i], currentKBL);
                // if it's an @ symbol we need to do some weird shit
                if (configstruct.userName[i] == '@') {
                    // press the "Shift" key
                    ip.ki.wVk = VK_SHIFT;
                    ip.ki.dwFlags = 0; // 0 for key press
                    SendInput(1, &ip, sizeof(INPUT));
             
                    // press the single quote key
                    ip.ki.wVk = kc;
                    ip.ki.dwFlags = 0; // 0 for key press
                    SendInput(1, &ip, sizeof(INPUT));
             
                    // release the single quote key
                    ip.ki.wVk = kc;
                    ip.ki.dwFlags = KEYEVENTF_KEYUP;
                    SendInput(1, &ip, sizeof(INPUT));
             
                    // release the "Shift" key
                    ip.ki.wVk = VK_SHIFT;
                    ip.ki.dwFlags = KEYEVENTF_KEYUP;
                    SendInput(1, &ip, sizeof(INPUT));
                } else {
                    pressKey(kc);
                }
                Sleep(100);
            }

            // tab then enter password
            pressKey(VK_TAB);
            
            for (i = 0; i < passLen; i++) {
                kc = VkKeyScanEx(configstruct.password[i], currentKBL);
                pressKey(kc);
                Sleep(100);
            }

            // finally, hit enter to submit
            pressKey(VK_RETURN);
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