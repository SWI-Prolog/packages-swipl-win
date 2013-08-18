/*
    pqConsole    : interfacing SWI-Prolog and Qt

    Author       : Carlo Capelli
    E-mail       : cc.carlo.cap@gmail.com
    Copyright (C): 2013, Carlo Capelli

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef PQTERM_H
#define PQTERM_H

#include "pqConsole_global.h"
#include <SWI-cpp.h>
#include <QVariant>

#define X PQCONSOLESHARED_EXPORT

/** since SWI-Prolog doesn't allow inter thread terms exchange,
 *  this class could be required to truly distribute execution
 *  but after sketching it, I've not more used, or completed...
 */

QVariant term2variant(PlTerm t);
PlTerm variant2term(const QVariant &v);

typedef QPair< QString, QVector<QVariant> > pqStruct;
typedef QVariantList pqList;

#endif // PQTERM_H
