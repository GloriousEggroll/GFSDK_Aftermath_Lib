This is a proxy DLL for GFSDK_Aftermath_Lib.x64.dll

Original author and source:

https://github.com/ValveSoftware/Proton/issues/3042#issuecomment-1046904681

Details:

Steps:

    Rename EasyAntiCheat folder to anything in %gamefolder%/GearGame/Binaries/Steam
    Rename Gears5.exe to Gears5_EAC.exe (original Gears5_EAC.exe rename to anything)
    Rename GFSDK_Aftermath_Lib.x64.dll in %gamefolder%/Engine/Binaries/ThirdParty/GFSDK_Aftermath/x64/ to gfsdk.dll
    Download GFSDK_Aftermath_Lib.x64.tar.gz, and extract GFSDK_Aftermath_Lib.x64.dll to the %gamefolder%/Engine/Binaries/ThirdParty/GFSDK_Aftermath/x64/

If you wish to compile the dll yourself:

x86_64-w64-mingw32-gcc -shared -o GFSDK_Aftermath_Lib.x64.dll gfsdk.c GFSDK_Aftermath_Lib.x64.def -s

Of course EAC doesn't work and multiplayer does not work. This workaround is only useful if you want to play the campaign.

