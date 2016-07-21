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
// SED-ML Editing view widget
//==============================================================================

#include "borderedwidget.h"
#include "corecliutils.h"
#include "coreguiutils.h"
#include "editorlistwidget.h"
#include "editorwidget.h"
#include "sedmleditingviewwidget.h"

//==============================================================================

#include <QApplication>
#include <QDesktopWidget>
#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace SEDMLEditingView {

//==============================================================================

SedmlEditingViewWidget::SedmlEditingViewWidget(const QString &pContents,
                                               const bool &pReadOnly,
                                               QsciLexer *pLexer,
                                               QWidget *pParent) :
    QSplitter(pParent),
    Core::CommonWidget(),
    mEditingWidgetSizes(QIntList())
{
    // Set our orientation

    setOrientation(Qt::Vertical);

    // Keep track of our movement

    connect(this, SIGNAL(splitterMoved(int, int)),
            this, SLOT(splitterMoved()));

    // Create our editor and editor list

    mEditor = new EditorWidget::EditorWidget(pContents, pReadOnly, pLexer, this);
    mEditorList = new EditorWidget::EditorListWidget(this);

    connect(mEditorList, SIGNAL(itemRequested(OpenCOR::EditorWidget::EditorListItem *)),
            this, SLOT(itemRequested(OpenCOR::EditorWidget::EditorListItem *)));

    // Add the bordered editor and editor list to ourselves

    addWidget(new Core::BorderedWidget(mEditor, false, false, true, false));
    addWidget(new Core::BorderedWidget(mEditorList, true, false, false, false));

    // Set our focus proxy to our editor

    setFocusProxy(mEditor);
}

//==============================================================================

static const auto SettingsSedmlEditingViewWidgetSizes = QStringLiteral("SedmlEditingViewWidgetSizes");

//==============================================================================

void SedmlEditingViewWidget::loadSettings(QSettings *pSettings)
{
    // Retrieve and set our sizes
    // Note #1: the editor list's default height is 13% of the desktop's height
    //          while that of the editor
    //          is as big as it can be...
    // Note #2: because the editor's default height is much bigger than that of
    //          our widget, the editor list's default height will effectively be
    //          less than 13% of the desktop's height, but that doesn't matter
    //          at all...

    QVariantList defaultSedmlEditingViewWidgetSizes = QVariantList() << qApp->desktop()->screenGeometry().height()
                                                                     << 0.13*qApp->desktop()->screenGeometry().height();

    mEditingWidgetSizes = qVariantListToIntList(pSettings->value(SettingsSedmlEditingViewWidgetSizes, defaultSedmlEditingViewWidgetSizes).toList());

    setSizes(mEditingWidgetSizes);

    // Retrieve our editor's settings

    mEditor->loadSettings(pSettings);
}

//==============================================================================

void SedmlEditingViewWidget::saveSettings(QSettings *pSettings) const
{
    // Keep track of our sizes

    pSettings->setValue(SettingsSedmlEditingViewWidgetSizes, qIntListToVariantList(mEditingWidgetSizes));

    // Keep track of our editor's settings

    mEditor->saveSettings(pSettings);
}

//==============================================================================

void SedmlEditingViewWidget::retranslateUi()
{
    // Retranslate our editor and editor list

    mEditor->retranslateUi();
    mEditorList->retranslateUi();
}

//==============================================================================

void SedmlEditingViewWidget::updateSettings(SedmlEditingViewWidget *pSedmlEditingViewWidget)
{
    // Make sure that we are given another editing widget

    if (!pSedmlEditingViewWidget || (pSedmlEditingViewWidget == this))
        return;

    // Update our sizes and editor settings

    mEditingWidgetSizes = pSedmlEditingViewWidget->editingWidgetSizes();

    setSizes(mEditingWidgetSizes);

    mEditor->updateSettings(pSedmlEditingViewWidget->editor());
}

//==============================================================================

EditorWidget::EditorWidget * SedmlEditingViewWidget::editor() const
{
    // Return our editor

    return mEditor;
}

//==============================================================================

EditorWidget::EditorListWidget * SedmlEditingViewWidget::editorList() const
{
    // Return our editor list

    return mEditorList;
}

//==============================================================================

QIntList SedmlEditingViewWidget::editingWidgetSizes() const
{
    // Return our editing widget sizes

    return mEditingWidgetSizes;
}

//==============================================================================

void SedmlEditingViewWidget::splitterMoved()
{
    // We have moved, so keep track of our new sizes

    mEditingWidgetSizes = sizes();
}

//==============================================================================

void SedmlEditingViewWidget::itemRequested(OpenCOR::EditorWidget::EditorListItem *pItem)
{
    // Set our editor's cursor position to the line/column of the given item and
    // give our editor the focus so that we can see the exact location of the
    // item (otherwise it will be mEditorList that will have the focus since we
    // just double-clicked on it)

    mEditor->setCursorPosition(pItem->line()-1, pItem->column()-1);
    mEditor->setFocus();
}

//==============================================================================

}   // namespace SEDMLEditingView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
