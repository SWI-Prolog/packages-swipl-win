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
