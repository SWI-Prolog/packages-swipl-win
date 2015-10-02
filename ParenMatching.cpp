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

#include "ParenMatching.h"
#include <QTextStream>
#include <QTextBlock>

ParenMatching::ParenMatching(QTextCursor c)
    : onOpen(false)
{
    int save_p = c.position();
    QChar p = cc(c), q;
    QTextCursor::MoveOperation d = c.NoMove;

    if (p == '(') q = ')', d = c.Right, onOpen = true;
    if (p == '[') q = ']', d = c.Right, onOpen = true;
    if (p == '{') q = '}', d = c.Right, onOpen = true;

    if (d == c.NoMove && c.movePosition(c.Left)) {
        p = cc(c);
        if (p == ')') q = '(', d = c.Left;
        if (p == ']') q = '[', d = c.Left;
        if (p == '}') q = '{', d = c.Left;
    }

    if (d != c.NoMove) {
        int n = 0;
        while (c.movePosition(d)) {
            QChar z = cc(c);
            if (z == q) {
                if (n-- == 0) {
                    if (onOpen)
                        positions = range(save_p, c.position());
                    else
                        positions = range(c.position(), save_p - 1);
                    break;
                }
            }
            else if (z == p)
                ++n;
        }
    }

    c.setPosition(save_p);
}

/** utility: fetch text in range from a text document
 */
QString ParenMatching::range::plainText(QTextDocument *doc) const
{
    QString x;
    QTextBlock
        b = doc->findBlock(beg),
        e = doc->findBlock(end);
    QTextStream s(&x);
    if (b != e) {
        s << b.text().mid(b.position() - beg) << endl;
        for (b = b.next(); b != e; b = b.next())
            s << b.text() << endl;
        s << b.text().left(end - b.position());
    }
    else
        s << b.text().mid(b.position() - beg, end - beg);
    return x;
}

/** utility: fetch lines containing the range
 */
QString ParenMatching::range::linesText(QTextDocument *doc) const
{
    QString x;
    QTextBlock
        b = doc->findBlock(beg < end ? beg : end),
        e = doc->findBlock(beg < end ? end : beg);
    if (b != doc->end()) {
        QTextStream s(&x);
        for ( ; b != e; b = b.next())
            s << b.text() << endl;
        if (b != doc->end())
            s << b.text();
    }
    return x;
}
