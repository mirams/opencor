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
// Editor widget
//==============================================================================

#pragma once

//==============================================================================

#include "corecliutils.h"
#include "editorwidgetglobal.h"
#include "widget.h"

//==============================================================================

#include <QColor>
#include <QString>

//==============================================================================

class QAction;
class QFrame;
class QLabel;
class QMenu;

//==============================================================================

class QsciLexer;

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace QScintillaSupport {
    class QScintillaWidget;
}   // namespace QScintillaSupport

//==============================================================================

namespace EditorWidget {

//==============================================================================

class EditorWidgetFindReplaceWidget;

//==============================================================================

class EDITORWIDGET_EXPORT EditorWidget : public Core::Widget
{
    Q_OBJECT

public:
    explicit EditorWidget(const QString &pContents, const bool &pReadOnly,
                          QsciLexer *pLexer, QWidget *pParent);

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    virtual void retranslateUi();

    void updateSettings(EditorWidget *pEditorWidget);

    QScintillaSupport::QScintillaWidget * editor() const;

    QMenu * contextMenu() const;
    void setContextMenu(const QList<QAction *> &pContextMenuActions);

    void cursorPosition(int &pLine, int &pColumn);
    void setCursorPosition(const int &pLine, const int &pColumn);

    int currentPosition() const;

    QString contents() const;
    void setContents(const QString &pContents,
                     const bool &pKeepHistory = false);

    int contentsSize() const;

    bool isReadOnly() const;
    void setReadOnly(const bool &pReadOnly);

    bool hasSelectedText() const;
    QString selectedText() const;

    QString textInRange(const int &pStartRange, const int &pEndRange) const;
    int findTextInRange(const int &pStartRange, const int &pEndRange,
                        const QString &pText, const bool &pRegularExpression,
                        const bool &pCaseSensitive, const bool &pWholeWordsOnly) const;

    bool isUndoAvailable() const;
    bool isRedoAvailable() const;

    bool isFindPreviousNextAvailable() const;

    bool isSelectAllAvailable() const;

    void cut();
    void copy();
    void paste();
    void del();

    void undo();
    void redo();

    void selectAll();

    void resetUndoHistory();

    QLabel * cursorPositionWidget() const;
    QLabel * editingModeWidget() const;

    QString eolString() const;

    int zoomLevel() const;
    void setZoomLevel(const int &pZoomLevel);

    bool findReplaceIsVisible() const;
    void setFindReplaceVisible(const bool &pVisible);

    int styleAt(const int &pPosition) const;

private:
    QScintillaSupport::QScintillaWidget *mEditor;
    QFrame *mSeparator;
    EditorWidgetFindReplaceWidget *mFindReplace;

    int mCurrentLine;
    int mCurrentColumn;

    bool mFindReplaceVisible;

    QIntList mReadOnlyStyles;

    EditorWidgetFindReplaceWidget * findReplace();

    bool findText(const QString &pText, const bool &pForward);

signals:
    void zoomLevelChanged(const int &pZoomLevel);

    void cursorPositionChanged(const int &pRow, const int &pColumn);

    void textChanged();

    void copyAvailable(const bool &pCopyAvailable);

    void canFindReplace(const bool &pCanFindReplace);

    void canSelectAll(const bool &pCanSelectAll);

public slots:
    bool findPrevious();
    bool findNext();

    void replace();
    void replaceAndFind();
    void replaceAll();

private slots:
    void zoomLevelChanged();

    void keepTrackOfCursorPosition(const int &pLine, const int &pColumn);

    void editorKeyPressed(QKeyEvent *pEvent, bool &pHandled);
    void findReplaceKeyPressed(QKeyEvent *pEvent, bool &pHandled);

    void findTextChanged(const QString &pText);
};

//==============================================================================

}   // namespace EditorWidget
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
