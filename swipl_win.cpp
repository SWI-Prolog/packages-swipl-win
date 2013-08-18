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

#include "swipl_win.h"
#include "SwiPrologEngine.h"
#include "PREDICATE.h"
#include <QFileOpenEvent>
#include <QDebug>

swipl_win::swipl_win(int& argc, char **argv) :
    QApplication(argc, argv)
{
    // I've moved the window locally, to reference in event handler
    w = new pqMainWindow(argc, argv);
    w->show();
}

bool swipl_win::event(QEvent *event) {
    switch (event->type()) {
    case QEvent::FileOpen:
      { QString name = static_cast<QFileOpenEvent *>(event)->file();

	qDebug() << "FileOpen: " << name;
        SwiPrologEngine::in_thread _it;
	try {
	    PlCall("prolog", "file_open_event", PlTermv(name.toStdWString().data()));
	} catch(PlException e) {
	    qDebug() << CCP(e);
	}
        return true;
      }
    default:
        return QApplication::event(event);
    }
}
