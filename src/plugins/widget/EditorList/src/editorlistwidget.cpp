/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// Editor list widget
//==============================================================================

#include "editorlistwidget.h"
#include "i18ninterface.h"

//==============================================================================

#include <QApplication>
#include <QMenu>
#include <QClipboard>

//==============================================================================

namespace OpenCOR {
namespace EditorList {

//==============================================================================

EditorListWidget::EditorListWidget(QWidget *pParent) :
    QListView(pParent),
    CommonWidget(pParent),
    mModel(new QStandardItemModel(this))
{
    // Customise ourselves

#ifdef Q_OS_MAC
    setAttribute(Qt::WA_MacShowFocusRect, false);
    // Note: the above removes the focus border since it messes up our look
#endif
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setFrameShape(QFrame::NoFrame);
    setModel(mModel);

    // Create our context menu

    mContextMenu = new QMenu(this);

    mClearAction = new QAction(this);
    mCopyToClipboardAction = new QAction(this);

    connect(mClearAction, SIGNAL(triggered()),
            this, SLOT(clear()));
    connect(mCopyToClipboardAction, SIGNAL(triggered()),
            this, SLOT(copyToClipboard()));

    mContextMenu->addAction(mClearAction);
    mContextMenu->addSeparator();
    mContextMenu->addAction(mCopyToClipboardAction);

    // We want a context menu

    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showCustomContextMenu(const QPoint &)));

    // Make sure that we are properly initialised

    clear();
}

//==============================================================================

void EditorListWidget::retranslateUi()
{
    // Retranslate our actions

    I18nInterface::retranslateAction(mClearAction, tr("Clear List"),
                                     tr("Clear the list"));
    I18nInterface::retranslateAction(mCopyToClipboardAction, tr("Copy List To Clipboard"),
                                     tr("Copy the list to the clipboard"));
}

//==============================================================================

void EditorListWidget::clear()
{
    // Reset our list of items

    mModel->clear();

    mClearAction->setEnabled(false);
    mCopyToClipboardAction->setEnabled(false);
}

//==============================================================================

void EditorListWidget::addItem(const EditorListItem::Type &pType,
                               const int &pLine, const int &pColumn,
                               const QString &pMessage)
{
    // Add the given item to our list

    mModel->invisibleRootItem()->appendRow(new EditorListItem(pType, pLine, pColumn, pMessage));

    mClearAction->setEnabled(true);
    mCopyToClipboardAction->setEnabled(true);
}

//==============================================================================

void EditorListWidget::selectFirstItem()
{
    // Select the first item in our list

    QStandardItem *listViewItem = mModel->invisibleRootItem()->child(0);

    if (listViewItem)
        setCurrentIndex(listViewItem->index());
}

//==============================================================================

void EditorListWidget::showCustomContextMenu(const QPoint &pPosition) const
{
    Q_UNUSED(pPosition);

    // Show our custom context menu

    mContextMenu->exec(QCursor::pos());
}

//==============================================================================

void EditorListWidget::copyToClipboard()
{
    // Copy our list to the clipboard

    QStringList list = QStringList();

    for (int i = 0, iMax = mModel->rowCount(); i < iMax; ++i) {
        EditorListItem *item = (EditorListItem *) mModel->item(i);
        QString itemType = (item->type() == EditorListItem::Error)?tr("Error"):tr("Warning");

        list << "["+itemType+"] "+item->text();
    }

    QApplication::clipboard()->setText(list.join("\n")+"\n");
}

//==============================================================================

}   // namespace EditorList
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
