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
