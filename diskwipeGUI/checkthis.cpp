#include <windows.h>
#include <fstream>
#include <string>

void WriteUnattendFile(bool keepFiles, bool wipeDrive) {
    std::string xml =
        "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
        "<unattend xmlns=\"urn:schemas-microsoft-com:unattend\">\n"
        "  <settings pass=\"oobeSystem\">\n"
        "    <component name=\"Microsoft-Windows-Reset\" "
        "processorArchitecture=\"amd64\" publicKeyToken=\"31bf3856ad364e35\" "
        "language=\"neutral\" versionScope=\"nonSxS\" "
        "xmlns:wcm=\"http://schemas.microsoft.com/WMIConfig/2002/State\">\n";

    xml += "      <ResetType>";
    xml += keepFiles ? "KeepUserData" : "RemoveAll";
    xml += "</ResetType>\n";

    xml += "      <WipeData>";
    xml += wipeDrive ? "true" : "false";
    xml += "</WipeData>\n";

    xml +=
        "    </component>\n"
        "  </settings>\n"
        "</unattend>\n";

    // Save file in Sysprep folder (admin rights required)
    std::ofstream file("C:\\Windows\\System32\\Sysprep\\Unattend.xml");
    file << xml;
    file.close();
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmd, int nShow) {
    int choice = MessageBox(
        NULL,
        "Choose reset option:\n\nYES = Remove Everything\nNO = Keep My Files",
        "Custom Reset", MB_YESNO | MB_ICONQUESTION);

    bool keepFiles = (choice == IDNO);

    int wipeChoice = MessageBox(
        NULL,
        "Do you want to fully clean the drive?\n\nYES = Full Wipe\nNO = Quick Remove",
        "Drive Wipe", MB_YESNO | MB_ICONQUESTION);

    bool wipeDrive = (wipeChoice == IDYES);

    WriteUnattendFile(keepFiles, wipeDrive);

    MessageBox(NULL, "System will now reboot into recovery and reset automatically.",
               "Reset Scheduled", MB_OK | MB_ICONWARNING);

    // Trigger reboot into WinRE
    system("shutdown /r /o /f /t 0");

    return 0;
}
