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

#ifndef _SIGNALSLOTDIALOG_H
#define _SIGNALSLOTDIALOG_H

#include "shared_global_p.h"
#include <QtCore/QStringList>
#include <QtWidgets/QDialog>
#include <QtWidgets/QStandardItemModel>

QT_BEGIN_NAMESPACE

class QDesignerFormEditorInterface;
class QDesignerFormWindowInterface;
class QDesignerDialogGuiInterface;
class QDesignerMemberSheet;
class QListView;
class QToolButton;
class QItemSelection;

namespace Ui {
    class SignalSlotDialogClass;
}

namespace qdesigner_internal {

// Dialog data
struct SignalSlotDialogData {
    void clear();
    QStringList m_existingMethods;
    QStringList m_fakeMethods;
};

// Internal helper class: A model for signatures that allows for verifying duplicates
// (checking signals versus slots and vice versa).
class SignatureModel : public QStandardItemModel {
    Q_OBJECT

public:
    SignatureModel(QObject *parent = 0);
    virtual bool setData (const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

signals:
    void checkSignature(const QString &signature, bool *ok);
};

// Internal helper class: Panel for editing method signatures. List view with validator,
// add and remove button
class SignaturePanel  : public QObject {
     Q_OBJECT

public:
    SignaturePanel(QObject *parent, QListView *listView, QToolButton *addButton, QToolButton *removeButton, const QString &newPrefix);

    QStringList fakeMethods() const;
    void setData(const SignalSlotDialogData &d);
    int count(const QString &signature) const;

signals:
    void checkSignature(const QString &signature, bool *ok);

private slots:
    void slotAdd();
    void slotRemove();
    void slotSelectionChanged(const QItemSelection &, const QItemSelection &);

private:
    void closeEditor();

    const QString m_newPrefix;
    SignatureModel *m_model;
    QListView *m_listView;
    QToolButton *m_removeButton;
};

// Dialog for  editing signals and slots.
// Provides static convenience function
// to modify fake signals and slots. They are
// handled in 2 ways:
// 1) For the MainContainer: Fake signals and slots are stored
//    in the meta database (per-instance)
// 2) For promoted widgets: Fake signals and slots are stored
//    in the widget database (per-class)
// Arguably, we could require the MainContainer to be promoted for that, too,
// but that would require entering a header.

class QDESIGNER_SHARED_EXPORT SignalSlotDialog : public QDialog {
    Q_OBJECT

public:
    enum FocusMode { FocusSlots, FocusSignals };

    explicit SignalSlotDialog(QDesignerDialogGuiInterface *dialogGui, QWidget *parent = 0, FocusMode m = FocusSlots);
    virtual ~SignalSlotDialog();

    DialogCode showDialog(SignalSlotDialogData &slotData, SignalSlotDialogData &signalData);

    // Edit fake methods stored in MetaDataBase (per instance, used for main containers)
    static bool editMetaDataBase(QDesignerFormWindowInterface *fw, QObject *object, QWidget *parent = 0, FocusMode m = FocusSlots);

    // Edit fake methods of a promoted class stored in WidgetDataBase (synthesizes a widget to obtain existing members).
    static bool editPromotedClass(QDesignerFormEditorInterface *core, const QString &promotedClassName, QWidget *parent = 0, FocusMode m = FocusSlots);
    // Edit fake methods of a promoted class stored in WidgetDataBase on a base class instance.
    static bool editPromotedClass(QDesignerFormEditorInterface *core, QObject *baseObject, QWidget *parent = 0, FocusMode m = FocusSlots);

private slots:
    void slotCheckSignature(const QString &signature, bool *ok);

private:
    // Edit fake methods of a promoted class stored in WidgetDataBase using an instance of the base class.
    static bool editPromotedClass(QDesignerFormEditorInterface *core, const QString &promotedClassName, QObject *baseObject, QWidget *parent, FocusMode m);

    const FocusMode m_focusMode;
    Ui::SignalSlotDialogClass *m_ui;
    QDesignerDialogGuiInterface *m_dialogGui;
    SignaturePanel *m_slotPanel;
    SignaturePanel *m_signalPanel;
};
}

QT_END_NAMESPACE

#endif
