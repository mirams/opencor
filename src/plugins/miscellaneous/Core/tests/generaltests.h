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
// Core general tests
//==============================================================================

#pragma once

//==============================================================================

#include <QObject>

//==============================================================================

class GeneralTests : public QObject
{
    Q_OBJECT

private:
    QString mString;
    QString mEol;

private slots:
    void initTestCase();

    void qSameStringListsTests();
    void sizeAsStringTests();
    void sha1Tests();
    void stringPositionAsLineColumnTests();
    void stringLineColumnAsPositionTests();
    void newFileNameTests();
};

//==============================================================================
// End of file
//==============================================================================
