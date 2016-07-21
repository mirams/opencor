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

#include <QDialog>

//==============================================================================

namespace Ui {
    class BioSignalMLSelectVariables;
}

//==============================================================================

namespace OpenCOR {
namespace BioSignalMLDataStore {

//==============================================================================

class BioSignalMLSelectVariables : public QDialog
{
    Q_OBJECT

public:
    explicit BioSignalMLSelectVariables(const QVector<QString> &pLabels,
                                        const QVector<bool> &pChecked,
                                        QWidget *pParent);
    ~BioSignalMLSelectVariables();

    virtual void retranslateUi();
    bool checked(size_t pIndex);

private slots:
    void selectAllVariables(bool checked = false);

private:
    Ui::BioSignalMLSelectVariables *mGui;
};

//==============================================================================

}   // namespace BioSignalMLDataStore
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
