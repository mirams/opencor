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
// CellML file class
//==============================================================================

#ifndef CELLMLFILE_H
#define CELLMLFILE_H

//==============================================================================

#include "cellmlfileissue.h"
#include "cellmlfilerdftriple.h"
#include "cellmlfileruntime.h"
#include "cellmlsupportglobal.h"

//==============================================================================

#include <QObject>

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlFile : public QObject
{
    Q_OBJECT

public:
    enum Format {
        Cellml_1_0,
        Cellml_1_1
    };

    explicit CellmlFile(const QString &pFileName);
    ~CellmlFile();

    iface::cellml_api::Model * model();

    iface::rdf_api::DataSource * rdfDataSource();

    bool load();
    bool reload();

    bool save(const QString &pNewFileName = QString());

    bool isValid();

    bool isModified() const;
    void setModified(const bool &pModified) const;

    CellmlFileIssues issues() const;

    CellmlFileRuntime * runtime();

    QString fileName() const;

    CellmlFileRdfTriples & rdfTriples();
    CellmlFileRdfTriples rdfTriples(iface::cellml_api::CellMLElement *pElement) const;

    bool rdfTripleExists(iface::cellml_api::CellMLElement *pElement,
                         const CellMLSupport::CellmlFileRdfTriple::ModelQualifier &pModelQualifier,
                         const QString &pResource, const QString &pId) const;
    bool rdfTripleExists(iface::cellml_api::CellMLElement *pElement,
                         const CellMLSupport::CellmlFileRdfTriple::BioQualifier &pBioQualifier,
                         const QString &pResource, const QString &pId) const;

    CellMLSupport::CellmlFileRdfTriple * addRdfTriple(iface::cellml_api::CellMLElement *pElement,
                                                      const CellMLSupport::CellmlFileRdfTriple::ModelQualifier &pModelQualifier,
                                                      const QString &pResource,
                                                      const QString &pId);
    CellMLSupport::CellmlFileRdfTriple * addRdfTriple(iface::cellml_api::CellMLElement *pElement,
                                                      const CellMLSupport::CellmlFileRdfTriple::BioQualifier &pBioQualifier,
                                                      const QString &pResource,
                                                      const QString &pId);

    QString uriBase() const;

    bool exportTo(const QString &pFileName, const Format &pFormat);

private:
    QString mFileName;

    ObjRef<iface::cellml_api::Model> mModel;

    ObjRef<iface::rdf_api::RDFAPIRepresentation> mRdfApiRepresentation;
    ObjRef<iface::rdf_api::DataSource> mRdfDataSource;

    QString mUriBase;

    CellmlFileRdfTriples mRdfTriples;

    bool mValid;

    CellmlFileIssues mIssues;

    CellmlFileRuntime *mRuntime;

    bool mLoadingNeeded;
    bool mValidNeeded;
    bool mRuntimeUpdateNeeded;

    void reset();

    bool rdfTripleExists(iface::cellml_api::CellMLElement *pElement,
                         const QString &pQualifier,
                         const QString &pResource, const QString &pId) const;

    QString rdfTripleSubject(iface::cellml_api::CellMLElement *pElement) const;
};

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================