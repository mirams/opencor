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
// Sample window
//==============================================================================

#pragma once

//==============================================================================

#include "windowwidget.h"

//==============================================================================

namespace Ui {
    class SampleWindowWindow;
}

//==============================================================================

namespace OpenCOR {
namespace SampleWindow {

//==============================================================================

class SampleWindowWindow : public Core::WindowWidget
{
    Q_OBJECT

public:
    explicit SampleWindowWindow(QWidget *pParent);
    ~SampleWindowWindow();

private:
    Ui::SampleWindowWindow *mGui;

private slots:
    void updateSum();
};

//==============================================================================

}   // namespace SampleWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
