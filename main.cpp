#include <iostream>
#include <windows.h>
using namespace std;

int main() {
    SYSTEM_POWER_STATUS status;
    if (GetSystemPowerStatus(&status)) {
        std::cout << "AC Line Status: " << (int)status.ACLineStatus << "\n"; // 0=offline,1=online,255=unknown
        std::cout << "Battery Flag: " << (int)status.BatteryFlag << "\n";     // bit flags: high/low/critical/charging/no battery
        std::cout << "Battery Life Percent: " << (int)status.BatteryLifePercent << "%\n"; // 0-100, 255=unknown

        if (status.BatteryLifeTime != (DWORD)-1)
            std::cout << "Battery Life Time (seconds): " << status.BatteryLifeTime << "\n";
        else
            std::cout << "Battery Life Time: unknown\n";

        if (status.BatteryFullLifeTime != (DWORD)-1)
            std::cout << "Battery Full Life Time (seconds): " << status.BatteryFullLifeTime << "\n";
    } else {
        std::cerr << "GetSystemPowerStatus failed: " << GetLastError() << "\n";
    }
    return 0;
}