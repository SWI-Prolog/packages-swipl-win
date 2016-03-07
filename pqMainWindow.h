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

#ifndef PQMAINWINDOW_H
#define PQMAINWINDOW_H

#include "pqConsole_global.h"
#include <QMainWindow>
#include <QTabWidget>
#include <QCloseEvent>
#include <QSignalMapper>

// forward declaration, avoid including all SWI-Prolog interface...
class ConsoleEdit;

/** must avoid multiple connections of menu target, then */
struct cSignalMapper : QSignalMapper {
    virtual int receivers(const char *signal) const { return QSignalMapper::receivers(signal); }
};

/** make a public top level widget
 *  ready to handle proper termination of XPCE thread
 */
class PQCONSOLESHARED_EXPORT pqMainWindow : public QMainWindow
{
    Q_OBJECT

public:

    /** this is the mandatory constructor to get SWI-prolog embedding
     *  and proper XPCE termination
     */
    pqMainWindow(int argc, char *argv[]);

    /** default constructor
     */
    explicit pqMainWindow(QWidget *parent = 0);

    /** load memory script
     */
    void set_script(QString name, QString text);

    /** get access to the widget */
    ConsoleEdit *console(int thread = -1) const;

    /** get current active widget */
    ConsoleEdit *consoleActive() const;

    /** add a console: switch the interface to tabbed one */
    void addConsole(ConsoleEdit *console, QString title);

    /** remove a console (NEVER the first) */
    void remConsole(ConsoleEdit *console);

    /** bind to menu */
    void addActionPq(ConsoleEdit *ce, QMenu *cmmenu, QString label, QString action);

    /** ditto */
    QAction* add_action(ConsoleEdit *ce, QMenu *mn, QString Label, QString ctxtmod, QString Goal, QAction *before = 0);

signals:
    
public slots:

    /** handle the close button on tabbed interface */
    void tabCloseRequested(int tabId);

protected:

    /** handle application closing, WRT XPCE termination */
    virtual void closeEvent(QCloseEvent *event);

    /** when there are more than a console, use a tabbed interface */
    QTabWidget *consoles() const;

    /** route menus to prolog */
    cSignalMapper *menu2pl;
};

/** utility to lookup a typed parent in hierarchy */

template <class W>
inline W* find_parent(QWidget *p) {
    W *w = 0;
    for ( ; p; p = p->parentWidget())
        if ((w = qobject_cast<W*>(p)))
            break;
    return w;
}

#endif // PQMAINWINDOW_H
