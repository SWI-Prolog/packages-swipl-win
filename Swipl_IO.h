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
