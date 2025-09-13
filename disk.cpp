// factory_reset_launcher_simple.cpp
// SAFE Factory Reset Assistant (Windows) — simplified.
// - Does NOT perform destructive reset itself.
// - Detects user profile automatically, asks for a single "RESET" confirmation,
//   opens Windows Recovery settings (ms-settings:recovery), and writes a small report.

#include <iostream>
#include <string>
#include <ctime>
#include <fstream>

#ifdef _WIN32
#include <windows.h>
#include <shlobj.h> // SHGetFolderPathA
#include <shellapi.h>
#else
#error "This program is Windows-only."
#endif

static std::string now_timestamp() {
    std::time_t t = std::time(nullptr);
    char buf[64];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d_%H-%M-%S", std::localtime(&t));
    return std::string(buf);
}

static std::string get_user_profile_path() {
    char path[MAX_PATH] = {0};
    if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_PROFILE, NULL, 0, path))) {
        return std::string(path);
    }
    const char* up = std::getenv("USERPROFILE");
    if (up) return std::string(up);
    return std::string();
}

static bool open_windows_recovery_settings() {
    // Open ms-settings:recovery (works on Windows 10/11)
    const wchar_t *uri = L"ms-settings:recovery";
    HINSTANCE res = ShellExecuteW(NULL, L"open", uri, NULL, NULL, SW_SHOWNORMAL);
    return (reinterpret_cast<intptr_t>(res) > 32);
}

static void write_report(const std::string &profile, const std::string &mode_note) {
    std::string ts = now_timestamp();
    std::string fname = profile + "\\factory_reset_launcher_report_" + ts + ".txt";
    std::ofstream out(fname.c_str());
    if (!out) return;
    out << "Factory Reset Launcher Report\n";
    out << "Timestamp: " << ts << "\n";
    out << "User profile: " << profile << "\n";
    out << "Action: Opened Windows Recovery settings (ms-settings:recovery).\n";
    out << "Note: The actual Reset must be executed via the Windows UI.\n";
    out << "Mode note: " << mode_note << "\n";
    out.close();
    std::cout << "Report saved to: " << fname << "\n";
}

int main() {
    std::cout << "Factory Reset Assistant (Windows) — Safe Launcher (Simplified)\n\n";
    std::cout << "This tool will NOT automatically wipe or format any disks.\n";
    std::cout << "It helps you prepare and then opens Windows' official Reset UI so you can run the reset safely.\n\n";

    std::string profile = get_user_profile_path();
    if (profile.empty()) {
        std::cerr << "Unable to detect user profile path. Exiting.\n";
        return 1;
    }

    std::cout << "Detected user profile: " << profile << "\n\n";
    std::cout << "Recommended pre-reset steps (please ensure before proceeding):\n";
    std::cout << "  - BACKUP important files (Documents, Desktop, Downloads, Pictures)\n";
    std::cout << "  - SIGN OUT of cloud accounts (Microsoft, Google, Apple)\n";
    std::cout << "  - DISABLE or SAVE BitLocker recovery key if encryption is enabled\n";
    std::cout << "  - REMOVE external drives / SD cards\n\n";

    std::cout << "Type RESET (all caps) to open Windows Recovery settings and continue, or anything else to cancel:\n> ";
    std::string confirm;
    std::getline(std::cin, confirm);
    if (confirm != "RESET") {
        std::cout << "Cancelled by user. Exiting.\n";
        return 0;
    }

    std::cout << "\nOpening Windows Recovery settings...\n";
    bool ok = open_windows_recovery_settings();
    if (!ok) {
        std::cerr << "Failed to open Recovery settings. Please open Settings -> Update & Security -> Recovery manually.\n";
        write_report(profile, "Failed to open ms-settings:recovery (fallback)");
        return 1;
    }

    std::cout << "Windows Recovery settings opened. Please follow the on-screen flow to perform 'Reset this PC'.\n";
    write_report(profile, "Opened ms-settings:recovery (user must complete Reset via UI).");

    std::cout << "\nDemo finished. Reminder:\n";
    std::cout << " - Choose 'Remove everything' and optionally 'Clean the drive' in the Reset UI for stronger erasure.\n";
    std::cout << " - For the strongest guarantee, perform a full OS reinstall or use professional disk-erase tools.\n";

    return 0;
}
