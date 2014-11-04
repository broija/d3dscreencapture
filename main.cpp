/*!
    Copyright 2014 Broija

    This file is part of d3dscreencapture library.

    d3dscreencapture is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    d3dscreencapture is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with d3dscreencapture library.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <QApplication>
#include <QMainWindow>

#include "d3dcapture.h"

const char * APP_NAME = "d3dcapture_test";

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);
    app.setApplicationName(APP_NAME);

    QMainWindow mainWindow;//Simply used to get a winId

    D3dScreenCapture::CaptureTester tester;

    D3dScreenCapture::Capturer capture;
    if (capture.init((HWND)mainWindow.winId())) capture.prepareOffscreenSurface();

    QObject::connect(&capture,SIGNAL(newScreenshot(const QByteArray&)),&tester,SLOT(saveScreenshot(const QByteArray&)));

    capture.screenshoot();

    return app.exec();
}//main
