@rem 只复制源时间比目标时间新的文件
xcopy /d /y   ..\..\..\lib\cuda\*.dll
xcopy /d /y   ..\..\..\lib\onnxruntime\*.dll
xcopy /d /y   ..\..\..\lib\sodium\*.dll
xcopy /d /y   ..\..\..\lib\opencv\release\*.dll
xcopy /d /y   ..\..\..\lib\opencv\debug\*.dll
xcopy /d /y   ..\..\..\lib\camera\debug\*.dll
xcopy /d /y   ..\..\..\lib\camera\basler\*.dll
xcopy /d /y   ..\..\..\lib\camera\mvs\*.dll


 set PATH = %PATH%;C:\Qt\Qt5.14.1\5.14.1\msvc2017_64\bin
 C:\Qt\Qt5.14.1\5.14.1\msvc2017_64\bin\windeployqt.exe  SCC_PCB_CaptureSoftware.exe
