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
// CellML Text view widget
//==============================================================================

#pragma once

//==============================================================================

#include "cellmlfile.h"
#include "cellmltextviewconverter.h"
#include "cellmltextviewparser.h"
#include "corecliutils.h"
#include "mathmlconverter.h"
#include "viewwidget.h"

//==============================================================================

#include <QMap>

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace CellMLEditingView {
    class CellmlEditingViewWidget;
}   // namespace CellMLEditingView

//==============================================================================

namespace EditorWidget {
    class EditorListWidget;
    class EditorWidget;
}   // namespace EditorWidget

//==============================================================================

namespace QScintillaSupport {
    class QScintillaWidget;
}   // namespace QScintillaSupport

//==============================================================================

namespace CellMLTextView {

//==============================================================================

class CellmlTextViewWidgetData
{
public:
    explicit CellmlTextViewWidgetData(CellMLEditingView::CellmlEditingViewWidget *pEditingWidget,
                                      const QString &pSha1, const bool &pValid,
                                      const CellMLSupport::CellmlFile::Version &pCellmlVersion,
                                      QDomDocument pRdfNodes);
    ~CellmlTextViewWidgetData();

    void retranslateUi();

    CellMLEditingView::CellmlEditingViewWidget * editingWidget() const;

    QString sha1() const;
    void setSha1(const QString &pSha1);

    bool isValid() const;

    CellMLSupport::CellmlFile::Version cellmlVersion() const;
    void setCellmlVersion(const CellMLSupport::CellmlFile::Version &pCellmlVersion);

    QDomDocument rdfNodes() const;

private:
    CellMLEditingView::CellmlEditingViewWidget *mEditingWidget;
    QString mSha1;
    bool mValid;
    CellMLSupport::CellmlFile::Version mCellmlVersion;
    QDomDocument mRdfNodes;
};

//==============================================================================

class CellmlTextViewWidget : public Core::ViewWidget
{
    Q_OBJECT

public:
    explicit CellmlTextViewWidget(QWidget *pParent);

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    virtual void retranslateUi();

    bool contains(const QString &pFileName) const;

    void initialize(const QString &pFileName, const bool &pUpdate = true);
    void finalize(const QString &pFileName);

    void fileReloaded(const QString &pFileName);
    void fileRenamed(const QString &pOldFileName, const QString &pNewFileName);

    EditorWidget::EditorWidget * editorWidget(const QString &pFileName) const;
    CellMLEditingView::CellmlEditingViewWidget * editingWidget(const QString &pFileName) const;

    bool isEditorWidgetUseable(const QString &pFileName) const;
    bool isEditorWidgetContentsModified(const QString &pFileName) const;

    bool saveFile(const QString &pOldFileName, const QString &pNewFileName,
                  bool &pNeedFeedback);

    virtual QList<QWidget *> statusBarWidgets() const;

    void reformat(const QString &pFileName);

    bool validate(const QString &pFileName);

private:
    bool mNeedLoadingSettings;
    QString mSettingsGroup;

    CellMLEditingView::CellmlEditingViewWidget *mEditingWidget;

    QMap<QString, CellmlTextViewWidgetData *> mData;

    CellMLTextViewConverter mConverter;
    CellmlTextViewParser mParser;

    QList<EditorWidget::EditorListWidget *> mEditorLists;

    QMap<QString, QString> mPresentationMathmlEquations;

    Core::MathmlConverter mMathmlConverter;

    QString mContentMathmlEquation;

    void commentOrUncommentLine(QScintillaSupport::QScintillaWidget *pEditorWidget,
                                const int &pLineNumber,
                                const bool &pCommentLine);

    bool parse(const QString &pFileName, const bool &pOnlyErrors = false);

    QString partialStatement(const int &pPosition, int &pFromPosition,
                             int &pToPosition) const;
    QString beginningOfPiecewiseStatement(int &pPosition) const;
    QString endOfPiecewiseStatement(int &pPosition) const;
    QString statement(const int &pPosition) const;

private slots:
    void editorKeyPressed(QKeyEvent *pEvent, bool &pHandled);

    void updateViewer();

    void selectFirstItemInEditorList(EditorWidget::EditorListWidget *pEditorList = 0);

    void mathmlConversionDone(const QString &pContentMathml,
                              const QString &pPresentationMathml);
};

//==============================================================================

}   // namespace CellMLTextView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
