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

#include "qdesigner_formwindowmanager_p.h"
#include "plugindialog_p.h"

#include <QtDesigner/QDesignerFormEditorInterface>

QT_BEGIN_NAMESPACE

using namespace qdesigner_internal;

/*!
    \class QDesignerFormWindowManager

    Extends QDesignerFormWindowManagerInterface with methods to control
    the preview and printing of forms. It provides a facade that simplifies
    the complexity of the more general PreviewConfiguration & PreviewManager
    interfaces.

    \since 4.5
  */


QDesignerFormWindowManager::QDesignerFormWindowManager(QObject *parent)
    : QDesignerFormWindowManagerInterface(parent), m_unused(0)
{
}

QDesignerFormWindowManager::~QDesignerFormWindowManager()
{
}

/*!
    \fn PreviewManager *QDesignerFormWindowManager::previewManager()

    Accesses the previewmanager implementation.

    \since 4.5
    \internal
  */

void QDesignerFormWindowManager::showPluginDialog()
{
    PluginDialog dlg(core(), core()->topLevel());
    dlg.exec();
}

/*!
    \fn
    void QDesignerFormWindowManager::formWindowSettingsChanged(QDesignerFormWindowInterface *fw);

    This signal is emitted when the form settings dialog was shown
    and changes have been made to the form.

    \since 4.5
    \internal
*/


QT_END_NAMESPACE
