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
// SecondOrderRungeKuttaSolver plugin
//==============================================================================

#include "secondorderrungekuttasolver.h"
#include "secondorderrungekuttasolverplugin.h"

//==============================================================================

namespace OpenCOR {
namespace SecondOrderRungeKuttaSolver {

//==============================================================================

PLUGININFO_FUNC SecondOrderRungeKuttaSolverPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin which implements the <a href=\"http://en.wikipedia.org/wiki/Runge–Kutta_methods\">second-order Runge-Kutta method</a> to solve ODEs."));
    descriptions.insert("fr", QString::fromUtf8("une extension qui implémente la <a href=\"http://en.wikipedia.org/wiki/Runge–Kutta_methods\">méthode Runge-Kutta du deuxième ordre</a> pour résoudre des EDOs."));

    return new PluginInfo(PluginInfo::Solver, true,
                          QStringList() << "CoreSolver",
                          descriptions);
}

//==============================================================================
// I18n interface
//==============================================================================

void SecondOrderRungeKuttaSolverPlugin::retranslateUi()
{
    // We don't handle this interface...
}

//==============================================================================
// Solver interface
//==============================================================================

void * SecondOrderRungeKuttaSolverPlugin::instance() const
{
    // Create and return an instance of the solver

    return new SecondOrderRungeKuttaSolver();
}

//==============================================================================

Solver::Type SecondOrderRungeKuttaSolverPlugin::type() const
{
    // Return the type of the solver

    return Solver::Ode;
}

//==============================================================================

QString SecondOrderRungeKuttaSolverPlugin::name() const
{
    // Return the name of the solver

    return "Runge-Kutta (2nd order)";
}

//==============================================================================

Solver::Properties SecondOrderRungeKuttaSolverPlugin::properties() const
{
    // Return the properties supported by the solver

    Solver::Properties res = Solver::Properties();
    Descriptions stepPropertyDescriptions;

    stepPropertyDescriptions.insert("en", QString::fromUtf8("Step"));
    stepPropertyDescriptions.insert("fr", QString::fromUtf8("Pas"));

    res.append(Solver::Property(Solver::Double, StepId, stepPropertyDescriptions, StepDefaultValue, true));

    return res;
}

//==============================================================================

}   // namespace SecondOrderRungeKuttaSolver
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================