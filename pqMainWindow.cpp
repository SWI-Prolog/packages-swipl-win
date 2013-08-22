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

#include "pqMainWindow.h"
#include "ConsoleEdit.h"
#include "Preferences.h"
#include "PREDICATE.h"
#include "do_events.h"

#include <QMenu>
#include <QDebug>
#include <QTimer>
#include <QMessageBox>
#include <QApplication>

inline ConsoleEdit *wid2con(QWidget *w) { return qobject_cast<ConsoleEdit*>(w); }

pqMainWindow::pqMainWindow(QWidget *parent) :
    QMainWindow(parent), menu2pl(0)
{
}

/** this is the mandatory constructor to get SWI-prolog embedding
 *  and proper XPCE termination
 */
pqMainWindow::pqMainWindow(int argc, char *argv[]) {

    // dispatch signals indexed
    menu2pl = new cSignalMapper;

    setCentralWidget(new ConsoleEdit(argc, argv));

    Preferences p;
    p.loadGeometry(this);
}

/** handle application closing, WRT XPCE termination
 */
void pqMainWindow::closeEvent(QCloseEvent *event) {
    auto t = consoles();
    if (t) {
        for (int c = 0; c < t->count(); ++c)
            if (!wid2con(t->widget(c))->can_close()) {
                event->ignore();
                return;
            }
    }
    else if (!wid2con(centralWidget())->can_close()) {
        event->ignore();
        return;
    }

    {   Preferences p;
        p.saveGeometry(this);
    }

    if (!SwiPrologEngine::quit_request())
        event->ignore();
}

/** pass initialization script to actual interface
 */
void pqMainWindow::set_script(QString name, QString text) {
    console()->engine()->script_run(name, text);
}

/** get access to the widget
 */
ConsoleEdit *pqMainWindow::console(int thread) const {

    if (!consoles()) {
        // don't search
        auto c = wid2con(centralWidget());
        return c->match_thread(thread) ? c : 0;
    }

    for (int i = 0; consoles()->count(); ++i) {
        auto c = wid2con(consoles()->widget(i));
        if (c->match_thread(thread))
            return c;
    }

    return 0;
}

/** get current active widget
 */
ConsoleEdit *pqMainWindow::consoleActive() const {
    auto t = consoles();
    QWidget *w = t ? t->currentWidget() : centralWidget();
    Q_ASSERT(w && wid2con(w));
    return wid2con(w);
}

/** qualify widget type
 */
QTabWidget *pqMainWindow::consoles() const {
    return qobject_cast<QTabWidget*>(centralWidget());
}

/** switch the interface to tabbed on creation of second (and subsequent) console
 */
void pqMainWindow::addConsole(ConsoleEdit *console, QString title) {
    auto t = consoles();
    if (!t) {
        auto c = wid2con(centralWidget());
        t = new QTabWidget;
        t->setTabsClosable(true);
        QString T = windowTitle();
        if (T.isEmpty())
            T = "Main";
        t->addTab(c, T);
        setCentralWidget(t);
        connect(t, SIGNAL(tabCloseRequested(int)), this, SLOT(tabCloseRequested(int)));
    }
    int i = t->addTab(console, title);
    t->setCurrentIndex(i);
    console->setFocus();
}

/** handle the close button, issuing console request and removing from tab
 */
void pqMainWindow::tabCloseRequested(int tabId) {
    auto c = wid2con(consoles()->widget(tabId));
    if (c->can_close()) {
        if (tabId == 0) {
            QMessageBox b(this);
            b.setWindowTitle(tr("Cannot close"));
            b.setText(tr("[%1] is the primary console.\nDo you want to quit?").arg(c->titleLabel()));
            b.setIcon(b.Question);
            b.setStandardButtons(b.Yes | b.No);
            if (b.exec() == b.Yes)
                qApp->quit();
            return;
        }
        consoles()->removeTab(tabId);
    }
}

/** close console by object
 */
void pqMainWindow::remConsole(ConsoleEdit *c) {
    if (auto t = consoles())
        if (t->indexOf(c) > 0)
            t->removeTab(t->indexOf(c));
}

/** handle menu dispatch by means of Qt signal mapper
 */
void pqMainWindow::addActionPq(ConsoleEdit *ce, QMenu *cmmenu, QString label, QString action) {
    QAction *a = cmmenu->addAction(label, menu2pl, SLOT(map()));
    menu2pl->setMapping(a, action);
    if (0 == menu2pl->receivers(SIGNAL(mapped(const QString &))))
        connect(menu2pl, SIGNAL(mapped(const QString &)), ce, SLOT(onConsoleMenuActionMap(const QString &)));
}

/** ditto
 */
QAction* pqMainWindow::add_action(ConsoleEdit *ce, QMenu *mn, QString Label, QString ctxtmod, QString Goal, QAction *before) {

    QAction *a;
    if (!before)
        a = mn->addAction(Label, menu2pl, SLOT(map()));
    else {
        mn->insertAction(before, a = new QAction(Label, mn));
        connect(a, SIGNAL(triggered()), menu2pl, SLOT(map()));
    }
    menu2pl->setMapping(a, ctxtmod + ':' + Goal);

    if (0 == menu2pl->receivers(SIGNAL(mapped(const QString &))))
        connect(menu2pl, SIGNAL(mapped(const QString &)), ce, SLOT(onConsoleMenuActionMap(QString)));

    return a;
}
