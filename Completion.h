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
