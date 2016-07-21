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
// Editing view plugin
//==============================================================================

#pragma once

//==============================================================================

#include "filehandlinginterface.h"
#include "guiinterface.h"
#include "i18ninterface.h"
#include "plugininfo.h"
#include "plugininterface.h"

//==============================================================================

namespace OpenCOR {

//==============================================================================

class EditingViewInterface;

//==============================================================================

namespace EditorWidget {
    class EditorWidget;
}   // namespace EditorWidget

//==============================================================================

namespace EditingView {

//==============================================================================

PLUGININFO_FUNC EditingViewPluginInfo();

//==============================================================================

class EditingViewPlugin : public QObject, public FileHandlingInterface,
                          public GuiInterface, public I18nInterface,
                          public PluginInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "OpenCOR.EditingViewPlugin" FILE "editingviewplugin.json")

    Q_INTERFACES(OpenCOR::FileHandlingInterface)
    Q_INTERFACES(OpenCOR::GuiInterface)
    Q_INTERFACES(OpenCOR::I18nInterface)
    Q_INTERFACES(OpenCOR::PluginInterface)

public:
    explicit EditingViewPlugin();

#include "filehandlinginterface.inl"
#include "guiinterface.inl"
#include "i18ninterface.inl"
#include "plugininterface.inl"

private:
    EditingViewInterface *mEditingViewInterface;

    QMenu *mEditMenu;

    QAction *mEditUndoAction;
    QAction *mEditRedoAction;

    QAction *mEditCutAction;
    QAction *mEditCopyAction;
    QAction *mEditPasteAction;
    QAction *mEditDeleteAction;

    QAction *mEditFindReplaceAction;
    QAction *mEditFindNextAction;
    QAction *mEditFindPreviousAction;

    QAction *mEditSelectAllAction;

    EditorWidget::EditorWidget *mEditor;

    QString mFileName;

    void updateGui(const QString &pFileName);

    void unpopulateEditMenu();
    void populateEditMenu(QAction *pBeforeAction = 0);

private slots:
    void clipboardDataChanged();

    void updateUndoAndRedoActions();
    void updateEditingActions();
    void updateFindPreviousNextActions();
    void updateSelectAllAction();

    void doUndo();
    void doRedo();

    void doCut();
    void doCopy();
    void doPaste();
    void doDelete();

    void doFindReplace();
    void doFindNext();
    void doFindPrevious();

    void doSelectAll();
};

//==============================================================================

}   // namespace EditingView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
