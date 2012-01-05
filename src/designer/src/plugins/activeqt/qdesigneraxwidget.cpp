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

#include "qdesigneraxwidget.h"

#include <QtCore/QMetaProperty>
#include <QtCore/QDebug>
#include <QtWidgets/QIcon>
#include <QtGui/QPainter>
#include <QtWidgets/QResizeEvent>

#include <ActiveQt/QAxWidget>

#include <olectl.h>
#include <qaxtypes.h>

enum { debugAxWidget = 0 };

QT_BEGIN_NAMESPACE

/* XPM */
const char *widgetIconXPM[]={
"22 22 6 1",
"a c #000000",
"# c #808080",
"+ c #aaa5a0",
"b c #dddddd",
"* c #d4d0c8",
". c none",
".........#aa#...#aa#..",
".........abba...abba..",
".........abba...abba..",
".........#aa#...#aa#..",
"..........aa.....aa...",
"..........aa.....aa...",
"..........aa.....aa...",
".......aaaaaaaaaaaaaaa",
".......a*************a",
".......a************#a",
".......a***********+#a",
".......a***********+#a",
".......a***********+#a",
"#aa#...a***********+#a",
"abbaaaaa***********+#a",
"abbaaaaa***********+#a",
"#aa#...a***********+#a",
".......a***********+#a",
".......a***********+#a",
".......a**++++++++++#a",
".......a*############a",
".......aaaaaaaaaaaaaaa"};

QDesignerAxWidget::QDesignerAxWidget(QWidget *parent) :
    QWidget(parent),
    m_defaultSize(80, 70),
    m_drawFlags(DrawIndicator|DrawFrame|DrawControl),
    m_axobject(0),
    m_axImage(widgetIcon())
{
}

QDesignerAxWidget::~QDesignerAxWidget()
{
    delete m_axobject;
}

QPixmap QDesignerAxWidget::widgetIcon()
{
   return QPixmap(widgetIconXPM);
}

QString QDesignerAxWidget::control() const
{
    if (m_axobject)
        return m_axobject->control();
    return QString();
}

void QDesignerAxWidget::setControl(const QString &clsid)
{
    if (clsid == control())
        return;
     if (clsid.isEmpty()) {
         resetControl();
     } else {
         loadControl(clsid);
     }
}
void QDesignerAxWidget::resetControl()
{
    if (!m_axobject)
        return;
    delete m_axobject;
    m_axobject = 0;
    update();
}

bool QDesignerAxWidget::loadControl(const QString &clsid)
{
    if (clsid.isEmpty())
        return false;
    if (m_axobject)
        resetControl();
    m_axobject = new QAxWidget();

    if (!m_axobject->setControl(clsid)) {
        delete m_axobject;
        m_axobject = 0;
        return false;
    }
    update();
    return true;
}

QSize QDesignerAxWidget::sizeHint() const
{
    if (m_axobject)
        return m_axobject->sizeHint();
    return m_defaultSize;
}

QSize QDesignerAxWidget::minimumSizeHint() const
{
    if (m_axobject)
        return m_axobject->minimumSizeHint();
    return QWidget::minimumSizeHint();
}

void QDesignerAxWidget::paintEvent(QPaintEvent * /*event */)
{
    QPainter p(this);
    const QRect r = contentsRect();
    const int contentsWidth = r.width();
    const int contentsHeight= r.height();
    if (m_axobject) { // QAxWidget has no concept of sizeHint()
        if (m_drawFlags & DrawControl) {
            m_axobject->resize(size());
            m_axobject->render(&p);
        }
        if (m_drawFlags & DrawIndicator) {
            static const QString loaded = tr("Control loaded");
            QColor patternColor(Qt::green);
            if (m_drawFlags & DrawControl)
                patternColor.setAlpha(80);
            p.setBrush(QBrush(patternColor, Qt::BDiagPattern));
            p.setPen(Qt::black);
            if (r.height() > 5)
                p.drawText(5,contentsHeight - 5, loaded);
        }
    }
    if (m_drawFlags & DrawFrame) {
        p.drawRect(r.adjusted(0, 0, -1, -1));
    }
    if (m_drawFlags & DrawIndicator) {
        if (contentsWidth > m_axImage.width() && contentsHeight > m_axImage.height())
            p.drawPixmap((contentsWidth - m_axImage.width()) / 2,
                         (contentsHeight-m_axImage.height()) / 2, m_axImage);
    }
}

// --------- QDesignerAxPluginWidget
QDesignerAxPluginWidget::QDesignerAxPluginWidget(QWidget *parent) :
        QDesignerAxWidget(parent)
{
}

QDesignerAxPluginWidget::~QDesignerAxPluginWidget()
{
}

const QMetaObject *QDesignerAxPluginWidget::metaObject() const
{
    if (const QAxWidget *aw = axobject())
        return aw->metaObject();

    return QDesignerAxWidget::metaObject();
}

static QString msgComException(const QObject *o, const QMetaObject::Call call, int index)
{
    return QDesignerAxWidget::tr("A COM exception occurred when executing a meta call of type %1, index %2 of \"%3\".").
            arg(call).arg(index).arg(o->objectName());
}

int QDesignerAxPluginWidget::qt_metacall(QMetaObject::Call call, int signal, void **argv)
{
    QAxWidget *aw = axobject();
    if (!aw)
        return QDesignerAxWidget::qt_metacall(call,signal,argv);


    const QMetaObject *mo = metaObject();
    // Have base class handle inherited stuff (geometry, enabled...)
    const bool inherited = call == QMetaObject::InvokeMetaMethod ?
                           (signal < mo->methodOffset()) : (signal < mo->propertyOffset());
    if (inherited)
        return QDesignerAxWidget::qt_metacall(call, signal, argv);

    int rc = -1;
#ifndef QT_NO_EXCEPTIONS
    try {
#endif
        if (debugAxWidget)
               if (call != QMetaObject::InvokeMetaMethod)
                   qDebug() << objectName() << call << signal << mo->property(signal).name();
        switch (call) {
        case QMetaObject::QueryPropertyStored: // Pretend all changed properties are stored for them to be saved
            if (m_propValues.contains(signal))
                if (argv[0])
                    *reinterpret_cast< bool*>(argv[0]) = true;
            break;
        case QMetaObject::ResetProperty:
            rc = aw->qt_metacall(call, signal, argv);
            update();
            m_propValues.remove(signal);
            break;
        case QMetaObject::WriteProperty:
            rc = aw->qt_metacall(call, signal, argv);
            update();
            m_propValues.insert(signal, true);
            break;
        default:
            rc = aw->qt_metacall(call, signal, argv);
            break;
        }
#ifndef QT_NO_EXCEPTIONS
    } catch(...) {
        qWarning(msgComException(this, call, signal).toUtf8());
     }
#endif
    return rc;
}

QT_END_NAMESPACE
