/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*******************************************************************************/

//==============================================================================
// CellML Annotation view CellML list widget
//==============================================================================

#pragma once

//==============================================================================

#include "cellmlfile.h"
#include "widget.h"

//==============================================================================

#include <QModelIndex>
#include <QStandardItem>
#include <QStyledItemDelegate>
#include <QStyleOptionViewItem>

//==============================================================================

class QPainter;

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace Core {
    class TreeViewWidget;
}   // namespace Core

//==============================================================================

namespace CellMLAnnotationView {

//==============================================================================

class CellmlAnnotationViewEditingWidget;

//==============================================================================

class CellmlAnnotationViewCellmlElementItemDelegate : public QStyledItemDelegate
{
public:
    virtual void paint(QPainter *pPainter, const QStyleOptionViewItem &pOption,
                       const QModelIndex &pIndex) const;
};

//==============================================================================

class CellmlAnnotationViewCellmlElementItem : public QStandardItem
{
public:
    enum Type {
        Error                 = QStandardItem::UserType,
        Warning               = QStandardItem::UserType+1,
        Model                 = QStandardItem::UserType+2,
        Import                = QStandardItem::UserType+3,
        ImportUnit            = QStandardItem::UserType+4,
        ImportComponent       = QStandardItem::UserType+5,
        Unit                  = QStandardItem::UserType+6,
        UnitElement           = QStandardItem::UserType+7,
        Component             = QStandardItem::UserType+8,
        Variable              = QStandardItem::UserType+9,
        Group                 = QStandardItem::UserType+10,
        RelationshipReference = QStandardItem::UserType+11,
        ComponentReference    = QStandardItem::UserType+12,
        Connection            = QStandardItem::UserType+13,
        ComponentMapping      = QStandardItem::UserType+14,
        VariableMapping       = QStandardItem::UserType+15
    };

    explicit CellmlAnnotationViewCellmlElementItem(const bool &pError, const QString &pText);
    explicit CellmlAnnotationViewCellmlElementItem(const Type &pType, const QString &pText);
    explicit CellmlAnnotationViewCellmlElementItem(const Type &pType,
                                                   iface::cellml_api::CellMLElement *pElement,
                                                   const int pNumber = -1);

    bool isCategory() const;
    virtual int type() const;
    int number() const;

    iface::cellml_api::CellMLElement * element() const;

private:
    bool mCategory;
    Type mType;

    ObjRef<iface::cellml_api::CellMLElement> mElement;

    int mNumber;

    void constructor(const bool &pCategory, const Type &pType,
                     iface::cellml_api::CellMLElement *pElement,
                     const int &pNumber);

    void setIcon(const Type &pType);
};

//==============================================================================

class CellmlAnnotationViewCellmlListWidget : public Core::Widget
{
    Q_OBJECT

public:
    explicit CellmlAnnotationViewCellmlListWidget(CellmlAnnotationViewEditingWidget *pParent);

    virtual void retranslateUi();

    Core::TreeViewWidget * treeViewWidget() const;

    CellmlAnnotationViewCellmlElementItem * currentCellmlElementItem() const;

private:
    CellMLSupport::CellmlFile *mCellmlFile;

    Core::TreeViewWidget *mTreeViewWidget;
    QStandardItemModel *mTreeViewModel;
    CellmlAnnotationViewCellmlElementItemDelegate *mTreeViewItemDelegate;

    QAction *mExpandAllAction;
    QAction *mCollapseAllAction;
    QAction *mRemoveCurrentMetadataAction;
    QAction *mRemoveAllMetadataAction;
    QAction *mOpenImportAction;

    void retranslateDataItem(CellmlAnnotationViewCellmlElementItem *pCellmlElementItem);

    void initializeTreeViewWidget(const bool &pSelectFirstItem = true);

    void populateModel();
    void populateUnitsModel(CellmlAnnotationViewCellmlElementItem *pCellmlElementItem,
                            iface::cellml_api::UnitsSet *pUnitsSet);
    void populateGroupComponentReferenceModel(CellmlAnnotationViewCellmlElementItem *pCellmlElementItem,
                                              iface::cellml_api::ComponentRef *pGroupComponentReference);

    void indexExpandAll(const QModelIndex &pIndex) const;
    void indexCollapseAll(const QModelIndex &pIndex) const;

    bool indexIsAllExpanded(const QModelIndex &pIndex) const;

signals:
    void metadataDetailsRequested(iface::cellml_api::CellMLElement *pCellElement);

private slots:
    void expandAll();
    void collapseAll();
    void removeCurrentMetadata();
    void removeAllMetadata();
    void openImport();

    void resizeTreeViewToContents();

    void updateMetadataDetails(const QModelIndex &pNewIndex,
                               const QModelIndex &pOldIndex);

    void showCustomContextMenu(const QPoint &pPosition) const;
};

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
