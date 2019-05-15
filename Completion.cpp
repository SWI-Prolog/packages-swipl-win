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

#include "Completion.h"
#include "PREDICATE.h"
#include "SwiPrologEngine.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>

struct Bin : C { Bin(CCP op, T Left, T Right) : C(op, V(Left, Right)) {} };
struct Uni : C { Uni(CCP op, T arg) : C(op, arg) {} };

struct quv : Bin { quv(T Var, T Expr) : Bin("^", Var, Expr) {} };
struct mod : Bin { mod(T Mod, T Pred) : Bin(":", Mod, Pred) {} };
struct neg : Uni { neg(T pred) : Uni("\\+", pred) {} };
struct join : Bin { join(T Left, T Right) : Bin(",", Left, Right) {} };
struct arith : Bin { arith(T Pred, T Num) : Bin("/", Pred, Num) {} };

#define zero long(0)
#define one long(1)
#define _V T()

/** context sensitive completion
 *  take current line, give list of completions (both atoms and files)
 *  thanks to Jan for crafting a proper interface wrapping SWI-Prolog available facilities
 */
QString Completion::initialize(int promptPosition, QTextCursor c, QStringList &strings) {

    SwiPrologEngine::in_thread _int;
    QString rets;

    try {
        int p = c.position();
        Q_ASSERT(p >= promptPosition);

        c.setPosition(promptPosition, c.KeepAnchor);
        QString left = c.selectedText();
        PlString Before(left.toStdWString().data());

        c.setPosition(p);
        c.movePosition(c.EndOfLine, c.KeepAnchor);
        QString after = c.selectedText();
        PlString After(after.toStdWString().data());

        PlTerm Completions, Delete, word;
        if (PlCall("prolog", "complete_input", PlTermv(Before, After, Delete, Completions))) {
            PlTail l(Completions); // cautiously make an explicit call to close
            while (l.next(word))
                strings.append(t2w(word));
            l.close();
        }

        c.setPosition(p);
        rets = t2w(Delete);
    }
    catch(PlException e) {
        qDebug() << CCP(e);
    }
    catch(...) {
        qDebug() << "SIGV";
    }

    return rets;
}

/** issue a query filling the model storage
 *  this will change when I will learn how to call SWI-Prolog completion interface
 */
void Completion::initialize(QStringList &strings) {

    SwiPrologEngine::in_thread _int;
    try {
        PlTerm p,m,a,l,v;
        PlQuery q("setof",
            PlTermv(p,
                quv(m,
                    quv(a,
                        join(PlCompound("current_predicate", mod(m, arith(p, a))),
                            neg(C("sub_atom", PlTermv(p, zero, one, _V, A("$"))))
                ))),
            l));
        if (q.next_solution())
            for (PlTail x(l); x.next(v); )
                strings.append(CCP(v));
    }
    catch(PlException e) {
        qDebug() << CCP(e);
    }
}

Completion::status Completion::setup_status = Completion::untried;
Completion::t_pred_docs Completion::pred_docs;

/** initialize and cache all predicates with description
 */
bool Completion::setup() {
    if (setup_status == untried) {
        setup_status = missing;
        SwiPrologEngine::in_thread _e;
        try {
            if ( PlCall("load_files(library(console_input), [silent(true)])") &&
		 PlCall("current_predicate(prolog:complete_input/4)")) {
	        setup_status = available;
	    }
        }
        catch(PlException e) {
//            qDebug() << CCP(e);
        }
    }

    return setup_status == available;
}

/** access/compute predicate description tip from cached
 * FIXME: Base tooltip on a callback
 */
QString Completion::pred_tip(QTextCursor c) {
#if 0
    if (helpidx_status == available) {
        c.select(c.WordUnderCursor);
        QString w =  c.selectedText();
        auto p = pred_docs.constFind(w);
        if (p != pred_docs.end()) {
            QStringList l;
            foreach(auto x, p.value())
                l.append(QString("%1/%2:%3").arg(w).arg(x.first).arg(x.second));
            return l.join("\n");
        }
    }
#endif
    return "";
}
