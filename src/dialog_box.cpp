#include <windows.h>
#include <commdlg.h>
#include <iostream>
using namespace std;

LPSTR dialog_box() {
    SetProcessDPIAware();

    OPENFILENAME ofn;
    char szFile[260];
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn) == TRUE)
        return ofn.lpstrFile;
    else 
        throw runtime_error("No file selected.");
}

namespace dialog {
    void warning_message(const char* message) {
        MessageBox(
            NULL,
            message,
            "Warning",
            MB_ICONWARNING | MB_OK
        );
    }

    void error_message(const char* message) {
        MessageBox(
            NULL,
            message,
            "Error",
            MB_ICONERROR | MB_OK
        );
    }
}


/*
int main() {
    cout << "Press any key to exit";
    return 0;
}
*/