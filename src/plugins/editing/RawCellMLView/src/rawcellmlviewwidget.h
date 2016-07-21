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
// Raw CellML view widget
//==============================================================================

#pragma once

//==============================================================================

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
    class EditorWidget;
}   // namespace EditorWidget

//==============================================================================

namespace RawCellMLView {

//==============================================================================

class RawCellmlViewWidget : public Core::ViewWidget
{
    Q_OBJECT

public:
    explicit RawCellmlViewWidget(QWidget *pParent);

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

    virtual QList<QWidget *> statusBarWidgets() const;

    void reformat(const QString &pFileName);

    bool validate(const QString &pFileName,
                  const bool &pOnlyErrors = false) const;

private:
    bool mNeedLoadingSettings;
    QString mSettingsGroup;

    CellMLEditingView::CellmlEditingViewWidget *mEditingWidget;
    QMap<QString, CellMLEditingView::CellmlEditingViewWidget *> mEditingWidgets;

    QMap<QString, QString> mPresentationMathmlEquations;

    Core::MathmlConverter mMathmlConverter;

    QString mContentMathmlEquation;

    QString retrieveContentMathmlEquation(const QString &pContentMathmlBlock,
                                          const int &pPosition) const;

private slots:
    void updateViewer();

    void mathmlConversionDone(const QString &pContentMathml,
                              const QString &pPresentationMathml);
};

//==============================================================================

}   // namespace RawCellMLView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
