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

#ifndef SWIPROLOGENGINE_H
#define SWIPROLOGENGINE_H

#include <SWI-cpp.h>

#include <QMap>
#include <QMutex>
#include <QThread>
#include <QVariant>
#include <QStringList>
#include <QWaitCondition>
#include <functional>

/** 1. attempt to run generic code inter threads */
typedef std::function<void()> pfunc;

#include "FlushOutputEvents.h"
#include "pqConsole_global.h"

/** interface IO running SWI Prolog engine in background
 */
class PQCONSOLESHARED_EXPORT SwiPrologEngine : public QThread, public FlushOutputEvents {
    Q_OBJECT
public:

    explicit SwiPrologEngine(ConsoleEdit *target, QObject *parent = 0);
    ~SwiPrologEngine();

    /** main console startup point */
    void start(int argc, char **argv);

    /** run query on background thread */
    void query_run(QString text);
    void query_run(QString module, QString text);

    /** run script on background thread */
    void script_run(QString name, QString text);

    /** start/stop a Prolog engine in thread - use for syncronized GUI */
    struct PQCONSOLESHARED_EXPORT in_thread {
        in_thread();
        ~in_thread();

        /** run named <n> script <t> in current thread */
        bool named_load(QString name, QString script, bool silent = true);

    private:
        PlFrame *frame;
    };

    /** handle application quit request in thread that started PL_toplevel */
    static bool quit_request();

    /** utility: make public */
    static void msleep(unsigned long n) { QThread::msleep(n); }

    /** query engine about expected interface */
    static bool is_tty(const FlushOutputEvents *target = 0);

signals:

    /** issued to queue a string to user output */
    void user_output(QString output);

    /** issued to peek input - til to CR - from user */
    void user_prompt(int threadId, bool tty);

    /** signal a query result */
    void query_result(QString query, int occurrence);

    /** signal query completed */
    void query_complete(QString query, int tot_occurrences);

    /** signal exception */
    void query_exception(QString query, QString message);

public slots:

    /** store string in buffer */
    void user_input(QString input);

protected:

    // run a polling loop on buffer and queries
    virtual void run();

    int argc;
    char **argv;

    /** queries to be dispatched to engine thread */
    struct query {
        bool is_script; // change entry type
        QString name;   // arbitrary symbol
        QString text;   // if is_script is path name, else query text
    };

    QMutex sync;
    QByteArray buffer;      // syncronized !
    QList<query> queries;   // syncronized !

    void serve_query(query q);

    static ssize_t _read_(void *handle, char *buf, size_t bufsize);
    static ssize_t _write_(void *handle, char *buf, size_t bufsize);
    static int     _control_(void *handle, int cmd, void *closure);
    ssize_t _read_(char *buf, size_t bufsize);
    ssize_t _read_f(char *buf, size_t bufsize);

    static int halt_engine(int status, void*data);

private slots:

    void awake();

private:

    /** main console singleton (thread constructed differently) */
    static SwiPrologEngine* spe;
    friend struct in_thread;
};

#endif // SWIPROLOGENGINE_H
