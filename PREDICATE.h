/*  Part of SWI-Prolog interface to Qt

    Author:        Carlo Capelli
    E-mail:        cc.carlo.cap@gmail.com
    Copyright (c)  2013, Carlo Capelli
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:

    1. Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in
       the documentation and/or other materials provided with the
       distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
    FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
    COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
    INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
    BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
    LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
    ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    POSSIBILITY OF SUCH DAMAGE.
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
