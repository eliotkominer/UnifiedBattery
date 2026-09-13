#define INITGUID

#include <iostream>
#include <windows.h> // windows battery information
// for bluetooth device battery below
#include <setupapi.h>
#include <devguid.h>
#include <devpkey.h>
#include <cfgmgr32.h>
using namespace std;

#pragma comment(lib, "Setupapi.lib")
#pragma comment(lib, "Cfgmgr32.lib")

int wmain() {
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

    cout << "===========" << endl;

    // bluetooth devices
    HDEVINFO hDevInfo = SetupDiGetClassDevsW(&GUID_DEVCLASS_BLUETOOTH, nullptr, nullptr, DIGCF_PRESENT);
    if (hDevInfo == INVALID_HANDLE_VALUE)
    {
        std::wcerr << L"Failed to get Bluetooth device list. Error: " << GetLastError() << std::endl;
        return 1;
    }
 
    SP_DEVINFO_DATA data{};
    data.cbSize = sizeof(data);
 
    bool foundAny = false;
 
    for (DWORD i = 0; SetupDiEnumDeviceInfo(hDevInfo, i, &data); i++)
    {
        ULONG status = 0, problemNumber = 0;
        CONFIGRET cr = CM_Get_DevNode_Status(&status, &problemNumber, data.DevInst, 0);
 
        bool isConnected = (cr == CR_SUCCESS)
                         && (status & DN_STARTED)
                         && !(status & DN_HAS_PROBLEM);
 
        if (!isConnected)
            continue;
 
        WCHAR name[512] = L"";
        DEVPROPTYPE propType;
        if (SetupDiGetDevicePropertyW(hDevInfo, &data, &DEVPKEY_Device_FriendlyName, &propType,
            reinterpret_cast<PBYTE>(name), sizeof(name), nullptr, 0))
        {
            std::wcout << name << std::endl;
            foundAny = true;
        }
    }
 
    if (!foundAny)
    {
        std::wcout << L"No Bluetooth devices are currently connected." << std::endl;
    }
 
    SetupDiDestroyDeviceInfoList(hDevInfo);
}