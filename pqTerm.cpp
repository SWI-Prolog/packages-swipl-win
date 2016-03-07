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
