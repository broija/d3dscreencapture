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

#include "d3dcapture.h"

#include <QImage>
#include <QBuffer>

#include "deepDebug.h"

namespace
{
//    const char * IMAGE_FORMAT = "JPG";///< \warning qjpeg.dll must be in a "imageformats" sub-directory
    const char * IMAGE_FORMAT = "PNG";
    const int IMAGE_QUALITY = 100;
}//namespace

namespace D3dScreenCapture
{

Capturer::Capturer(QObject *parent) :
    QObject(parent),
    m_pDev(NULL),
    m_pD3DSurface9(NULL)
{
}//Capturer

//------------------------------

Capturer::~Capturer()
{
    if (m_pD3D9) m_pD3D9->Release();
    if (m_pDev) m_pDev->Release();
    if (m_pD3DSurface9) m_pD3DSurface9->Release();
}//~Capturer

//------------------------------

bool Capturer::init(HWND _winId)
{
    bool result = false;

    m_pD3D9 = Direct3DCreate9(D3D_SDK_VERSION);

    // Create the D3D object, which is needed to create the D3DDevice.
    if (m_pD3D9 != NULL)
    {
        m_windowHandle = _winId;

        D3DPRESENT_PARAMETERS d3dPParams;

        SecureZeroMemory(&d3dPParams, sizeof(d3dPParams));
        d3dPParams.Windowed   = TRUE;
        d3dPParams.SwapEffect = D3DSWAPEFFECT_COPY;

        result = !FAILED(m_pD3D9->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,m_windowHandle,D3DCREATE_HARDWARE_VERTEXPROCESSING ,&d3dPParams,&m_pDev));
    }//if (m_pD3D9 != NULL)

    if (result) deepDebug("Capturer::init : success!");

    return result;
}//init

//------------------------------

bool Capturer::prepareOffscreenSurface()
{
    bool result = false;

    if (m_pDev)
    {
        if (m_pD3DSurface9) m_pD3DSurface9->Release();

        D3DDISPLAYMODE displayMode;

        m_pDev->GetDisplayMode(0,&displayMode);

        HRESULT hResult;

        //qWarning("Capturer::prepareOffscreenSurface: please check that surface fits window dimensions.");
        parseReturnValue("CreateOffscreenPlainSurface",m_pDev->CreateOffscreenPlainSurface(displayMode.Width, displayMode.Height,
                                                                                           D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH, &m_pD3DSurface9, NULL));

        result = (hResult == D3D_OK);
    }//if (m_pDev)

    if (result) deepDebug("Capturer::prepareOffscreenSurface : success!");
    return result;
}//prepareOffscreenSurface

//------------------------------

bool Capturer::screenshoot()
{
    bool result = false;

    if (m_pDev && m_pD3DSurface9)
    {
//        HRESULT hResult;

        //GetFrontBufferData is very slow according to MSDN documentation.
        if (!parseReturnValue("GetFrontBufferData",m_pDev->GetFrontBufferData(0, m_pD3DSurface9)))
        {
            qWarning("Capturer::screenshoot : front buffer retrieval failure.");
        }//if (!parseReturnValue("GetFrontBufferData"...
        else
        {
            D3DSURFACE_DESC d3dSurfaceDesc;

            SecureZeroMemory(&d3dSurfaceDesc, sizeof(d3dSurfaceDesc));

            if (parseReturnValue("screenshoot: GetDesc",m_pD3DSurface9->GetDesc(&d3dSurfaceDesc)))
            {
                QImage frontBufferImage(d3dSurfaceDesc.Width , d3dSurfaceDesc.Height, QImage::Format_RGB32);

                D3DLOCKED_RECT rect;

                //Copy from surface to QImage
                if (parseReturnValue("screenshoot:LockRect",m_pD3DSurface9->LockRect(&rect, 0, 0)))
                {
                    DWORD * dst = (DWORD *) frontBufferImage.scanLine(0);
                    DWORD * src = (DWORD *) rect.pBits;

                    Q_ASSERT((rect.Pitch/4) == (frontBufferImage.bytesPerLine()/4));

                    memcpy(dst, src, rect.Pitch*frontBufferImage.height());

                    if (parseReturnValue("screenshoot:UnlockRect",m_pD3DSurface9->UnlockRect()))
                    {
                        //Conversion to PNG
                        QBuffer buffer(&m_byteArray);
                        buffer.open(QIODevice::WriteOnly);

                        if (true && frontBufferImage.save(&buffer,IMAGE_FORMAT,IMAGE_QUALITY))
                        {
                            deepDebug("Capturer::screenshoot : Image byte count %d",frontBufferImage.byteCount());
                            deepDebug("Capturer::screenshoot : Byte array size %d",m_byteArray.size());

                            result = true;
                        }//if (image.save(&buffer,IMAGE_FORMAT))

                        buffer.close();

                        if (result)
                        {
                            QByteArray imageData(m_byteArray);

                            //Reducing storage amount of bytes while not squeezing
                            //m_byteArray which size changes all the time.
                            imageData.squeeze();
                            emit newScreenshot(imageData);
                        }//if (result)
                    }//if (parseReturnValue("screenshoot:UnlockRect",m_pD3DSurface9->UnlockRect()))
                }//parseReturnValue
            }//if (parseReturnValue(m_pD3DSurface9->GetDesc(&d3dSurfaceDesc)))
        }//if (!parseReturnValue("GetFrontBufferData"...else
    }//if (m_pDev && m_pD3DSurface9)

    if (result) deepDebug("Capturer::screenshoot : success!");

    return result;
}//screenshoot

//------------------------------

bool Capturer::parseReturnValue(const char * _name, HRESULT _hResult)
{
    bool result = false;

    switch (_hResult)
    {
    case D3DERR_DRIVERINTERNALERROR:
        deepDebug("%s : driver internal error",_name);
        break;
    case D3DERR_DEVICELOST:
        deepDebug("%s : device lost.",_name);
        break;
    case D3DERR_INVALIDCALL:
        deepDebug("%s : invalid call",_name);
        break;
    case D3DERR_WASSTILLDRAWING:
        deepDebug("%s : was still drawing",_name);
        break;
    case D3D_OK:
        deepDebug("%s : success!",_name);
        result = true;
        break;
    case E_OUTOFMEMORY:
        deepDebug("%s : out of memory.",_name);
        break;
    default:
        deepDebug("%s : failure...",_name);
        break;
    }//switch (_hResult)

    return result;
}//parseReturnValue

//------------------------------

}//namespace D3dScreenCapture
