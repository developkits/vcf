# Microsoft Developer Studio Project File - Name="RemoteObjectKit" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=RemoteObjectKit - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "RemoteObjectKit.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "RemoteObjectKit.mak" CFG="RemoteObjectKit - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RemoteObjectKit - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "RemoteObjectKit - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RemoteObjectKit - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../../lib/"
# PROP Intermediate_Dir "vc70/Release/"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O1 /I "$(VCF_INCLUDE)" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "REMOTEKIT_DLL" /D "REMOTEKIT_EXPORTS" /D "USE_NETKIT_DLL" /D "USE_FRAMEWORK_DLL" /D "BUILD_REMOTEOBJECTKIT_LIB" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"../../../bin/RemoteObjectKit_vc70.bsc"
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 /nologo /dll /machine:I386 /out:"../../../bin/RemoteObjectKit_vc70.dll" /libpath:"../../../lib"
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "RemoteObjectKit - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../lib/"
# PROP Intermediate_Dir "vc70/Debug/"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "$(VCF_INCLUDE)" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "REMOTEKIT_DLL" /D "REMOTEKIT_EXPORTS" /D "USE_NETKIT_DLL" /D "USE_FRAMEWORK_DLL" /D "BUILD_REMOTEOBJECTKIT_LIB" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"../../../bin/RemoteObjectKit_vc70_d.bsc"
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /dll /debug /machine:I386 /out:"../../../bin/RemoteObjectKit_vc70_d.dll" /pdbtype:sept /libpath:"../../../lib"
# SUBTRACT LINK32

!ENDIF 

# Begin Target

# Name "RemoteObjectKit - Win32 Release"
# Name "RemoteObjectKit - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=../../../src/vcf/RemoteObjectKit/AbstractDistributedApplication.cpp
# End Source File
# Begin Source File

SOURCE=../../../src/vcf/RemoteObjectKit/ClassRegistryEntry.cpp
# End Source File
# Begin Source File

SOURCE=../../../src/vcf/RemoteObjectKit/ClassServerInstance.cpp
# End Source File
# Begin Source File

SOURCE=../../../src/vcf/RemoteObjectKit/DistributedClassRegistry.cpp
# End Source File
# Begin Source File

SOURCE=../../../src/vcf/RemoteObjectKit/DistributedException.cpp
# End Source File
# Begin Source File

SOURCE=../../../src/vcf/RemoteObjectKit/InstanceID.cpp
# End Source File
# Begin Source File

SOURCE=../../../src/vcf/RemoteObjectKit/Proxy.cpp
# End Source File
# Begin Source File

SOURCE=../../../src/vcf/RemoteObjectKit/RemoteObjectKit.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=../../../src/vcf/RemoteObjectKit/AbstractDistributedApplication.h
# End Source File
# Begin Source File

SOURCE=../../../src/vcf/RemoteObjectKit/ClassRegistryEntry.h
# End Source File
# Begin Source File

SOURCE=../../../src/vcf/RemoteObjectKit/ClassServerInstance.h
# End Source File
# Begin Source File

SOURCE=../../../src/vcf/RemoteObjectKit/DistributedClassRegistry.h
# End Source File
# Begin Source File

SOURCE=../../../src/vcf/RemoteObjectKit/DistributedException.h
# End Source File
# Begin Source File

SOURCE=../../../src/vcf/RemoteObjectKit/InstanceID.h
# End Source File
# Begin Source File

SOURCE=../../../src/vcf/RemoteObjectKit/Proxy.h
# End Source File
# Begin Source File

SOURCE=../../../src/vcf/RemoteObjectKit/RemoteCommon.h
# End Source File
# Begin Source File

SOURCE=../../../src/vcf/RemoteObjectKit/RemoteConfig.h
# End Source File
# Begin Source File

SOURCE=../../../src/vcf/RemoteObjectKit/RemoteObjectKit.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
