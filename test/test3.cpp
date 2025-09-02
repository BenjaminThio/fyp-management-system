#include <windows.h>
#include <iostream>

int main() {
    HKEY hKey;
    // Adjust for your system: System32 vs SysWOW64
    LPCWSTR subkey = L"Console\\C:_Windows_System32_cmd.exe";

    LONG status = RegCreateKeyExW(
        HKEY_CURRENT_USER,
        subkey,
        0, NULL, 0, KEY_SET_VALUE, NULL, &hKey, NULL);

    if (status == ERROR_SUCCESS) {
        DWORD value = 0; // 0 = disable wrapping
        status = RegSetValueExW(hKey,
                                L"WrapTextOutput",
                                0,
                                REG_DWORD,
                                reinterpret_cast<const BYTE*>(&value),
                                sizeof(value));

        if (status == ERROR_SUCCESS) {
            std::wcout << L"✅ WrapTextOutput set to 0. Restart CMD to apply.\n";
        } else {
            std::wcerr << L"❌ Failed to set value. Error code: " << status << "\n";
        }
        RegCloseKey(hKey);
    } else {
        std::wcerr << L"❌ Failed to open/create registry key. Error code: " << status << "\n";
    }

    std::cout << "Benjaminnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn";
    getchar();
    return 0;
}