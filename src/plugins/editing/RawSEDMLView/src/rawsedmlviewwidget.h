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
// Raw SED-ML view widget
//==============================================================================

#pragma once

//==============================================================================

#include "corecliutils.h"
#include "viewwidget.h"

//==============================================================================

#include <QMap>

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace SEDMLEditingView {
    class SedmlEditingViewWidget;
}   // namespace SEDMLEditingView

//==============================================================================

namespace EditorWidget {
    class EditorWidget;
}   // namespace EditorWidget

//==============================================================================

namespace RawSEDMLView {

//==============================================================================

class RawSedmlViewWidget : public Core::ViewWidget
{
    Q_OBJECT

public:
    explicit RawSedmlViewWidget(QWidget *pParent);

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    virtual void retranslateUi();

    bool contains(const QString &pFileName) const;

    void initialize(const QString &pFileName, const bool &pUpdate = true);
    void finalize(const QString &pFileName);

    void fileReloaded(const QString &pFileName);
    void fileRenamed(const QString &pOldFileName, const QString &pNewFileName);

    EditorWidget::EditorWidget * editorWidget(const QString &pFileName) const;

    virtual QList<QWidget *> statusBarWidgets() const;

    void reformat(const QString &pFileName);

    bool validate(const QString &pFileName,
                  const bool &pOnlyErrors = false) const;

private:
    bool mNeedLoadingSettings;
    QString mSettingsGroup;

    SEDMLEditingView::SedmlEditingViewWidget *mEditingWidget;
    QMap<QString, SEDMLEditingView::SedmlEditingViewWidget *> mEditingWidgets;
};

//==============================================================================

}   // namespace RawSEDMLView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
