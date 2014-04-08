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
// Editor find/replace widget
//==============================================================================

#include "editorfindreplacewidget.h"
#include "guiinterface.h"
#include "guiutils.h"

//==============================================================================

#include "ui_editorfindreplacewidget.h"

//==============================================================================

#include <Qt>

//==============================================================================

#include <QAction>
#include <QKeyEvent>
#include <QGridLayout>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QMenu>
#include <QPainter>
#include <QPixmap>

//==============================================================================

namespace OpenCOR {
namespace Editor {

//==============================================================================

EditorFindReplaceWidget::EditorFindReplaceWidget(QWidget *pParent) :
    Core::Widget(pParent),
    mGui(new Ui::EditorFindReplaceWidget)
{
    // Set up the GUI

    mGui->setupUi(this);

#ifdef Q_OS_MAC
    mGui->findEdit->setAttribute(Qt::WA_MacShowFocusRect, false);
    mGui->replaceWithEdit->setAttribute(Qt::WA_MacShowFocusRect, false);
    // Note: the above remove the focus border since it messes up the look of
    //       our edit widgets...
#endif

    // Create and handle our drop-down menu action

    mDropDownAction = new QAction(this);

    mCaseSensitiveAction = GuiInterface::newAction(true, this);
    mWholeWordsOnlyAction = GuiInterface::newAction(true, this);
    mRegularExpressionAction = GuiInterface::newAction(true, this);

    QMenu *dropDownMenu = new QMenu(this);

    dropDownMenu->addAction(mCaseSensitiveAction);
    dropDownMenu->addAction(mWholeWordsOnlyAction);
    dropDownMenu->addAction(mRegularExpressionAction);

    mDropDownAction->setMenu(dropDownMenu);

    mGui->findEdit->addAction(mDropDownAction, QLineEdit::LeadingPosition);

    connect(mCaseSensitiveAction, SIGNAL(toggled(bool)),
            this, SLOT(searchOptionChanged()));
    connect(mWholeWordsOnlyAction, SIGNAL(toggled(bool)),
            this, SLOT(searchOptionChanged()));
    connect(mRegularExpressionAction, SIGNAL(toggled(bool)),
            this, SLOT(searchOptionChanged()));

    // Create and handle our clear text action

    mClearTextAction = GuiInterface::newAction(QIcon(":/qtCreator/src/plugins/coreplugin/images/editclear.png"), this);

    connect(mClearTextAction, SIGNAL(triggered()),
            mGui->findEdit, SLOT(clear()));

    // Make sure that we take as little vertical space as possible while as much
    // horizontal space as possible

    mGui->layout->setSizeConstraint(QLayout::SetMaximumSize);

    // Make our find edit widget our focus proxy

    setFocusProxy(mGui->findEdit);

    // Some connections for our find widget

    connect(mGui->findEdit, SIGNAL(textChanged(const QString &)),
            this, SIGNAL(findTextChanged(const QString &)));
    connect(mGui->findEdit, SIGNAL(textChanged(const QString &)),
            this, SLOT(updateClearTextAction(const QString &)));

    // A few more things , so that we are properly initialised

    retranslateUi();

    searchOptionChanged();

    updateStyleSheet();
}

//==============================================================================

void EditorFindReplaceWidget::retranslateUi()
{
    // Retranslate our GUI

    mGui->retranslateUi(this);

    // Retranslate our actions

    GuiInterface::retranslateAction(mCaseSensitiveAction, tr("Case Sensitive"));
    GuiInterface::retranslateAction(mWholeWordsOnlyAction, tr("Whole Words Only"));
    GuiInterface::retranslateAction(mRegularExpressionAction, tr("Regular Expression"));

    GuiInterface::retranslateAction(mClearTextAction, tr("Clear Text"));
}

//==============================================================================

bool EditorFindReplaceWidget::caseSensitive() const
{
    // Return whether the search is to be case sensitive

    return mCaseSensitiveAction->isChecked();
}

//==============================================================================

bool EditorFindReplaceWidget::wholeWordsOnly() const
{
    // Return whether we search whole words only

    return mWholeWordsOnlyAction->isChecked();
}

//==============================================================================

bool EditorFindReplaceWidget::regularExpression() const
{
    // Return whether we use a regular expression

    return mRegularExpressionAction->isChecked();
}

//==============================================================================

void EditorFindReplaceWidget::updateStyleSheet()
{
    // Change the style of our tool buttons

    QColor shadowColor = Core::shadowColor();

    setStyleSheet(QString("QToolButton {"
                          "    border: 0px;"
                          "    border-radius: 3px;"
                          "    padding: 1px;"
                          "}"
                          ""
                          "QToolButton:focus {"
                          "    background: rgba(%1, %2, %3, 0.13);"
                          "    border: 1px solid rgba(%1, %2, %3, 0.39);"
                          "}"
                          ""
                          "QToolButton:hover {"
                          "    background: rgba(%1, %2, %3, 0.39);"
                          "}"
                          ""
                          "QToolButton:pressed {"
                          "    background: rgba(%1, %2, %3, 0.79);"
                          "}").arg(QString::number(shadowColor.red()),
                                   QString::number(shadowColor.green()),
                                   QString::number(shadowColor.blue())));
}

//==============================================================================

void EditorFindReplaceWidget::changeEvent(QEvent *pEvent)
{
    // Check whether the palette has changed and if so then update our style
    // sheet

    if (pEvent->type() == QEvent::PaletteChange)
        updateStyleSheet();
}

//==============================================================================

void EditorFindReplaceWidget::keyPressEvent(QKeyEvent *pEvent)
{
    // Let people know that a key has been pressed

    bool handled = false;

    emit keyPressed(pEvent, handled);

    // Carry on as normal, if the event wasn't handled

    if (handled)
        // Accept the event

        pEvent->accept();
    else
        // Default handling of the event

        QWidget::keyPressEvent(pEvent);
}

//==============================================================================

void EditorFindReplaceWidget::searchOptionChanged()
{
    // Update the icon used for the leading position of our find widget

    int nbOfOptions =  mCaseSensitiveAction->isChecked()
                      +mWholeWordsOnlyAction->isChecked()
                      +mRegularExpressionAction->isChecked();

    if (nbOfOptions) {
        static QIcon caseSensitiveIcon = QIcon(":/qtCreator/src/plugins/coreplugin/find/images/casesensitively.png");
        static QIcon wholeWordsOnlyIcon = QIcon(":/qtCreator/src/plugins/coreplugin/find/images/wholewords.png");
        static QIcon regularExpressionIcon = QIcon(":/qtCreator/src/plugins/coreplugin/find/images/regexp.png");

        static int IconSize = 16;
        static int IconWidth = 6;

        QPixmap dropDownPixmap = QPixmap(nbOfOptions*IconWidth-mRegularExpressionAction->isChecked()-1,
                                         IconSize);
        // Note: IconWidth-1, because regularExpressionIcon is effectively one
        //       pixel narrower than caseSensitiveIcon and wholeWordsOnlyIcon...

        dropDownPixmap.fill(Qt::transparent);

        QPainter dropDownPixmapPainter(&dropDownPixmap);

        int left = -IconWidth;

        if (mCaseSensitiveAction->isChecked()) {
            caseSensitiveIcon.paint(&dropDownPixmapPainter, left, 0, IconSize, IconSize);

            left += IconWidth;
        }

        if (mWholeWordsOnlyAction->isChecked()) {
            wholeWordsOnlyIcon.paint(&dropDownPixmapPainter, left, 0, IconSize, IconSize);

            left += IconWidth;
        }

        if (mRegularExpressionAction->isChecked())
            regularExpressionIcon.paint(&dropDownPixmapPainter, left-1, 0, IconSize, IconSize);

        mDropDownAction->setIcon(dropDownPixmap);
    } else {
        // No options are set, so use our default icon

        static QIcon defaultIcon = QIcon(":/qtCreator/src/plugins/coreplugin/images/magnifier.png");

        mDropDownAction->setIcon(defaultIcon);
    }
}

//==============================================================================

void EditorFindReplaceWidget::updateClearTextAction(const QString &pText)
{
    // Show/hide our clear text action, based on whether our find widget
    // contains some text

    if (pText.isEmpty())
        mGui->findEdit->removeAction(mClearTextAction);
    else
        mGui->findEdit->addAction(mClearTextAction, QLineEdit::TrailingPosition);
}

//==============================================================================

}   // namespace Editor
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================