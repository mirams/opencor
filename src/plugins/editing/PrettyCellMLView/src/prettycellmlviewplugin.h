/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// PrettyCellMLView plugin
//==============================================================================

#ifndef PRETTYCELLMLVIEWPLUGIN_H
#define PRETTYCELLMLVIEWPLUGIN_H

//==============================================================================

#include "cellmleditinginterface.h"
#include "coreinterface.h"
#include "editinginterface.h"
#include "guiinterface.h"
#include "i18ninterface.h"
#include "plugininfo.h"

//==============================================================================

namespace OpenCOR {
namespace PrettyCellMLView {

//==============================================================================

PLUGININFO_FUNC PrettyCellMLViewPluginInfo();

//==============================================================================

class PrettyCellmlViewWidget;

//==============================================================================

class PrettyCellMLViewPlugin : public QObject, public CellmlEditingInterface,
                               public CoreInterface, public EditingInterface,
                               public GuiInterface, public I18nInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "OpenCOR.PrettyCellMLViewPlugin" FILE "prettycellmlviewplugin.json")

    Q_INTERFACES(OpenCOR::CellmlEditingInterface)
    Q_INTERFACES(OpenCOR::CoreInterface)
    Q_INTERFACES(OpenCOR::EditingInterface)
    Q_INTERFACES(OpenCOR::GuiInterface)
    Q_INTERFACES(OpenCOR::I18nInterface)

public:
    explicit PrettyCellMLViewPlugin();

#include "cellmleditinginterface.inl"
#include "coreinterface.inl"
#include "editinginterface.inl"
#include "guiinterface.inl"
#include "i18ninterface.inl"

private:
    PrettyCellmlViewWidget *mViewWidget;
};

//==============================================================================

}   // namespace PrettyCellMLView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================