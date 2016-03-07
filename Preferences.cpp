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

#include "Preferences.h"
#include <QDebug>

QList<QColor> Preferences::ANSI_sequences;

/** peek color by index
 */
QColor Preferences::ANSI2col(int c, bool highlight) {
    int p = highlight ? c + 8 : c;
    return ANSI_sequences[p];
}

/** get configured values, with reasonable defaults
 */
Preferences::Preferences(QObject *parent) :
    QSettings("SWI-Prolog", "pqConsole", parent)
{
    console_font = value("console_font", QFont("courier", 12)).value<QFont>();
    wrapMode = static_cast<ConsoleEditBase::LineWrapMode>(value("wrapMode", ConsoleEditBase::WidgetWidth).toInt());

    console_out_fore = value("console_out_fore", 0).toInt();
    console_out_back = value("console_out_back", 7).toInt();
    console_inp_fore = value("console_inp_fore", 0).toInt();
    console_inp_back = value("console_inp_back", 15).toInt();

    // selection from SVG named colors
    // see http://www.w3.org/TR/SVG/types.html#ColorKeywords
    static QColor v[] = {
        "black",
        "red",
        "green",
        "brown",
        "blue",
        "magenta",
        "cyan",
        "white",
        "gray",     // 'highlighted' from here
        "magenta",
        "chartreuse",
        "gold",
        "dodgerblue",
        "magenta",
        "lightblue",
        "beige"
    };

    ANSI_sequences.clear();

    beginReadArray("ANSI_sequences");
    for (int i = 0; i < 16; ++i) {
        setArrayIndex(i);
        QColor c = value("color", v[i]).value<QColor>();
	if ( !c.isValid() )	// Play safe if the color is invalid
	    c = v[i];		// Happens on MacOSX 10.11 (El Captain)
	ANSI_sequences.append(c);
    }
    endArray();
}

/** save configured values
 */
Preferences::~Preferences() {

    #define SV(s) setValue(#s, s)

    SV(console_font);
    SV(wrapMode);

    SV(console_out_fore);
    SV(console_out_back);

    SV(console_inp_fore);
    SV(console_inp_back);

    #undef SV

    beginWriteArray("ANSI_sequences");
    for (int i = 0; i < ANSI_sequences.size(); ++i) {
        setArrayIndex(i);
        setValue("color", ANSI_sequences[i]);
    }
    endArray();
}

void Preferences::loadGeometry(QString key, QWidget *w) {
    beginGroup(key);
    QPoint pos = value("pos", QPoint(200, 200)).toPoint();
    QSize size = value("size", QSize(800, 600)).toSize();
    int state = value("state", static_cast<int>(Qt::WindowNoState)).toInt();
    w->move(pos);
    w->resize(size);
    w->setWindowState(static_cast<Qt::WindowStates>(state));
    endGroup();
}

void Preferences::saveGeometry(QString key, QWidget *w) {
    beginGroup(key);
    setValue("pos", w->pos());
    setValue("size", w->size());
    setValue("state", static_cast<int>(w->windowState()));
    endGroup();
}
void Preferences::loadGeometry(QWidget *w) {
    loadGeometry(w->metaObject()->className(), w);
}
void Preferences::saveGeometry(QWidget *w) {
    saveGeometry(w->metaObject()->className(), w);
}
