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
// View widget
//==============================================================================

#pragma once

//==============================================================================

#include "busysupportwidget.h"
#include "coreglobal.h"
#include "widget.h"

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

class CORE_EXPORT ViewWidget : public Widget, public BusySupportWidget
{
    Q_OBJECT

public:
    explicit ViewWidget(QWidget *pParent);

    virtual QList<QWidget *> statusBarWidgets() const;

protected:
    void resizeEvent(QResizeEvent *pEvent);

signals:
    void updateFileTabIcon(const QString &pViewName, const QString &pFileName,
                           const QIcon &pIcon);
};

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
