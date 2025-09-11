#include <windows.h>
#include <commdlg.h>
#include <iostream>

int main() {
    // Make the process DPI aware
    SetProcessDPIAware();

    OPENFILENAME ofn;
    char szFile[260];
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "Audio Files\0*.MP3;*.WAV;*.FLAC\0All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn) == TRUE) {
        std::cout << "Selected file: " << ofn.lpstrFile << std::endl;
    } else {
        std::cout << "No file selected." << std::endl;
    }

    std::cout << "\nPress Enter to exit...";
    std::cin.get();
    return 0;
}