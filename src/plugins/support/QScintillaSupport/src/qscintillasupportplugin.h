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
// QScintillaSupport plugin
//==============================================================================

#ifndef QSCINTILLASUPPORTPLUGIN_H
#define QSCINTILLASUPPORTPLUGIN_H

//==============================================================================

#include "plugininfo.h"

//==============================================================================

namespace OpenCOR {
namespace QScintillaSupport {

//==============================================================================

PLUGININFO_FUNC QScintillaSupportPluginInfo();

//==============================================================================

class QScintillaSupportPlugin : public QObject
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "OpenCOR.QScintillaSupportPlugin" FILE "qscintillasupportplugin.json")
};

//==============================================================================

}   // namespace QScintillaSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================