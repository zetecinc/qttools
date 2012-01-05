/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Designer of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of Qt Designer.  This header
// file may change from version to version without notice, or even be removed.
//
// We mean it.
//

#ifndef CSSHIGHLIGHTER_H
#define CSSHIGHLIGHTER_H

#include <QtGui/QSyntaxHighlighter>
#include "shared_global_p.h"

QT_BEGIN_NAMESPACE

namespace qdesigner_internal {

class QDESIGNER_SHARED_EXPORT CssHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    explicit CssHighlighter(QTextDocument *document);

protected:
    void highlightBlock(const QString&);
    void highlight(const QString&, int, int, int/*State*/);

private:
    enum State { Selector, Property, Value, Pseudo, Pseudo1, Pseudo2, Quote, 
                 MaybeComment, Comment, MaybeCommentEnd };
};

} // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // CSSHIGHLIGHTER_H
