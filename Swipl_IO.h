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

#ifndef SWIPL_IO_H
#define SWIPL_IO_H

#include "ConsoleEdit.h"
#include "SwiPrologEngine.h"

/** This class keeps the essential elements to get console behaviour.
 *  It's obtained from [SwiPrologEngine](@ref SwiPrologEngine), but tied to a SWI-Prolog built thread
 */
class Swipl_IO : public QObject, public FlushOutputEvents {
    Q_OBJECT

public:

    /** fill the buffer */   static ssize_t _read_f(void *handle, char *buf, size_t bufsize);
    /** empty the buffer */  static ssize_t _write_f(void *handle, char*buf, size_t bufsize);
    /** seek to position */  static long    _seek_f(void *handle, long pos, int whence);
    /** close stream */      static int     _close_f(void *handle);
    /** Info/control */      static int     _control_f(void *handle, int action, void *arg);
    /** seek to position */  static int64_t _seek64_f(void *handle, int64_t pos, int whence);

    /** standard interface */
    explicit Swipl_IO(QObject *parent = 0);

    /** surrogate signal/slot not working in foreign thread */
    void take_input(QString cmd);

    /** foreign thread connection completed */
    void attached(ConsoleEdit *c);

    void query_run(QString query);

private:

    /** syncronize inter thread access to buffer and query */
    QMutex sync;

    /** output text buffer, made UTF8 */
    QByteArray buffer;

    /** factorize access to members */
    ssize_t _read_(char *buf, size_t bufsize);

    /** allows a call without issuing the read */
    QString query;

    /** termination control */
    static void eng_at_exit(void *);

signals:

    /** issued to queue a string to user output */
    void user_output(QString output);

    /** issued to peek input - til to CR - from user */
    void user_prompt(int threadId, bool tty);

    /**  attempt to run generic code inter threads */
    void sig_eng_at_exit();

public slots:

    /** store string in buffer */
    void user_input(QString input);
};

#endif // SWIPL_IO_H
