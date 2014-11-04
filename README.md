d3dscreencapture
================

A simple screen capture library based on DirectX 9.

Tested with:
 - Windows 7 Home Premium.
 - DXSDK_Jun10 : http://www.microsoft.com/en-us/download/details.aspx?id=6812
 - Qt 5.1
 
Instructions
------------

 - Call **init** with a valid window handle.
 - If **init** returns *true*, call **prepareOffscreenSurface**.
 - Call **screenshoot**
 
Please take a look at **main.cpp** for a basic example.
