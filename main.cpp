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

#include <QApplication>
#include "pqMainWindow.h"
#include <QFile>
#include <QDebug>
#include <QTextStream>
#include "swipl_win.h"

static FILE *logfile;

#if QT_VERSION < 0x050000

static QtMsgHandler previous;

static void logger(QtMsgType type, const char *msg)
{
    if (!logfile) {
        if (previous)
            previous(type, msg);
        return;
    }

    switch (type) {
    case QtDebugMsg:
        fprintf(logfile, "Debug: %s\n", msg);
        break;
    case QtWarningMsg:
        fprintf(logfile, "Warning: %s\n", msg);
        break;
    case QtCriticalMsg:
        fprintf(logfile, "Critical: %s\n", msg);
        break;
    case QtFatalMsg:
        fprintf(logfile, "Fatal: %s\n", msg);
        abort();
    }

    fflush(logfile);
}

#else

static QtMessageHandler previous;
static void logger(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if (!logfile) {
        if (previous)
            previous(type, context, msg);
        return;
    }

    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        fprintf(logfile, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtWarningMsg:
        fprintf(logfile, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtCriticalMsg:
        fprintf(logfile, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtFatalMsg:
        fprintf(logfile, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        abort();
    }

    fflush(logfile);
}

#endif

/* Note that QApplication is created using new to avoid destruction
   on program shutdown.  Destroying causes a crash in XCloseDisplay()
   if xpce was started.
*/

int main(int argc, char *argv[]) {
    const char *logname;

    if ( (logname = getenv("SWIPL_LOGFILE")) ) {
        logfile = fopen(logname, "w");
#if QT_VERSION < 0x050000
        previous = qInstallMsgHandler(logger);
#else
        previous = qInstallMessageHandler(logger);
#endif
    }

    auto a = new swipl_win(argc, argv);
    int rc = a->exec();
    qDebug() << "main loop finished" << rc;
    return rc;
}
