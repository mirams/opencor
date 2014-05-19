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
// SampleView plugin
//==============================================================================

#include "sampleviewplugin.h"

//==============================================================================

namespace OpenCOR {
namespace SampleView {

//==============================================================================

PLUGININFO_FUNC SampleViewPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin that provides a test view."));
    descriptions.insert("fr", QString::fromUtf8("une extension qui fournit une vue de test."));

    return new PluginInfo(PluginInfo::Sample, true, false,
                          QStringList() << "Core" << "Sample",
                          descriptions);
}

//==============================================================================

}   // namespace SampleWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================