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

#ifndef D3DCAPTURE_H
#define D3DCAPTURE_H

#include <QObject>

#include <d3d9.h>
#include <d3dx9.h>

#ifdef D3DCAPTURE_APP
#include <QPixmap>
#include <QDateTime>
#endif//D3DCAPTURE_APP

#include "d3dcapture_global.h"

class QBuffer;

namespace D3dScreenCapture
{

class D3DCAPTURESHARED_EXPORT Capturer : public QObject
{
    Q_OBJECT
public:
    explicit Capturer(QObject *parent = 0);
    ~Capturer();

    bool init(HWND _winId);

    bool prepareOffscreenSurface();
    
public slots:
    bool screenshoot();///< Take a screenshot.

signals:
    void newScreenshot(const QByteArray & _data);///< Successful screenshot notification.

protected:
    bool parseReturnValue(const char * _name, HRESULT _hResult);

    IDirect3D9 * m_pD3D9;
    LPDIRECT3DDEVICE9 m_pDev;
    IDirect3DSurface9 * m_pD3DSurface9;
    HWND m_windowHandle;

    QByteArray m_byteArray;
};//Capturer

//-----------------------------

#ifdef D3DCAPTURE_APP
class CaptureTester : public QObject
{
    Q_OBJECT
public:
    CaptureTester(){}

public slots:
    void saveScreenshot(const QByteArray & _data)
    {
        QPixmap pixmap;
        pixmap.loadFromData(_data);

        if (!pixmap.isNull())
        {
            pixmap.save(QString("screenshot_%1.png").arg(QDateTime::currentMSecsSinceEpoch()));
        }//if (!pixmap.isNull())
    }//saveScreenshot
};//CaptureTester

//---------------
#endif//D3DCAPTURE_APP

}//namespace D3dScreenCapture

#endif // D3DCAPTURE_H
