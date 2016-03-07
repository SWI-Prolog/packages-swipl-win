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

#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QFont>
#include <QColor>
#include <QSettings>
#include <QTextCharFormat>
#include "ConsoleEdit.h"

/** some configurable user preference
 */
class Preferences : public QSettings
{
    Q_OBJECT
public:

    explicit Preferences(QObject *parent = 0);
    ~Preferences();

    /** let user select with a font dialog */
    QFont console_font;

    /** assign colors by indexes */
    static QList<QColor> ANSI_sequences;

    /** peek color by index */
    static QColor ANSI2col(int c, bool highlight = false);

    /** indices into colors' list */
    int console_out_fore;
    int console_out_back;
    int console_inp_fore;
    int console_inp_back;

    /** enable a scroll bar when not wrapped */
    ConsoleEditBase::LineWrapMode wrapMode;

    /** helpers to save/restore windows placements */
    void loadGeometry(QWidget *w);
    void saveGeometry(QWidget *w);
    void loadGeometry(QString key, QWidget *w);
    void saveGeometry(QString key, QWidget *w);

signals:
    
public slots:
    
};

#endif // PREFERENCES_H
