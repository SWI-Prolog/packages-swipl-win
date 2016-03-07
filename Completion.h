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

#ifndef COMPLETION_H
#define COMPLETION_H

#include "pqConsole_global.h"

#include <QMap>
#include <QCompleter>
#include <QTextCursor>
#include <QAbstractItemView>

/** service class, holds a sorted list of predicates for word completion
 */
struct PQCONSOLESHARED_EXPORT Completion
{
    /** context sensitive completion */
    static QString initialize(int promptPosition, QTextCursor cursor, QStringList &strings);

    /** load predicates into strings */
    static void initialize(QStringList &strings);

    /** tooltips display, from helpidx.pl */
    enum status { untried, available, missing };
    static status helpidx_status;

    /** predicate -> declarations */
    typedef QPair<int, QString> t_decl;
    typedef QList<t_decl> t_decls;
    typedef QMap<QString, t_decls> t_pred_docs;
    static t_pred_docs pred_docs;

    /** initialize if required, return true if available */
    static bool helpidx();

    /** access/compute predicate description tip from cached */
    static QString pred_tip(QTextCursor c);
};

#endif // COMPLETION_H
