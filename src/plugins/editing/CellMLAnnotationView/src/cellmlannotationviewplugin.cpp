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
// CellMLAnnotationView plugin
//==============================================================================

#include "cellmlannotationvieweditingwidget.h"
#include "cellmlannotationviewplugin.h"
#include "cellmlannotationviewwidget.h"
#include "cellmlfilemanager.h"
#include "cellmlsupportplugin.h"

//==============================================================================

#include <QMainWindow>

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

PLUGININFO_FUNC CellMLAnnotationViewPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to annotate CellML files."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour annoter des fichiers CellML."));

    return new PluginInfo(PluginInfo::Editing, true,
                          QStringList() << "CoreCellMLEditing",
                          descriptions);
}

//==============================================================================

CellMLAnnotationViewPlugin::CellMLAnnotationViewPlugin()
{
    // Set our settings

    mGuiSettings->setView(GuiViewSettings::Editing,
                          QStringList() << CellMLSupport::CellmlMimeType);
}

//==============================================================================
// Core interface
//==============================================================================

void CellMLAnnotationViewPlugin::initialize()
{
    // Create our CellML annotation view widget

    mViewWidget = new CellmlAnnotationViewWidget(this, mMainWindow);

    // Hide our CellML annotation view widget since it may not initially be
    // shown in our central widget

    mViewWidget->setVisible(false);
}

//==============================================================================

void CellMLAnnotationViewPlugin::finalize()
{
    // We don't handle this interface...
}

//==============================================================================

void CellMLAnnotationViewPlugin::initialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void CellMLAnnotationViewPlugin::loadSettings(QSettings *pSettings)
{
    // Retrieve our CellML annotation view widget settings

    loadViewSettings(pSettings, mViewWidget);
}

//==============================================================================

void CellMLAnnotationViewPlugin::saveSettings(QSettings *pSettings) const
{
    // Keep track of our CellML annotation view widget settings

    saveViewSettings(pSettings, mViewWidget);
}

//==============================================================================

void CellMLAnnotationViewPlugin::settingsLoaded(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void CellMLAnnotationViewPlugin::handleArguments(const QStringList &pArguments)
{
    Q_UNUSED(pArguments);

    // We don't handle this interface...
}

//==============================================================================

void CellMLAnnotationViewPlugin::handleAction(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================
// Editing interface
//==============================================================================

QScintillaSupport::QScintillaWidget * CellMLAnnotationViewPlugin::editor(const QString &pFileName) const
{
    Q_UNUSED(pFileName);

    // Return the requested editor, i.e. nothing since we don't have an editor
    // as such

    return 0;
}

//==============================================================================
// GUI interface
//==============================================================================

void CellMLAnnotationViewPlugin::changeEvent(QEvent *pEvent)
{
    Q_UNUSED(pEvent);

    // We don't handle this interface...
}

//==============================================================================

void CellMLAnnotationViewPlugin::updateGui(Plugin *pViewPlugin,
                                           const QString &pFileName)
{
    Q_UNUSED(pViewPlugin);
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void CellMLAnnotationViewPlugin::initializeView()
{
    // We don't handle this interface...
}

//==============================================================================

void CellMLAnnotationViewPlugin::finalizeView()
{
    // We don't handle this interface...
}

//==============================================================================

bool CellMLAnnotationViewPlugin::hasViewWidget(const QString &pFileName)
{
    // Return whether we know about the given CellML file

    return mViewWidget->contains(pFileName);
}

//==============================================================================

QWidget * CellMLAnnotationViewPlugin::viewWidget(const QString &pFileName,
                                                 const bool &pCreate)
{
    // Make sure that we are dealing with a CellML file

    if (!CellMLSupport::CellmlFileManager::instance()->cellmlFile(pFileName))
        return 0;

    // We are dealing with a CellML file, so update our CellML annotation view
    // widget using the given CellML file

    if (pCreate) {
        mViewWidget->initialize(pFileName);

        return mViewWidget;
    } else {
        return 0;
    }
}

//==============================================================================

void CellMLAnnotationViewPlugin::removeViewWidget(const QString &pFileName)
{
    // Ask our CellML annotation view widget to finalise the given CellML file

    mViewWidget->finalize(pFileName);
}

//==============================================================================

QString CellMLAnnotationViewPlugin::viewName() const
{
    // Return our CellML annotation view's name

    return tr("CellML Annotation");
}

//==============================================================================

QIcon CellMLAnnotationViewPlugin::fileTabIcon(const QString &pFileName) const
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...

    return QIcon();
}

//==============================================================================

bool CellMLAnnotationViewPlugin::saveFile(const QString &pOldFileName,
                                          const QString &pNewFileName)
{
    // Ask our CellML annotation view widget to save the given file

    CellmlAnnotationViewEditingWidget *editingWidget = mViewWidget->editingWidget(pOldFileName);

    return editingWidget?editingWidget->cellmlFile()->save(pNewFileName):false;
}

//==============================================================================

void CellMLAnnotationViewPlugin::fileOpened(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void CellMLAnnotationViewPlugin::filePermissionsChanged(const QString &pFileName)
{
    // The given file has been un/locked, so retranslate ourselves (since some
    // messages may be locked-dependent)
    // Note: our plugin is such that retranslating it will update the GUI (since
    //       it was easier/faster to do it that way), so all we had to do was to
    //       make those updateGui() methods locked-dependent...

    if (hasViewWidget(pFileName))
        retranslateUi();
}

//==============================================================================

void CellMLAnnotationViewPlugin::fileModified(const QString &pFileName,
                                              const bool &pModified)
{
    Q_UNUSED(pFileName);
    Q_UNUSED(pModified);

    // We don't handle this interface...
}

//==============================================================================

void CellMLAnnotationViewPlugin::fileReloaded(const QString &pFileName)
{
    // The given file has been reloaded, so let our view widget know about it

    mViewWidget->fileReloaded(pFileName);
}

//==============================================================================

void CellMLAnnotationViewPlugin::fileRenamed(const QString &pOldFileName,
                                             const QString &pNewFileName)
{
    // The given file has been renamed, so let our view widget know about it

    mViewWidget->fileRenamed(pOldFileName, pNewFileName);
}

//==============================================================================

void CellMLAnnotationViewPlugin::fileClosed(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

bool CellMLAnnotationViewPlugin::canClose()
{
    // We don't handle this interface...

    return true;
}

//==============================================================================
// I18n interface
//==============================================================================

void CellMLAnnotationViewPlugin::retranslateUi()
{
    // Retranslate our CellML annotation view widget

    mViewWidget->retranslateUi();
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
