#include <stdio.h>
#include <windows.h>

int printKeyName(int key_code)
{
    char key_name[1024];
    UINT scanCode = MapVirtualKeyW(key_code, MAPVK_VK_TO_VSC);
    LONG lParamValue = (scanCode << 16);

    int rc = GetKeyNameText(lParamValue, key_name, 1024);
    if (rc) printf("%s\n", key_name);
    return rc;
}