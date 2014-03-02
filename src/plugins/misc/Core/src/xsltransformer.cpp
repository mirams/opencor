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
// XSL transformation
//==============================================================================

#include "cliutils.h"
#include "xsltransformer.h"

//==============================================================================

#include <QAbstractMessageHandler>
#include <QThread>
#include <QXmlQuery>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

class DummyMessageHandler : public QAbstractMessageHandler
{
protected:
    virtual void handleMessage(QtMsgType pType, const QString &pDescription,
                               const QUrl &pIdentifier,
                               const QSourceLocation &pSourceLocation)
    {
        Q_UNUSED(pType);
        Q_UNUSED(pDescription);
        Q_UNUSED(pIdentifier);
        Q_UNUSED(pSourceLocation);

        // We ignore the message...
    }
};

//==============================================================================

XslTransformer::XslTransformer(const QString &pInput, const QString &pXsl) :
    QObject(),
    mInput(pInput),
    mXsl(pXsl),
    mOutputReady(false),
    mOutput(QString())
{
    // Create our thread
    // Note: XSL transformation requires using a QXmlQuery object. Howwever,
    //       QXmlQuery is not thread-safe, hence we create a thread and move
    //       ourselves to it...

    mThread = new QThread();

    // Move ourselves to our thread

    moveToThread(mThread);

    // Create a few connections

    connect(mThread, SIGNAL(started()),
            this, SLOT(doTransformation()));

    connect(mThread, SIGNAL(finished()),
            mThread, SLOT(deleteLater()));
    connect(mThread, SIGNAL(finished()),
            this, SLOT(deleteLater()));

    // Start our thread, i.e. do our transformation

    mThread->start();
}

//==============================================================================

bool XslTransformer::outputReady() const
{
    // Return whether our output is ready

    return mOutputReady;
}

//==============================================================================

QString XslTransformer::output() const
{
    // Return the result of our XSL transformation

    return mOutput;
}

//==============================================================================

void XslTransformer::doTransformation()
{
    // Create and customise our XML query object

    QXmlQuery *xmlQuery = new QXmlQuery(QXmlQuery::XSLT20);
    DummyMessageHandler *dummyMessageHandler = new DummyMessageHandler();

    xmlQuery->setMessageHandler(dummyMessageHandler);

    xmlQuery->setFocus(mInput);
    xmlQuery->setQuery(mXsl);

    // Do the XSL tranformation

    xmlQuery->evaluateTo(&mOutput);

    // We are done

    delete xmlQuery;
    delete dummyMessageHandler;

    mOutputReady = true;
}

//==============================================================================

QString XslTransformer::transform(const QString &pInput, const QString &pXsl)
{
    XslTransformer xslTransformer(pInput, pXsl);

    while (!xslTransformer.outputReady())
        doNothing(1);

    return xslTransformer.output();
}

//==============================================================================

QString contentMathmlToPresentationMathml(const QString &pContentMathml)
{
    // Transform the given content MathML to presentation MathML and return it

    static const QString CtopXsl = resourceAsByteArray(":/ctop.xsl");

    return XslTransformer::transform(pContentMathml, CtopXsl);
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================