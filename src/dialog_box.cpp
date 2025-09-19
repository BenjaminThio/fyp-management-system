#include <windows.h>
#include <commdlg.h>
#include <iostream>
#include <string>
#include <filesystem>
using namespace std;

namespace fs = filesystem;

fs::path dialog_box() {
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
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetOpenFileName(&ofn) == TRUE)
        return fs::path(ofn.lpstrFile);
    else
        return fs::path();
        // throw runtime_error("No file selected.");
}

namespace dialog {
    void warning_message(string message) {
        MessageBox(
            NULL,
            message.c_str(),
            "Warning",
            MB_ICONWARNING | MB_OK
        );
    }

    void error_message(string message) {
        MessageBox(
            NULL,
            message.c_str(),
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