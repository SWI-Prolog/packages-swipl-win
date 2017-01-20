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

#ifndef CONSOLEEDIT_H
#define CONSOLEEDIT_H

#include <QEvent>
#include <QCompleter>

#ifndef PQCONSOLE_NO_HTML
    #include <QTextEdit>
    typedef QTextEdit ConsoleEditBase;
#elif PQCONSOLE_BROWSER
    #include <QTextBrowser>
    typedef QTextBrowser ConsoleEditBase;
#else
    #include <QPlainTextEdit>
    typedef QPlainTextEdit ConsoleEditBase;
#endif

#include "SwiPrologEngine.h"
#include "Completion.h"

#include <QElapsedTimer>

class Swipl_IO;

/** client side of command line interface
  * run in GUI thread, sync using SwiPrologEngine interface
  */
class PQCONSOLESHARED_EXPORT ConsoleEdit : public ConsoleEditBase {
    Q_OBJECT
    Q_PROPERTY(int updateRefreshRate READ updateRefreshRate WRITE setUpdateRefreshRate)

public:

    /** build command line interface to SWI Prolog engine */
    ConsoleEdit(int argc, char **argv, QWidget *parent = 0);

    /** create in prolog thread - call win_open_console() */
    ConsoleEdit(Swipl_IO* io, QString Title);

    /** create in prolog thread - from win_open_console(), add to tabbed interface */
    ConsoleEdit(Swipl_IO* io);

    /** push command on queue */
    bool command(QString text);

    /** access low level interface */
    SwiPrologEngine* engine() { return eng; }

    /** a console is associated with a worker Prolog thread
     *  should handle the case of yet-to-be-initialized root console
     */
    bool match_thread(int thread_id) const;

    /** should always match PL_thread_id() ... */
    int thread_id() const { return thids[0]; }
    void add_thread(int id);

    /** remove all text */
    void tty_clear();

    /** make public property, then available on Prolog side */
    int updateRefreshRate() const { return update_refresh_rate; }
    void setUpdateRefreshRate(int v) { update_refresh_rate = v; }

    /** create a new console, bound to calling thread */
    void new_console(Swipl_IO *e, QString title);

    /** closeEvent only called for top level widgets */
    bool can_close();

    /** 4. attempt to run generic code inter threads */
    void exec_func(pfunc f) { emit sig_run_function(f); }

    /** 5. helper syncronization for modal loop */
    struct PQCONSOLESHARED_EXPORT exec_sync {
        exec_sync(int timeout_ms = 100);

        void stop();
        void go();

    private:
        QThread *stop_, *go_;
        QMutex sync;
        int timeout_ms;
    };

    /** give access to rl_... predicates */
    const QStringList& history_lines() const { return history; }
    void add_history_line(QString line);

    /** run interrupt/0 */
    void int_request();

    /** just check the status member */
    bool is_running() const { return status == running; }

    /** the user identifying label is attached somewhere to parents chain */
    QString titleLabel();

    /** route menu / action */
    void addActionPq(QMenu *cmmenu, QString label, QString action);

    /** html_write */
    void html_write(QString html);

    /** can be disabled from ~/.plrc */
    static bool color_term;

protected:

    /** host actual interface object, running in background */
    SwiPrologEngine *eng;

    /** can't get <eng> to work on a foreign thread - initiated from SWI-Prolog */
    Swipl_IO *io;

    /** strict control on keyboard events required */
    virtual void keyPressEvent(QKeyEvent *event);

    /** jump to source location on warning/error messages */
    virtual void mousePressEvent(QMouseEvent *e);

    /** support completion */
    virtual void focusInEvent(QFocusEvent *e);

    /** filter out insertion when cursor is not in editable position */
    virtual void insertFromMimeData(const QMimeData *source);

    /** support SWI... exec thread console creation */
    struct req_new_console : public QEvent {
        Swipl_IO *iop;
        QString title;
        req_new_console(Swipl_IO *iop, QString title) : QEvent(User), iop(iop), title(title) {}
    };
    virtual void customEvent(QEvent *event);

    /** handle tooltip placing */
    virtual bool event(QEvent *event);

    /** sense word under cursor for tooltip display */
    virtual bool eventFilter(QObject *, QEvent *event);

    /** output/input text attributes */
    QTextCharFormat output_text_fmt, input_text_fmt;

    /** start point of engine output insertion */
    /** i.e. keep last user editable position */
    int fixedPosition;

    /** commands to be dispatched to engine thread */
    QStringList commands;

    /** poor man command history */
    QStringList history;
    int history_next;
    QString history_spare;

    /** count output before setting cursor at end */
    int count_output;

    /** interval on count_output, determine how often to force output flushing */
    int update_refresh_rate;

    /** autocompletion - today not context sensitive */
    /** will eventually become with help from the kernel */
    typedef QCompleter t_Completion;
    t_Completion *preds;
    QStringList lmodules;

    /** factorize code, attempt to get visual clue from QCompleter */
    void compinit(QTextCursor c);
    void compinit2(QTextCursor c);

    /** associated thread id (see PL_thread_self()) */
    QList<int> thids;

    /** wiring etc... */
    void setup();
    void setup(Swipl_IO *iop);

    /** tooltips & completion support, from helpidx.pl */
    QString last_word, last_tip;
    void set_cursor_tip(QTextCursor c);

    /** attempt to track *where* to place outpout */
    enum e_status { idle, attaching, wait_input, running, closing, eof };
    e_status status;
    int promptPosition;
    bool is_tty;

    // while solving inter threads problems...
    friend class SwiPrologEngine;
    friend class Swipl_IO;

    /** need to sense the processor type to execute code
     *  bypass IO based execution, direct calling
     */
    void query_run(QString call);
    void query_run(QString module, QString call);

    /** check if line content is appropriate, then highlight or open editor on it */
    void clickable_message_line(QTextCursor c, bool highlight);
#ifdef PQCONSOLE_HANDLE_HOOVERING
    int cposition;
    QTextCharFormat fposition;
#endif

    /** sense URL */
    virtual void setSource(const QUrl & name);

    /** relax selection check requirement */
    QElapsedTimer sel_check_timing;

public slots:

    /** display different cursor where editing available */
    void onCursorPositionChanged();

    /** serve console menus */
    void onConsoleMenuAction();
    void onConsoleMenuActionMap(const QString &action);

    /** 2. attempt to run generic code inter threads */
    void run_function(pfunc f) { f(); }

protected slots:

    /** send text to output */
    void user_output(QString text);

    /** issue an input request */
    void user_prompt(int threadId, bool tty);

    /** push command from queue to Prolog processor */
    void command_do();

    /** push completion request in current command line */
    void insertCompletion(QString completion);

    /** when engine gracefully complete-... */
    void eng_completed();

    /** handle HREF (simpler is query_run(Target) */
    void anchorClicked(const QUrl &link);

    /** highlight related 'symbols' on selection */
    void selectionChanged();

signals:

    /** issued to serve prompt */
    void user_input(QString);

    /** 3. attempt to run generic code inter threads */
    void sig_run_function(pfunc f);
};

#endif
