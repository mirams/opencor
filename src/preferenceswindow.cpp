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
// Preferences window
//==============================================================================

#include "preferenceswindow.h"

//==============================================================================

#include "ui_preferenceswindow.h"

//==============================================================================

namespace OpenCOR {

//==============================================================================

PreferencesWindow::PreferencesWindow(QWidget *pParent) :
    QDialog(pParent),
    mGui(new Ui::PreferencesWindow)
{
    // Set up the GUI

    mGui->setupUi(this);
}

//==============================================================================

PreferencesWindow::~PreferencesWindow()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
