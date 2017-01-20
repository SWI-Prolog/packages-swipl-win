/*  Part of SWI-Prolog interface to Qt

    Author:        Carlo Capelli
    E-mail:        cc.carlo.cap@gmail.com
    Copyright (c)  2015, 2014,2015
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

#ifndef PARENMATCHING_H
#define PARENMATCHING_H

#include "lqUty_global.h"

#include <QObject>
#include <QTextCursor>
#include <QTextDocument>
#include <QTextCharFormat>

/** get open/close parenthesis matching from QTextCursor current position
 */
class LQUTYSHARED_EXPORT ParenMatching : public QObject
{
    Q_OBJECT
public:

    /** a pair of text positions */
    struct LQUTYSHARED_EXPORT range {

        /** initialize invalid */
        range(int beg = -1, int end = -1) : beg(beg), end(end) {}

        /** add intialization */
        range(int position, int removed, int added) : beg(position), end(position - removed + added) { }

        int beg, end;

        int size() const { return end - beg; }
        bool contains(const int p) const { return beg <= p && end > p; }
        bool overlap(const int p) const { return beg <= p && end >= p; }
        bool operator==(const range &r) const { return r.end == end && r.beg == beg; }

        void normalize() { if (beg > end) std::swap(beg, end); }

        /** apply range selection to cursor */
        QTextCursor select(QTextCursor C) const {
            C.setPosition(beg);
            C.movePosition(C.NextCharacter, C.KeepAnchor, size());
            return C;
        }

        /** apply range formatting to cursor */
        static QTextCursor format(QTextCursor c, int p, int n = 1, QTextCharFormat f = QTextCharFormat()) {
            c.setPosition(p);
            c.movePosition(c.Right, c.KeepAnchor, n);
            c.setCharFormat(f);
            return c;
        }

        /** apply range formatting to cursor */
        QTextCursor format(QTextCursor c, QTextCharFormat f = QTextCharFormat()) const {
             select(c).setCharFormat(f);
             return c;
        }

        /** apply formatting to cursor */
        QTextCursor format_both(QTextCursor c, QTextCharFormat f = QTextCharFormat()) const {
            return format(format(c, beg, 1, f), end, 1, f);
        }

        /** some common usage */
        static QTextCharFormat bold() { QTextCharFormat f; f.setFontWeight(QFont::Bold); return f; }
        static QTextCharFormat underline_wave(bool y = true) {
            QTextCharFormat f;
            f.setUnderlineStyle(y ? f.WaveUnderline : f.NoUnderline);
            return f;
        }

        /** utility: fetch text in range from a text document */
        QString plainText(QTextDocument *doc) const;

        /** utility: fetch lines containing the range */
        QString linesText(QTextDocument *doc) const;
    };

    /** fetch a single character from current position */
    static QChar cc(QTextCursor c) {
        if (c.movePosition(c.Right, c.KeepAnchor))
            return c.selectedText()[0];
        return QChar();
    }

    /** apply matching to current cursor position */
    explicit ParenMatching(QTextCursor c);

    /** true if match found */
    operator bool() const { return positions.size() > 0; }

    /** match in text cursor positions */
    range positions;

    /** true if matched a closed parenthesis (c.position(open)) is on open) */
    bool onOpen;

signals:
    
public slots:
    
};

#endif // PARENMATCHING_H
