/*
    lqUty        : loqt utilities

    Author       : Carlo Capelli
    E-mail       : cc.carlo.cap@gmail.com
    Copyright (C): 2013,2014,2015

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
