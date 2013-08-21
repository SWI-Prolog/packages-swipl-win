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

#ifndef PREDICATE_H
#define PREDICATE_H

#include <SWI-cpp.h>

#ifndef NO_SHORTEN_INTERFACE

/** shorten interface */

typedef const char* CCP;
typedef const wchar_t* WCP;
typedef const void* CVP;
#define CT QThread::currentThread()

#include <QString>

inline CCP S(const PlTerm &T) { return T; }

inline PlAtom W(const QString &s) {
    return PlAtom(s.toStdWString().data());
}
inline PlAtom A(QString s) {
    return W(s);
}

inline QString t2w(PlTerm t) {
    return QString::fromWCharArray(WCP(t));
}

inline QString serialize(PlTerm t) {
    wchar_t *s;

    if ( PL_get_wchars(t, NULL, &s, CVT_WRITEQ|BUF_RING) )
      return QString::fromWCharArray(s);

    throw PlTypeError("text", t);
    PL_THROWN(NULL);
}

typedef PlTerm T;
typedef PlTermv V;
typedef PlCompound C;
typedef PlTail L;

/** get back an object passed by pointer to Prolog */
template<typename Obj> Obj* pq_cast(T ptr) { return static_cast<Obj*>(static_cast<void*>(ptr)); }

#endif

#endif // PREDICATE_H
