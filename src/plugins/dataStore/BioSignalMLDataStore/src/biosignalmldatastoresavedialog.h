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
// Save as BioSignalML widget
//==============================================================================

#pragma once

//==============================================================================

#include "datastoreinterface.h"

//==============================================================================

#include <QDialog>
#include <QVector>

//==============================================================================

namespace Ui {
    class BioSignalMLSaveDialog;
}

//==============================================================================

namespace OpenCOR {
namespace BioSignalMLDataStore {

//==============================================================================

class BiosignalmlSaveDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BiosignalmlSaveDialog(QWidget *pParent);
    ~BiosignalmlSaveDialog();

    virtual void retranslateUi();

    bool run();

    void setDefaultFileName(const QString &pFileName);
    QString fileName() const;
    QString shortName() const;
    QString description() const;
    QString author() const;
    void setComment(const QString &pComment);
    void setSelectedVariables(const DataStore::DataStoreVariables &pVariables);
    QVector<bool> selectedVariables() const;
    bool selectedVariable(const size_t pIndex) const;

private slots:
    void accepted();
    void setFileName(bool checked = false);
    void selectVariables(bool checked = false);

private:
    void setButtonStates() const;

    Ui::BioSignalMLSaveDialog *mGui;

    QString mDefaultFileName;
    QVector<QString> mVariableLabels;
    QVector<bool> mSelectedVariables;
    bool mGotSignals;
    bool mAccepted;
};

//==============================================================================

}   // namespace BioSignalMLDataStore
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
