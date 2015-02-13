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
// Parser for the CellML text format
//==============================================================================

#include "cellmltextviewparser.h"
#include "cellmltextviewscanner.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLTextView {

//==============================================================================

CellmlTextViewParser::CellmlTextViewParser(const QString &pText) :
    mScanner(new CellmlTextViewScanner(pText)),
    mDomDocument(QDomDocument(QString()))
{
    mValid = true;
}

//==============================================================================

CellmlTextViewParser::~CellmlTextViewParser()
{
    // Delete some internal objects

    delete mScanner;
}

//==============================================================================

bool CellmlTextViewParser::isValid() const
{
    // Return whether we are valid

    return mValid;
}

//==============================================================================

QDomDocument CellmlTextViewParser::domDocument() const
{
    // Return whether our DOM document

    return mDomDocument;
}

//==============================================================================

}   // namespace CellMLTextView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================