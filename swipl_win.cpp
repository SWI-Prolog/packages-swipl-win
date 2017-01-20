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
