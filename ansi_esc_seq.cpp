/*  Part of SWI-Prolog interface to Qt

    Author:        Carlo Capelli
    E-mail:        cc.carlo.cap@gmail.com
    Copyright (c)  2019, Carlo Capelli
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

#include "ansi_esc_seq.h"
#include "Preferences.h"

#include <QRegularExpression>
#include <QDebug>

#define ESC_CSI "\033["

ANSI_ESC_SEQ::ANSI_ESC_SEQ(const QString src, QTextCharFormat &tcf)
    : src(src),
      tcf(tcf),
      off(0),
      pos(src.indexOf(ESC_CSI)) // note: only CSI right now
{
}
ANSI_ESC_SEQ::~ANSI_ESC_SEQ()
{
}

/*
Code	Name	Effect
CSI n A		CUU - Cursor Up		Moves the cursor n (default 1) cells in the given direction. If the cursor is already at the edge of the screen, this has no effect.
CSI n B		CUD - Cursor Down
CSI n C		CUF - Cursor Forward
CSI n D		CUB - Cursor Back
CSI n E		CNL - Cursor Next Line	Moves cursor to beginning of the line n (default 1) lines down. (not ANSI.SYS)
CSI n F		CPL - Cursor Previous Line	Moves cursor to beginning of the line n (default 1) lines up. (not ANSI.SYS)
CSI n G		CHA - Cursor Horizontal Absolute	Moves the cursor to column n (default 1). (not ANSI.SYS)
CSI n ; m H	CUP - Cursor Position	Moves the cursor to row n, column m. The values are 1-based, and default to 1 (top left corner) if omitted. A sequence such as CSI ;5H is a synonym for CSI 1;5H as well as CSI 17;H is the same as CSI 17H and CSI 17;1H
CSI n J		ED - Erase in Display	Clears part of the screen. If n is 0 (or missing), clear from cursor to end of screen. If n is 1, clear from cursor to beginning of the screen. If n is 2, clear entire screen (and moves cursor to upper left on DOS ANSI.SYS). If n is 3, clear entire screen and delete all lines saved in the scrollback buffer (this feature was added for xterm and is supported by other terminal applications).
CSI n K		EL - Erase in Line	Erases part of the line. If n is 0 (or missing), clear from cursor to the end of the line. If n is 1, clear from cursor to beginning of the line. If n is 2, clear entire line. Cursor position does not change.
CSI n S		SU - Scroll Up	Scroll whole page up by n (default 1) lines. New lines are added at the bottom. (not ANSI.SYS)
CSI n T		SD - Scroll Down	Scroll whole page down by n (default 1) lines. New lines are added at the top. (not ANSI.SYS)
CSI n ; m f	HVP - Horizontal Vertical Position	Same as CUP
CSI n m		SGR - Select Graphic Rendition	Sets the appearance of the following characters, see SGR parameters below.
CSI 5i	AUX Port On	Enable aux serial port usually for local serial printer
CSI 4i	AUX Port Off	Disable aux serial port usually for local serial printer
CSI 6n	DSR - Device Status Report	Reports the cursor position (CPR) to the application as (as though typed at the keyboard) ESC[n;mR, where n is the row and m is the column.)
CSI s	SCP - Save Cursor Position	Saves the cursor position/state.
CSI u	RCP - Restore Cursor Position	Restores the cursor position/state.
*/

void ANSI_ESC_SEQ::Out::setStyle(QTextCharFormat &tcf) const
{
    if (mode.fg != SGR::Color::_Color)
        tcf.setForeground(Preferences::ANSI2col(mode.fg));
    else if (mode.bright_fg != SGR::Color::_Color)
        tcf.setForeground(Preferences::ANSI2col(mode.bright_fg, true));
    else
        tcf.setForeground(Preferences::ANSI2col(Preferences::console_out_fore));

    if (mode.bg != SGR::Color::_Color)
        tcf.setBackground(Preferences::ANSI2col(mode.bg));
    else if (mode.bright_bg != SGR::Color::_Color)
        tcf.setBackground(Preferences::ANSI2col(mode.bright_bg, true));
    else
        tcf.setBackground(Preferences::ANSI2col(Preferences::console_out_back));

    switch (mode.f) {
    case SGR::Formatting::Bold:
        tcf.setFontWeight(QFont::Bold);
        break;
    default:
        tcf.setFontWeight(QFont::Normal);
    }
}

QString ANSI_ESC_SEQ::next()
{
    Q_ASSERT(*this);
    Out seq;

    if (off < pos) {
        // out text before current sequence
        seq.out = src.mid(off, pos - off);
        off = pos;
        return seq.out;
    }

    auto _f = SGR::Formatting::_Formatting;
    auto _c = SGR::Color::_Color;
    seq.mode = SGR {
        _f,
        _c, _c, _c, _c
    };

    static QRegularExpression eseq("(0m|39m)|(1;1;1m|1;1m|1m)|(3[0-7]m|3[0-7];1m|3[0-7];1;1m)|(1;3[0-7];1m)|(1;3[0-7]m)|(4[0-7]m)|(9[0-7]m)");
    QRegularExpressionMatch m = eseq.match(src, pos + 2);
    QString p;

    //static int z = 0;
    //qDebug() << "next" << z++ << pos << off << m.hasMatch() << m.capturedTexts() << src;

    if (m.hasMatch()) {
        if (!(p = m.captured(1)).isNull()) {
            ; //tcf = save;
        } else
        if (!(p = m.captured(2)).isNull()) {
            seq.mode.f = SGR::Formatting::Bold;
        } else
        if (!(p = m.captured(3)).isNull()) {
            seq.mode.fg = SGR::Color(p[1].digitValue());
        } else
        if (!(p = m.captured(4)).isNull()) {
            //? seq.mode.f = SGR::Formatting::Bold;
            seq.mode.fg = SGR::Color(p[3].digitValue());
        } else
        if (!(p = m.captured(5)).isNull()) {
            seq.mode.f = SGR::Formatting::Bold;
            seq.mode.fg = SGR::Color(p[3].digitValue());
        } else
        if (!(p = m.captured(6)).isNull()) {
            seq.mode.bg = SGR::Color(p[1].digitValue());
        } else
        if (!(p = m.captured(7)).isNull()) {
            seq.mode.bright_fg = SGR::Color(p[1].digitValue());
        }

        if (!p.isNull()) {
            seq.setStyle(tcf);

            // text to output
            off = pos + 2 + p.length();
            pos = src.indexOf(ESC_CSI, off);
            if (pos == -1)
                seq.out = src.mid(off);
            else
                seq.out = src.mid(off, pos - off);
            off = pos;

        } else {
            qDebug() << "extraction failed!" << src.right(pos + 2).mid(5);
        }

    } else {
        qDebug() << "no pattern match!" << src.right(pos + 2).mid(5);
        pos = -1;
    }

    return seq.out;
}
