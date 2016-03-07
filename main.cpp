/*  Part of SWI-Prolog interface to Qt

    Author:        Carlo Capelli
    E-mail:        cc.carlo.cap@gmail.com
    Copyright (c)  2013-2015, Carlo Capelli
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

#include <QApplication>
#include "pqMainWindow.h"
#include <QFile>
#include <QDebug>
#include <QTextStream>
#include "swipl_win.h"

static FILE *logfile;
static bool  nolog = true;

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
static void
logger(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if ( nolog )
        return;

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
        break;
#if QT_VERSION >= 0x050500
    case QtInfoMsg:
        fprintf(logfile, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
#endif
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

    if ( (logname = getenv("QDEBUG")) ) {
        nolog = false;
        if ( strcmp(logname, "stderr") != 0 )
	    logfile = fopen(logname, "w");
    }

#if QT_VERSION < 0x050000
    previous = qInstallMsgHandler(logger);
#else
    previous = qInstallMessageHandler(logger);
#endif

    auto a = new swipl_win(argc, argv);
    int rc = a->exec();
    qDebug() << "main loop finished" << rc;
    return rc;
}


/* JW: Hack to compile swipl-win on MacOs 10.7 using g++ 4.8 while
   avoiding linking to a new C++ shared object.
*/

#if __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ < 1070
namespace std {
void __throw_bad_function_call()
{ fprintf(stderr, "Bad function call\n");
  exit(1);
}
}
#endif

