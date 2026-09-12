#include <iostream>
#include <windows.h> // windows battery information
#include <bluetoothapis.h> // windows bluetooth information
using namespace std;

int main() {
    // battery data stuff
    SYSTEM_POWER_STATUS status;
    if (GetSystemPowerStatus(&status)) {
        cout << "AC Line Status: " << (int)status.ACLineStatus << "\n"; // 0=offline,1=online,255=unknown
        cout << "Battery Flag: " << (int)status.BatteryFlag << "\n";     // bit flags: high/low/critical/charging/no battery
        cout << "Battery Life Percent: " << (int)status.BatteryLifePercent << "%\n"; // 0-100, 255=unknown

        if (status.BatteryLifeTime != (DWORD)-1)
            cout << "Battery Life Time (seconds): " << status.BatteryLifeTime << "\n";
        else
            cout << "Battery Life Time: unknown\n";

        if (status.BatteryFullLifeTime != (DWORD)-1)
            cout << "Battery Full Life Time (seconds): " << status.BatteryFullLifeTime << "\n";
    } else {
        cerr << "GetSystemPowerStatus failed: " << GetLastError() << "\n";
    }

    // bluetooth devices
    _BLUETOOTH_DEVICE_INFO bluetooth;
    cout << "Bluetooth Devices:" << bluetooth.Address << endl;
}