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
// Some useful tests-related functions
//==============================================================================

#include "plugin.h"
#include "testsutils.h"

//==============================================================================

#include <QtTest/QtTest>

//==============================================================================

namespace OpenCOR {

//==============================================================================

QStringList fileContents(const QString &pFileName)
{
    // Read and return the contents of the given file

    QFile file(pFileName);
    QString contents = QString();

    if (file.open(QIODevice::ReadOnly|QIODevice::Text)) {
        contents = QTextStream(&file).readAll();

        file.close();
    }

    return contents.replace("\r", "").split("\n");
}

//==============================================================================

QStringList runCli(const QStringList pArguments)
{
    // Execute the CLI version of OpenCOR (passing to it the given arguments)
    // and return the output it has generated, if any

#if defined(Q_OS_WIN)
    QString program = "bin/OpenCOR.com";
#elif defined(Q_OS_LINUX)
    QString program = "bin/OpenCOR";
#elif defined(Q_OS_MAC)
    QString program = "OpenCOR.app/Contents/MacOS/OpenCOR";
#else
    #error Unsupported platform
#endif

    QProcess process;

    process.setProcessChannelMode(QProcess::MergedChannels);

    QStringList defaultArguments = QStringList();

#ifdef Q_OS_LINUX
    defaultArguments << "-platform" << "offscreen";
#endif

    process.start(program, defaultArguments << pArguments);

    QString output = QString();

    while (process.waitForReadyRead())
        output += process.readAll();

    return output.replace("\r", "").split("\n");
}

//==============================================================================

void loadPlugin(const QString &pPluginName)
{
    // Load the given plugin

    QStringList plugins = QStringList() << Plugin::requiredPlugins(".", pPluginName)
                                        << pPluginName;

    foreach (const QString &plugin, plugins) {
        QPluginLoader pluginLoader(PluginPrefix+plugin+PluginExtension);

        QVERIFY2(pluginLoader.load(),
                 qPrintable(QString("the %1 plugin could not be loaded").arg(plugin)));
    }
}

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================