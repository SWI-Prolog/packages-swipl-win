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

#include "pqTerm.h"
#include "PREDICATE.h"

QVariant term2variant(PlTerm t) {
    switch (t.type()) {
    case PL_VARIABLE:
        return QVariant();
    case PL_INT:
    case PL_INTEGER:
        return QVariant(int(t));
    case PL_FLOAT:
        return double(t);

    case PL_ATOM:
    case PL_STRING:
        return t2w(t);

    default:
        throw QObject::tr("term2variant: unknown type %1").arg(t.type());
    }
}

PlTerm variant2term(const QVariant& v) {
    switch (v.type()) {
    case v.Char:
    case v.String:
    case v.ByteArray:
        return PlTerm(v.toString().toStdWString().data());
    case v.Int:
        return PlTerm(long(v.toInt()));
    case v.Double:
        return PlTerm(v.toDouble());
    case v.List: {
        PlTerm t;
        PlTail l(t);
        foreach(QVariant e, v.toList())
            l.append(variant2term(e));
        l.close();
        return t;
    }
    default:
        return PlTerm();
    }
}
