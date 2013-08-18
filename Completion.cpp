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
        if (PlCall("prolog", "complete_input", PlTermv(Before, After, Delete, Completions)))
            for (PlTail l(Completions); l.next(word); )
                strings.append(t2w(word));

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

Completion::status Completion::helpidx_status = Completion::untried;
Completion::t_pred_docs Completion::pred_docs;

/** initialize and cache all predicates with description
 */
bool Completion::helpidx() {
    if (helpidx_status == untried) {
        helpidx_status = missing;
        SwiPrologEngine::in_thread _e;
        try {
            if (    PlCall("load_files(library(helpidx), [silent(true)])") &&
                    PlCall("current_module(help_index)"))
            {
                {   PlTerm Name, Arity, Descr, Start, Stop;
                    PlQuery q("help_index", "predicate", V(Name, Arity, Descr, Start, Stop));
                    while (q.next_solution()) {
                        long arity = Arity.type() == PL_INTEGER ? long(Arity) : -1;
                        QString name = t2w(Name);
                        t_pred_docs::iterator x = pred_docs.find(name);
                        if (x == pred_docs.end())
                            x = pred_docs.insert(name, t_decls());
                        x.value().append(qMakePair(int(arity), t2w(Descr)));
                    }
                }

                if (PlCall("load_files(library(console_input), [silent(true)])"))
                    if (PlCall("current_module(prolog_console_input)"))
                        helpidx_status = available;
            }

            /*
            if (!PlCall("current_module(prolog_console_input)")) {
                QString ci = "console_input.pl";
                QFile f(QString(":/%1").arg(ci));
                if (f.open(f.ReadOnly)) {
                    QTextStream s(&f);
                    if (!_e.named_load(ci, s.readAll()))
                        qDebug() << "can't load" << ci;
                }
            }
            */
        }
        catch(PlException e) {
            qDebug() << CCP(e);
        }
    }

    return helpidx_status == available && !pred_docs.isEmpty();
}

/** access/compute predicate description tip from cached
 */
QString Completion::pred_tip(QTextCursor c) {
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
    return "";
}
