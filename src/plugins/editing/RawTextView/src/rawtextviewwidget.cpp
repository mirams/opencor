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
// Raw Text view widget
//==============================================================================

#include "corecliutils.h"
#include "editorwidget.h"
#include "filemanager.h"
#include "rawtextviewwidget.h"

//==============================================================================

#include <QLabel>
#include <QLayout>
#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace RawTextView {

//==============================================================================

RawTextViewWidget::RawTextViewWidget(QWidget *pParent) :
    ViewWidget(pParent),
    mNeedLoadingSettings(true),
    mSettingsGroup(QString()),
    mEditor(0),
    mEditors(QMap<QString, EditorWidget::EditorWidget *>())
{
}

//==============================================================================

void RawTextViewWidget::loadSettings(QSettings *pSettings)
{
    // Normally, we would retrieve the editing widget's settings, but
    // mEditingWidget is not set at this stage. So, instead, we keep track of
    // our settings' group and load them when initialising ourselves (see
    // initialize())...

    mSettingsGroup = pSettings->group();
}

//==============================================================================

void RawTextViewWidget::saveSettings(QSettings *pSettings) const
{
    Q_UNUSED(pSettings);
    // Note: our view is such that our settings are actually saved when calling
    //       finalize() on the last file...
}

//==============================================================================

void RawTextViewWidget::retranslateUi()
{
    // Retranslate our viewer and editor

    if (mEditor)
        mEditor->retranslateUi();
}

//==============================================================================

bool RawTextViewWidget::contains(const QString &pFileName) const
{
    // Return whether we know about the given file

    return mEditors.contains(pFileName);
}

//==============================================================================

void RawTextViewWidget::initialize(const QString &pFileName,
                                   const bool &pUpdate)
{
    // Retrieve the editor associated with the given file, if any

    EditorWidget::EditorWidget *newEditor = mEditors.value(pFileName);

    if (!newEditor) {
        // No editor exists for the given file, so create one

        QByteArray fileContents;

        Core::readFileContentsFromFile(pFileName, fileContents);

        newEditor = new EditorWidget::EditorWidget(fileContents,
                                                   !Core::FileManager::instance()->isReadableAndWritable(pFileName),
                                                   0, parentWidget());

        // Keep track of our editor

        mEditors.insert(pFileName, newEditor);
    }

    // Update our editor, if required

    if (pUpdate) {
        EditorWidget::EditorWidget *oldEditor = mEditor;

        mEditor = newEditor;

        // Load our settings, if needed, or reset our editing widget using the
        // 'old' one

        if (mNeedLoadingSettings) {
            QSettings settings;

            settings.beginGroup(mSettingsGroup);
                newEditor->loadSettings(&settings);
            settings.endGroup();

            mNeedLoadingSettings = false;
        } else {
            newEditor->updateSettings(oldEditor);
        }

        // Set our focus proxy to our 'new' editor and make sure that the latter
        // immediately gets the focus
        // Note: if we were not to immediately give the focus to our 'new'
        //       editor, then the central widget would give the focus to our
        //       'old' editor (see CentralWidget::updateGui()), which is clearly
        //       not what we want...

        setFocusProxy(newEditor);

        newEditor->setFocus();
    } else {
        // Hide our 'new' editor

        newEditor->hide();
    }
}

//==============================================================================

void RawTextViewWidget::finalize(const QString &pFileName)
{
    // Remove the editor, should there be one for the given file

    EditorWidget::EditorWidget *editor  = mEditors.value(pFileName);

    if (editor) {
        // There is an editor for the given file name, so save our settings and
        // reset our memory of the current editor, if needed

        if (mEditor == editor) {
            QSettings settings;

            settings.beginGroup(mSettingsGroup);
                editor->saveSettings(&settings);
            settings.endGroup();

            mNeedLoadingSettings = true;
            mEditor = 0;
        }

        // Delete the editor and remove it from our list

        delete editor;

        mEditors.remove(pFileName);
    }
}

//==============================================================================

void RawTextViewWidget::fileReloaded(const QString &pFileName)
{
    // The given file has been reloaded, so reload it, should it be managed
    // Note: if the view for the given file is not the active view, then to call
    //       finalize() and then initialize() would activate the contents of the
    //       view (but the file tab would still point to the previously active
    //       file). However, we want to the 'old' file to remain the active one,
    //       hence the extra argument we pass to initialize()...

    if (contains(pFileName)) {
        bool update = mEditor == mEditors.value(pFileName);

        finalize(pFileName);
        initialize(pFileName, update);
    }
}

//==============================================================================

void RawTextViewWidget::fileRenamed(const QString &pOldFileName,
                                    const QString &pNewFileName)
{
    // The given file has been renamed, so update our editors mapping

    EditorWidget::EditorWidget *editor = mEditors.value(pOldFileName);

    if (editor) {
        mEditors.insert(pNewFileName, editor);
        mEditors.remove(pOldFileName);
    }
}

//==============================================================================

EditorWidget::EditorWidget * RawTextViewWidget::editorWidget(const QString &pFileName) const
{
    // Return the requested editor widget

    return mEditors.value(pFileName);
}

//==============================================================================

QList<QWidget *> RawTextViewWidget::statusBarWidgets() const
{
    // Return our status bar widgets

    if (mEditor) {
        return QList<QWidget *>() << mEditor->cursorPositionWidget()
                                  << mEditor->editingModeWidget();
    } else {
        return QList<QWidget *>();
    }
}

//==============================================================================

}   // namespace RawTextView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
