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
// Single cell view simulation widget
//==============================================================================

#ifndef SINGLECELLVIEWSIMULATIONWIDGET_H
#define SINGLECELLVIEWSIMULATIONWIDGET_H

//==============================================================================

#include "cellmlfileruntime.h"
#include "corecliutils.h"
#include "singlecellviewgraphpanelplotwidget.h"
#include "singlecellviewsimulation.h"
#include "viewwidget.h"

//==============================================================================

class QFrame;
class QLabel;
class QMenu;
class QSplitter;
class QTextEdit;

//==============================================================================

class QwtWheel;

//==============================================================================

namespace Ui {
    class SingleCellViewSimulationWidget;
}

//==============================================================================

namespace libsedml {
    class SedDocument;
    class SedModel;
    class SedRepeatedTask;
    class SedSimulation;
    class SedVariable;
}   // namespace libsedml

//==============================================================================

namespace OpenCOR {

//==============================================================================

class DataStoreInterface;

//==============================================================================

namespace Core {
    class Property;
    class ProgressBarWidget;
    class ToolBarWidget;
    class UserMessageWidget;
}   // namespace Core

//==============================================================================

namespace SingleCellView {

//==============================================================================

class SingleCellViewContentsWidget;
class SingleCellViewGraphPanelWidget;
class SingleCellViewPlugin;
class SingleCellViewWidget;

//==============================================================================

class SingleCellViewSimulationWidget : public Core::ViewWidget
{
    Q_OBJECT

public:
    explicit SingleCellViewSimulationWidget(SingleCellViewPlugin *pPlugin,
                                            const QString &pFileName,
                                            QWidget *pParent);
    ~SingleCellViewSimulationWidget();

    virtual void retranslateUi();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    void initialize(const bool &pReloadingView = false);
    void finalize();

    void setSizes(const QIntList &pSizes);

    SingleCellViewContentsWidget * contentsWidget() const;

    QIcon fileTabIcon() const;

    bool saveFile(const QString &pOldFileName, const QString &pNewFileName);

    void fileOpened(const QString &pFileName);
    void filePermissionsChanged();
    void fileModified(const QString &pFileName);
    void fileReloaded(const QString &pFileName);
    void fileRenamed(const QString &pOldFileName, const QString &pNewFileName);
    void fileClosed(const QString &pFileName);

    SingleCellViewSimulation *simulation() const;

    void updateSimulationResults(const qulonglong &pSimulationResultsSize);

    static QIcon parameterIcon(const CellMLSupport::CellmlFileRuntimeParameter::ParameterType &pParameterType);

private:
    enum ErrorType {
        General,
        InvalidCellmlFile,
        InvalidSimulationEnvironment
    };

    Ui::SingleCellViewSimulationWidget *mGui;

    SingleCellViewPlugin *mPlugin;

    SingleCellViewWidget *mViewWidget;

    QString mFileName;

    QMap<QAction *, DataStoreInterface *> mDataStoreInterfaces;

    SingleCellViewSimulation *mSimulation;

    Core::ProgressBarWidget *mProgressBarWidget;

    int mProgress;
    bool mLockedDevelopmentMode;

    Core::ToolBarWidget *mToolBarWidget;

    QMenu *mSimulationDataExportDropDownMenu;

    QFrame *mTopSeparator;
    QFrame *mBottomSeparator;

    QwtWheel *mDelayWidget;
    QLabel *mDelayValueWidget;

    QSplitter *mSplitterWidget;

    SingleCellViewContentsWidget *mContentsWidget;

    bool mRunActionEnabled;

    Core::UserMessageWidget *mInvalidModelMessageWidget;

    QTextEdit *mOutputWidget;

    ErrorType mErrorType;

    QMap<SingleCellViewGraphPanelWidget *, SingleCellViewGraphPanelPlotWidget *> mGraphPanelsPlots;
    SingleCellViewGraphPanelPlotWidgets mPlots;
    QMap<SingleCellViewGraphPanelPlotWidget *, QRectF> mPlotsViewports;

    bool mCanUpdatePlotsForUpdatedGraphs;

    bool mNeedReloadView;

    void reloadView();

    void output(const QString &pMessage);

    void updateSimulationMode();

    int tabBarPixmapSize() const;

    void updateRunPauseAction(const bool &pRunActionEnabled);

    void updateDataStoreActions();

    void updateInvalidModelMessageWidget();

    void checkAxisValue(double &pValue, const double &pOrigValue,
                        const QList<double> &pTestValues);

    bool updatePlot(SingleCellViewGraphPanelPlotWidget *pPlot,
                    const bool &pForceReplot = false);

    double * dataPoints(CellMLSupport::CellmlFileRuntimeParameter *pParameter) const;

    void updateGraphData(SingleCellViewGraphPanelPlotGraph *pGraph,
                         const qulonglong &pSize);

    QVariant value(Core::Property *pProperty) const;

    void updateSimulationProperties(OpenCOR::Core::Property *pProperty = 0);
    void updateSolversProperties(Core::Property *pProperty = 0);

    void addSedmlSimulation(libsedml::SedDocument *pSedmlDocument,
                            libsedml::SedModel *pSedmlModel,
                            libsedml::SedRepeatedTask *pSedmlRepeatedTask,
                            libsedml::SedSimulation *pSedmlSimulation,
                            const int &pOrder);
    void addSedmlVariableTarget(libsedml::SedVariable *pSedmlVariable,
                                const QString &pComponent,
                                const QString &pVariable);
    void createSedmlFile(const QString &pFileName, const QString &pModelSource);

    void checkSimulationDataModified(const bool &pIsModified);

Q_SIGNALS:
    void splitterMoved(const QIntList &pSizes);

private Q_SLOTS:
    void on_actionRunPauseResumeSimulation_triggered();
    void on_actionStopSimulation_triggered();

    void on_actionResetModelParameters_triggered();
    void on_actionClearSimulationData_triggered();

    void on_actionDevelopmentMode_triggered();

    void on_actionAddGraphPanel_triggered();

    void on_actionRemoveGraphPanel_triggered();
    void on_actionRemoveCurrentGraphPanel_triggered();
    void on_actionRemoveAllGraphPanels_triggered();

    void on_actionSedmlExportSedmlFile_triggered();
    void on_actionSedmlExportCombineArchive_triggered();

    void emitSplitterMoved();

    void simulationDataExport();

    void updateDelayValue(const double &pDelayValue);

    void simulationRunning(const bool &pIsResuming);
    void simulationPaused();
    void simulationStopped(const qint64 &pElapsedTime);

    void resetProgressBar();
    void resetFileTabIcon();

    void simulationError(const QString &pMessage,
                         const ErrorType &pErrorType = General);

    void simulationDataModified(const bool &pIsModified);

    void simulationPropertyChanged(Core::Property *pProperty);
    void solversPropertyChanged(Core::Property *pProperty);

    void graphPanelAdded(SingleCellViewGraphPanelWidget *pGraphPanel);
    void graphPanelRemoved(SingleCellViewGraphPanelWidget *pGraphPanel);

    void addGraph(CellMLSupport::CellmlFileRuntimeParameter *pParameterX,
                  CellMLSupport::CellmlFileRuntimeParameter *pParameterY);

    void graphAdded(SingleCellViewGraphPanelPlotWidget *pPlot,
                    SingleCellViewGraphPanelPlotGraph *pGraph);
    void graphsRemoved(SingleCellViewGraphPanelPlotWidget *pPlot,
                       const SingleCellViewGraphPanelPlotGraphs &pGraphs);

    void graphsUpdated(SingleCellViewGraphPanelPlotWidget *pPlot,
                       const SingleCellViewGraphPanelPlotGraphs &pGraphs);
};

//==============================================================================

typedef QList<SingleCellViewSimulationWidget *> SingleCellViewSimulationWidgets;

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
