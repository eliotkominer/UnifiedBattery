#include <iostream>
#include <string>
#include <windows.h>
#include <powrprof.h> // for CallNtPowerInformation
#include <setupapi.h> // for Bluetooth Devices
#include <devpkey.h> // for Bluetooth Devices
#include <devguid.h> // for Bluetooth Devices
#pragma comment(lib, "SetupAPI.lib") // for Bluetooth Devices
using namespace std;

// A simple script to print out all the possible battery percentage
// sources on Windows (laptop battery, bluetooth devices, etc.)

// placeholder function i found online, i do not trust this to work at all
void PrintBluetoothBatteries() {
    // GUID_DEVCLASS_BLUETOOTH lists paired Bluetooth devices
    HDEVINFO devInfo = SetupDiGetClassDevs(
        &GUID_DEVCLASS_BLUETOOTH, nullptr, nullptr, DIGCF_PRESENT);

    if (devInfo == INVALID_HANDLE_VALUE) return;

    SP_DEVINFO_DATA devData{};
    devData.cbSize = sizeof(SP_DEVINFO_DATA);

    for (DWORD i = 0; SetupDiEnumDeviceInfo(devInfo, i, &devData); i++) {
        // Get friendly name
        WCHAR name[256]{};
        SetupDiGetDeviceRegistryPropertyW(
            devInfo, &devData, SPDRP_FRIENDLYNAME, nullptr,
            (PBYTE)name, sizeof(name), nullptr);

        // Get battery level property
        DEVPROPTYPE propType;
        BYTE battery = 0;
        BOOL ok = SetupDiGetDevicePropertyW(
            devInfo, &devData, &DEVPKEY_Bluetooth_Battery,
            &propType, &battery, sizeof(battery), nullptr, 0);

        if (ok) {
            std::wcout << name << L": " << (int)battery << L"%\n";
        }
    }

    SetupDiDestroyDeviceInfoList(devInfo);
}

int main() {
    SYSTEM_POWER_STATUS status;
    cout << "====Device Battery====" << endl;

    // GetSystemPowerStatus (rounded number)
    cout << "Rounded Value: ";
    if (status.BatteryLifePercent == 255) {
        cout << "No Battery Detected (or unknown status)";
    } else {
        cout << (int)status.BatteryLifePercent << "% ";
        if (status.BatteryFlag & 8) cout << "(Charging) ";
        if (status.ACLineStatus == 1) cout << "(AC Power)";
    }

    //CallNtPowerInformation (precise number)
    cout << endl << "Precise Value: ";
    SYSTEM_BATTERY_STATE state{};
    if (CallNtPowerInformation(SystemBatteryState, nullptr, 0,
        &state, sizeof(state)) == 0) {
        if (state.BatteryPresent) {
            double pct = 100.0 * state.RemainingCapacity / state.MaxCapacity;
            cout << "Battery: " << pct << "%";
        }
    }
    cout << endl << "====Bluetooth Devices====" << endl;

}