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

#include "Preferences.h"

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
        ANSI_sequences.append(value("color", v[i]).value<QColor>());
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
