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

#ifndef D3DCAPTURE_GLOBAL_H
#define D3DCAPTURE_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef D3DCAPTURE_APP
#define D3DCAPTURESHARED_EXPORT
#else
#ifdef D3DCAPTURE_LIBRARY
#define D3DCAPTURESHARED_EXPORT Q_DECL_EXPORT
#else
#define D3DCAPTURESHARED_EXPORT Q_DECL_IMPORT
#endif
#endif//ifdef D3DCAPTURE_APP...else

#endif // D3DCAPTURE_GLOBAL_H
