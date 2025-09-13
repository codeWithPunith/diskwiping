#include <iostream>
#include <shellapi.h>
#include <windows.h>

int main(){
    ShellExecute(NULL,"open","systemreset.exe","-factoryreset -cleanpc",NULL,SW_SHOWNORMAL);

    return 0;
}