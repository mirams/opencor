//==============================================================================
// Main window
//==============================================================================

#include "checkforupdateswindow.h"
#include "common.h"
#include "i18ninterface.h"
#include "mainwindow.h"
#include "plugin.h"
#include "pluginmanager.h"
#include "pluginswindow.h"
#include "preferenceswindow.h"
#include "utils.h"

//==============================================================================

#include "ui_mainwindow.h"

//==============================================================================

#ifdef Q_WS_WIN
    #include <windows.h>
#endif

//==============================================================================

#include <QDesktopServices>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#ifdef Q_WS_MAC
    #include <QShortcut>
#endif
#include <QUrl>

//==============================================================================

namespace OpenCOR {

//==============================================================================

MainWindow::MainWindow(QWidget *pParent) :
    QMainWindow(pParent),
    mUi(new Ui::MainWindow),
    mFileNewMenu(0),
    mViewOrganisationMenu(0),
    mViewEditingMenu(0),
    mViewSeparator(0)
{
    // Create our settings object

    mSettings = new QSettings(qApp->applicationName());

    // Create our plugin manager (which will automatically load our various
    // plugins)

    mPluginManager = new PluginManager(mSettings, PluginInfo::Gui);

    // Specify some general docking settings

    setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);
    setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);

    // Set up the UI

    mUi->setupUi(this);
    // Note: the application icon (which needs to be set for Linux, but neither
    //       for Windows nor Mac OS X, since it's set through CMake in those
    //       cases (see CMakeLists.txt)) is set within the UI file. This being
    //       said, it's good to have it set for all three platforms, since it
    //       can then be used in, for example, the about box, so...

//---GRY--- THE BELOW CODE WOULD BE GOOD TO HAVE, BUT THERE IS A KNOWN BUG (SEE
//          http://bugreports.qt.nokia.com/browse/QTBUG-3116) WHICH MAKES THAT
//          CODE TO CAUSE PROBLEMS WITH THE LOADING/RESTORING OF THE APPLICATION
//          GEOMETRY, SO...

//#ifdef Q_WS_MAC
//    // Make the application look more like a Mac OS X application

//    setUnifiedTitleAndToolBarOnMac(true);
//#endif

    // Some connections to handle our Help toolbar

    GuiInterface::connectToolBarToAction(mUi->helpToolbar,
                                         mUi->actionHelpToolbar);

    // A connection to handle the status bar

    connect(mUi->actionStatusBar, SIGNAL(triggered(bool)),
            statusBar(), SLOT(setVisible(bool)));

    // Some connections to handle our various menu items

    connect(mUi->actionExit, SIGNAL(triggered(bool)),
            this, SLOT(close()));
    connect(mUi->actionResetAll, SIGNAL(triggered(bool)),
            this, SLOT(resetAll()));

    // Set the shortcuts of some actions
    // Note: we do it here, so that we can use standard shortcuts (whenever
    //       possible)...

    mUi->actionFullScreen->setShortcut(Qt::Key_F11);
    mUi->actionExit->setShortcut(QKeySequence::Quit);

#ifdef Q_WS_MAC
    // A special shortcut to have OpenCOR minimised on Mac OS X when pressing
    // Cmd+M
    // Note: indeed, when pressing Cmd+M on Mac OS X, the active application
    //       is expected to minimise itself, so...

    new QShortcut(QKeySequence("Ctrl+M"),
                  this, SLOT(showMinimized()));

    // A special shortcut to have OpenCOR hidden on Mac OS X when pressing
    // Cmd+H
    // Note: indeed, when pressing Cmd+H on Mac OS X, the active application
    //       is expected to hide itself, so...

    new QShortcut(QKeySequence("Ctrl+H"),
                  this, SLOT(hide()));
#endif

    // Initialise our various plugins

    QList<Plugin *> loadedPlugins = mPluginManager->loadedPlugins();

    foreach (Plugin *plugin, loadedPlugins) {
        // Do various things that are related to our differennt plugin
        // interfaces
        // Note: the order in which we do those things is important since, for
        //       example, the call to CoreInterface::initialize() may need some
        //       information which has been set as part of the GUI interface
        //       (e.g. the pointer to the main window)...

        // GUI interface

        GuiInterface *guiInterface = qobject_cast<GuiInterface *>(plugin->instance());

        if (guiInterface)
            // Keep track of some information

            guiInterface->setMainWindow(this);

        // Internationalisation interface

        I18nInterface *i18nInterface = qobject_cast<I18nInterface *>(plugin->instance());

        if (i18nInterface)
            // Keep track of some information

            i18nInterface->setI18nPluginName(plugin->name());

        // Core interface

        CoreInterface *coreInterface = qobject_cast<CoreInterface *>(plugin->instance());

        if (coreInterface)
            // Initialise the plugin

            coreInterface->initialize();

        // Back to the GUI interface

        if (guiInterface)
            // Initialise the plugin further (i.e. do things which can only be
            // done by OpenCOR itself)

            initializeGuiPlugin(plugin->name(), guiInterface->guiSettings());
    }

    // Setup our various plugins
    // Note: this is very different from initialising our various plugins.
    //       Indeed, to initialise a plugin is something that doesn't require to
    //       know anything about any other plugin while to setup a plugin is for
    //       anything that requires prior knowledge of what one or several other
    //       plugins are about, and this is something that cannot be known until
    //       all the plugins have been initialised...

    foreach (Plugin *plugin, loadedPlugins) {
        CoreInterface *coreInterface = qobject_cast<CoreInterface *>(plugin->instance());

        if (coreInterface)
            coreInterface->setup(loadedPlugins);
    }

    // Retrieve the user settings from the previous session, if any

    loadSettings();

    // Initialise the checking state of the full screen action, since OpenCOR
    // may (re)start in full screen mode

    mUi->actionFullScreen->setChecked(isFullScreen());

    // Bring ourselves to the foreground
    // Note: indeed, when starting/restarting OpenCOR (as a result of a Reset
    //       All in the case of a restart), OpenCOR will on Mac OS X be behind
    //       any other application. Now, because it doesn't harm to bring
    //       ourselves to the foreground when on Windows or Linux, we may as
    //       well do it on those platforms too...

    showSelf();
}

//==============================================================================

MainWindow::~MainWindow()
{
    // Finalize our various plugins
    // Note: we only need to test for our default interface since we want to
    //       call the finalize method and this method is not overriden by any
    //       other interface, so...

    foreach (Plugin *plugin, mPluginManager->loadedPlugins()) {
        CoreInterface *coreInterface = qobject_cast<CoreInterface *>(plugin->instance());

        if (coreInterface)
            coreInterface->finalize();
    }

    // Delete some internal objects

    delete mPluginManager;
    delete mSettings;
    delete mUi;
}

//==============================================================================

void MainWindow::changeEvent(QEvent *pEvent)
{
    // Default handling of the event

    QMainWindow::changeEvent(pEvent);

    // If the system's locale has changed, then update OpenCOR's locale in case
    // the user wants to use the system's locale

    if (   (pEvent->type() == QEvent::LocaleChange)
        && (mUi->actionSystem->isChecked()))
        setLocale(SystemLocale);
}

//==============================================================================

void MainWindow::showEvent(QShowEvent *pEvent)
{
    // Default handling of the event

    QMainWindow::showEvent(pEvent);

    // The first time we show OpenCOR, we want to make sure that its menu is
    // fine (i.e. it respects OpenCOR's settings that were loaded in the
    // constructor). Various connections (set in the constructor) take care of
    // this, but there still remains one menu item (which specifies whether the
    // status bar is to be shown or not) for which no connection can be set. So,
    // we have to 'manually' set the status of that menu item here (as opposed
    // to, say, the constructor), since we may need (on Windows at least) all of
    // OpenCOR to be visible in order to be able to determine whether the status
    // bar is visible or not

    static bool firstTime = true;

    if (firstTime) {
        firstTime = false;

        mUi->actionStatusBar->setChecked(statusBar()->isVisible());
    }
}

//==============================================================================

void MainWindow::closeEvent(QCloseEvent *pEvent)
{
    // Keep track of our default settings
    // Note: it must be done here, as opposed to the destructor, otherwise some
    //       settings (e.g. dock windows) won't be properly saved

    saveSettings();

    // Default handling of the event

    QMainWindow::closeEvent(pEvent);
}

//==============================================================================

void MainWindow::initializeGuiPlugin(const QString &pPluginName,
                                     GuiSettings *pGuiSettings)
{
    static const QString CorePlugin = "Core";
    static const QString HelpPlugin = "Help";

    // Add the menus to our menu bar or merge them to existing menus, if needed
    // Note: we must do that in reverse order since we are inserting menus,
    //       as opposed to appending them...

    QListIterator<GuiMenuSettings *> menuIter(pGuiSettings->menus());

    menuIter.toBack();

    while (menuIter.hasPrevious()) {
        // Insert the menu in the right place

        GuiMenuSettings *menuSettings = menuIter.previous();

        QMenu *newMenu = menuSettings->menu();
        QString newMenuName = newMenu->objectName();

        QMenu *oldMenu = mMenus.value(newMenuName);

        if (oldMenu) {
            // A menu with the same name already exists, so add the contents of
            // the new menu to the existing one

            oldMenu->addSeparator();
            oldMenu->addActions(newMenu->actions());

            // Delete the new menu, since we don't need it...
            // Note: it's not critical since the menu never gets shown, but it
            //       doesn't harm either, so...

            delete newMenu;
        } else {
            // No menu with the same name already exists, so add the new menu to
            // our menu bar

            switch (menuSettings->type()) {
            default:   // View
                mUi->menuBar->insertAction(mUi->menuView->menuAction(),
                                           newMenu->menuAction());
            }

            // Keep track of the new menu

            mMenus.insert(newMenuName, newMenu);
        }
    }

    // Add the actions/separators to our different menus
    // Note: as for the menus above, we must do that in reverse order since we
    //       are inserting actions, as opposed to appending them...

    QListIterator<GuiMenuActionSettings *> menuActionIter(pGuiSettings->menuActions());

    menuActionIter.toBack();

    while (menuActionIter.hasPrevious()) {
        // Insert the action/separator to the right menu

        GuiMenuActionSettings *menuActionSettings = menuActionIter.previous();

        switch (menuActionSettings->type()) {
        case GuiMenuActionSettings::File:
            if(menuActionSettings->action())
                mUi->menuFile->insertAction(mUi->menuFile->actions().first(),
                                            menuActionSettings->action());
            else
                mUi->menuFile->insertSeparator(mUi->menuFile->actions().first());

            break;
        default:
            // Not a type of interest, so do nothing...

            ;
        }
    }

    // Add the actions to our different menus, but we process the actions in the
    // right order this time, since we must add the actions rather than insert
    // them

    static QString pluginForFileNewMenu = QString();

    foreach (GuiMenuActionSettings *menuActionSettings,
             pGuiSettings->menuActions())
        // Insert the action to the right menu

        switch (menuActionSettings->type()) {
        case GuiMenuActionSettings::FileNew:
            // Check whether the File|New menu has been created and if not, then
            // create it

            if (!mFileNewMenu) {
                // The menu doesn't already exist, so create it

                mFileNewMenu = new QMenu(this);

                mFileNewMenu->menuAction()->setIcon(QIcon(":oxygen/mimetypes/application-x-zerosize.png"));

                // Add the New menu to our File menu and add a separator after
                // it

                mUi->menuFile->insertMenu(mUi->menuFile->actions().first(),
                                          mFileNewMenu);
                mUi->menuFile->insertSeparator(mUi->menuFile->actions().at(1));

                pluginForFileNewMenu = pPluginName;
            } else if (pluginForFileNewMenu.compare(pPluginName)) {
                // The File|New menu already exists, so add a separator to it so
                // that previous menu items (from a different plugin) don't get
                // mixed up with the new one

                mFileNewMenu->addSeparator();

                pluginForFileNewMenu = pPluginName;
            }

            mFileNewMenu->addAction(menuActionSettings->action());

            break;
        default:
            // Not a type of interest, so do nothing...

            ;
        }

    // Add the toolbars (including to the View|Toolbars menu)

    foreach (GuiToolBarSettings *toolbarSettings, pGuiSettings->toolbars()) {
        QToolBar *newToolbar = toolbarSettings->toolbar();
        QString newToolbarName = newToolbar->objectName();

        QToolBar *oldToolbar = mToolbars.value(newToolbarName);

        if (oldToolbar) {
            // A toolbar with the same name already exists, so add the contents
            // of the new toolbar to the existing one

            oldToolbar->addSeparator();
            oldToolbar->addActions(newToolbar->actions());

            // Delete the new toolbar, since we don't need it...
            // Note: this prevents from the toolbar from being shown in the
            //       top-left corner of the main window

            delete newToolbar;
        } else {
            // No toolbar with the same name already exists, so add the new
            // toolbar

            addToolBar(toolbarSettings->defaultDockingArea(), newToolbar);

            // Also add a toolbar action to our View|Toolbars menu

            mUi->menuToolbars->addAction(toolbarSettings->action());

            // Keep track of the new toolbar

            mToolbars.insert(newToolbarName, newToolbar);
        }
    }

    // Set the central widget, but only if it is being done from the Core plugin

    if (!pPluginName.compare(CorePlugin))
        if (pGuiSettings->centralWidget())
            setCentralWidget((QWidget *) pGuiSettings->centralWidget());

    // Add the windows (including to the corresponding menu)

    foreach (GuiWindowSettings *windowSettings, pGuiSettings->windows()) {
        // Dock the window to its default docking area

        QDockWidget *dockWidget = (QDockWidget *) windowSettings->window();

        addDockWidget(windowSettings->defaultDockingArea(), dockWidget);

        // Add an action to our menu to show/hide the menu

        bool doConnectDockWidgetToAction = true;

        switch (windowSettings->type()) {
        case GuiWindowSettings::Organisation:
            // Update the View menu by adding the action to the
            // View|Organisation menu

            updateViewMenu(GuiWindowSettings::Organisation,
                           windowSettings->action());

            break;
        case GuiWindowSettings::Editing:
            // Update the View menu by adding the action to the View|Editing
            // menu

            updateViewMenu(GuiWindowSettings::Editing,
                           windowSettings->action());

            break;
        case GuiWindowSettings::Help:
            if (!pPluginName.compare(HelpPlugin)) {
                // We only want to add the action if we are coming here from the
                // Help plugin

                mUi->menuHelp->insertAction(mUi->actionHomePage,
                                            windowSettings->action());
                mUi->menuHelp->insertSeparator(mUi->actionHomePage);

                // In the case of a Help window, we also want to add the action
                // to our Help toolbar

                mUi->helpToolbar->insertAction(mUi->actionHomePage,
                                               windowSettings->action());
                mUi->helpToolbar->insertSeparator(mUi->actionHomePage);
            } else {
                doConnectDockWidgetToAction = false;
            }

            break;
        default:
            // Unknown type, so...

            doConnectDockWidgetToAction = false;
        }

        // Connect the action to the window

        if (doConnectDockWidgetToAction)
            GuiInterface::connectDockWidgetToAction(dockWidget,
                                                    windowSettings->action());
    }

    // Reorder our various View menus, just in case

    reorderViewMenus();
}

//==============================================================================

static const QString SettingsLocale              = "Locale";
static const QString SettingsGeometry            = "Geometry";
static const QString SettingsState               = "State";
static const QString SettingsStatusBarVisibility = "StatusBarVisibility";

//==============================================================================

void MainWindow::loadSettings()
{
    // Retrieve the language to be used by OpenCOR

    setLocale(mSettings->value(SettingsLocale, SystemLocale).toString());

    // Retrieve the geometry and state of the main window

    if (   !restoreGeometry(mSettings->value(SettingsGeometry).toByteArray())
        || !restoreState(mSettings->value(SettingsState).toByteArray())) {
        // The geometry and/or state of the main window couldn't be
        // retrieved, so go with some default settins

        // Default size and position of the main window

        double ratio = 1.0/13.0;
        QRect desktopGeometry = qApp->desktop()->availableGeometry();
        int horizSpace = ratio*desktopGeometry.width();
        int vertSpace  = ratio*desktopGeometry.height();

        setGeometry(desktopGeometry.left()+horizSpace,
                    desktopGeometry.top()+vertSpace,
                    desktopGeometry.width()-2*horizSpace,
                    desktopGeometry.height()-2*vertSpace);
    } else {
        // The geometry and state of the main window could be retrieved, so
        // carry on with the loading of the settings

        // Retrieve whether the status bar is to be shown

        mUi->statusBar->setVisible(mSettings->value(SettingsStatusBarVisibility,
                                                    true).toBool());
    }

    // Retrieve the settings of our various plugins

    foreach (Plugin *plugin, mPluginManager->loadedPlugins()) {
        GuiInterface *guiInterface = qobject_cast<GuiInterface *>(plugin->instance());

        if (guiInterface) {
            mSettings->beginGroup(SettingsPlugins);
                mSettings->beginGroup(plugin->name());
                    guiInterface->loadSettings(mSettings);
                mSettings->endGroup();
            mSettings->endGroup();
        }
    }
}

//==============================================================================

void MainWindow::saveSettings() const
{
    // Keep track of the language to be used by OpenCOR

    mSettings->setValue(SettingsLocale, mLocale);

    // Keep track of the geometry of the main window

    mSettings->setValue(SettingsGeometry, saveGeometry());

    // Keep track of the state of the main window

    mSettings->setValue(SettingsState, saveState());

    // Keep track of whether the status bar is to be shown

    mSettings->setValue(SettingsStatusBarVisibility,
                        mUi->statusBar->isVisible());

    // Keep track of the settings of our various plugins

    foreach (Plugin *plugin, mPluginManager->loadedPlugins()) {
        GuiInterface *guiInterface = qobject_cast<GuiInterface *>(plugin->instance());

        if (guiInterface) {
            mSettings->beginGroup(SettingsPlugins);
                mSettings->beginGroup(plugin->name());
                    guiInterface->saveSettings(mSettings);
                mSettings->endGroup();
            mSettings->endGroup();
        }
    }
}

//==============================================================================

QString MainWindow::locale() const
{
    // Return the current locale

    const QString systemLocale = QLocale::system().name().left(2);

    return (mLocale == SystemLocale)?systemLocale:mLocale;
}

//==============================================================================

void MainWindow::setLocale(const QString &pLocale)
{
    if ((pLocale != mLocale) || (pLocale == SystemLocale)) {
        // The new locale is different from the existing one, so we need to
        // translate everything

        const QString systemLocale = QLocale::system().name().left(2);

        QString realLocale = (pLocale == SystemLocale)?systemLocale:pLocale;

        mLocale = pLocale;

        // Specify the language to be used by OpenCOR

        qApp->removeTranslator(&mQtTranslator);
        mQtTranslator.load(":qt_"+realLocale);
        qApp->installTranslator(&mQtTranslator);

        qApp->removeTranslator(&mAppTranslator);
        mAppTranslator.load(":app_"+realLocale);
        qApp->installTranslator(&mAppTranslator);

        // Retranslate OpenCOR

        mUi->retranslateUi(this);

        // Retranslate some widgets that are not originally part of our user
        // interface

        if (mFileNewMenu)
            GuiInterface::retranslateMenu(mFileNewMenu, tr("New"));

        if (mViewOrganisationMenu)
            GuiInterface::retranslateMenu(mViewOrganisationMenu,
                                          tr("Organisation"));

        if (mViewEditingMenu)
            GuiInterface::retranslateMenu(mViewEditingMenu, tr("Editing"));

        // Update the locale of our various loaded plugins
        // Note: we do this in reverse order of the loaded plugins since some
        //       plugins loaded first (e.g. the Core plugin) may need an updated
        //       translation from a plugin that depends on them (e.g. the
        //       RawView plugin depends on the Core plugin and the Core plugin
        //       needs the RawView plugin to be properly translated before being
        //       translated, since it needs to know the name of the view that
        //       was created by the RawView plugin), so...

        QList<Plugin *> loadedPlugins = mPluginManager->loadedPlugins();

        for (int i = loadedPlugins.count()-1; i >= 0; --i) {
            I18nInterface *i18nInterface = qobject_cast<I18nInterface *>(loadedPlugins.at(i)->instance());

            if (i18nInterface)
                i18nInterface->setLocale(realLocale);
        }

        // Reorder our various View menus, just in case

        reorderViewMenus();
    }

    // Update the checked menu item
    // Note: it has to be done every single time, since selecting a menu item
    //       will automatically toggle its checked status, so...

    mUi->actionSystem->setChecked(mLocale == SystemLocale);

    mUi->actionEnglish->setChecked(mLocale == EnglishLocale);
    mUi->actionFrench->setChecked(mLocale == FrenchLocale);
}

//==============================================================================

void MainWindow::reorderViewMenu(QMenu *pViewMenu)
{
    // Reorder the required View menu

    QStringList menuItemTitles;
    QMap<QString, QAction *> menuItemActions;

    // Retrieve the title of the menu items and keep track of their actions

    foreach(QAction *menuItemAction, pViewMenu->actions()) {
        // Remove any '&' present in the menu item title, as well as replace
        // accentuated characters by non-accentuated ones, making the sorting
        // sensible

        QString menuItemTitle = menuItemAction->text().remove("&").normalized(QString::NormalizationForm_KD);

        for (int i = menuItemTitle.length()-1; i >= 0; --i)
            if (menuItemTitle[i].category() == QChar::Mark_NonSpacing)
                menuItemTitle.remove(i, 1);

        // Keep track of the menu item title and the action to which it is
        // associated

        menuItemTitles << menuItemTitle;
        menuItemActions.insert(menuItemTitle, menuItemAction);
    }

    // Sort the menu items

    menuItemTitles.sort();

    // Add the menu items actions in the new order
    // Note: to use addAction will effectively 'move' the menu items to the end
    //       of the menu, so since we do it in the right order, we end up with
    //       the menu items being properly ordered...

    foreach(const QString menuItemTitle, menuItemTitles)
        pViewMenu->addAction(menuItemActions.value(menuItemTitle));
}

//==============================================================================

void MainWindow::reorderViewMenus()
{
    // Reorder the View|Toolbars menu, as well as the View|Organisation and
    // View|Editing menus, should they exist
    // Note: this is useful after having added a new menu item or after having
    //       changed the locale

    reorderViewMenu(mUi->menuToolbars);

    if (mViewOrganisationMenu)
        reorderViewMenu(mViewOrganisationMenu);

    if (mViewEditingMenu)
        reorderViewMenu(mViewEditingMenu);
}

//==============================================================================

void MainWindow::updateViewMenu(const GuiWindowSettings::GuiWindowSettingsType &pMenuType,
                                QAction *pAction)
{
    // Check whether we need to insert a separator before the Full Screen menu
    // item

    if ((pMenuType != GuiWindowSettings::Help) && !mViewSeparator)
        // None of the menus have already been inserted which means that we need
        // to insert a separator before the Full Screen menu item

        mViewSeparator =  mUi->menuView->insertSeparator(mUi->actionFullScreen);

    // Determine the menu that is to be inserted, should this be required, and
    // the action before which it is to be inserted

    QMenu **menu;
    QAction *action;

    switch (pMenuType) {
    case GuiWindowSettings::Organisation:
        menu   = &mViewOrganisationMenu;
        action = mViewEditingMenu?mViewEditingMenu->menuAction():mViewSeparator;

        break;
    case GuiWindowSettings::Editing:
        menu   = &mViewEditingMenu;
        action = mViewSeparator;

        break;
    default:
        // Unknown type, so just leave...

        return;
    }

    // Check whether the menu already exists and create it if not

    if (!*menu) {
        // The menu doesn't already exist, so create it

        *menu = new QMenu(this);

        // Add the menu to our View menu

        mUi->menuView->insertMenu(action, *menu);
    }

    // At this stage, the menu to which we want to add an action has been
    // created, so we can just add the action to it

    (*menu)->addAction(pAction);

}

//==============================================================================

#ifdef Q_WS_WIN
void MainWindow::showSelf() const
#else
void MainWindow::showSelf()
#endif
{
    // Note: to show ourselves, one would normally use activateWindow(), but
    //       depending on the operating system it may or not bring OpenCOR to
    //       the foreground, so... instead we do what follows, depending on the
    //       operating system...

#ifdef Q_WS_WIN
    // Retrieve OpenCOR's window Id

    WId mainWinId = winId();

    // Bring OpenCOR to the foreground

    DWORD foregroundThreadProcId = GetWindowThreadProcessId(GetForegroundWindow(),
                                                            NULL);
    DWORD mainThreadProcId = GetWindowThreadProcessId(mainWinId, NULL);

    if (foregroundThreadProcId != mainThreadProcId) {
        // OpenCOR's thread process Id is not that of the foreground window, so
        // attach the foreground thread to OpenCOR's, set OpenCOR to the
        // foreground, and detach the foreground thread from OpenCOR's

        AttachThreadInput(foregroundThreadProcId, mainThreadProcId, true);

        SetForegroundWindow(mainWinId);
        SetFocus(mainWinId);

        AttachThreadInput(foregroundThreadProcId, mainThreadProcId, false);
    } else {
        // OpenCOR's thread process Id is that of the foreground window, so
        // just set OpenCOR to the foreground

        SetForegroundWindow(mainWinId);
    }

    // Show/restore OpenCOR, depending on its current state

    if (IsIconic(mainWinId))
        ShowWindow(mainWinId, SW_RESTORE);
    else
        ShowWindow(mainWinId, SW_SHOW);

    // Note: under Windows, to use activateWindow() will only highlight the
    //       application in the taskbar, since under Windows no application
    //       should be allowed to bring itself to the foreground when another
    //       application is already in the foreground. Fair enough, but it
    //       happens that, here, the user wants OpenCOR to be brought to the
    //       foreground, hence the above code to get the effect we are after...
#else
    // Do what one would normally do

    activateWindow();
    raise();   // Just to be on the safe side
    // Note: raise() never seems to be required on Mac OS X, but to use
    //       activateWindow() on its own under Linux may or not give the
    //       expected result, so...
#endif
}

//==============================================================================

#ifdef Q_WS_WIN
void MainWindow::singleAppMsgRcvd(const QString &) const
#else
void MainWindow::singleAppMsgRcvd(const QString &)
#endif
{
    // We have just received a message from another instance of OpenCOR, so
    // bring ourselves to the foreground

    showSelf();

    // Now, we must handle the arguments that were passed to OpenCOR

    // TODO: handle the arguments passed to the 'official' instance of OpenCOR
}

//==============================================================================

void MainWindow::on_actionFullScreen_triggered()
{
    // Switch to / back from full screen mode

    if (mUi->actionFullScreen->isChecked())
        showFullScreen();
    else
        showNormal();
}

//==============================================================================

void MainWindow::on_actionSystem_triggered()
{
    // Select the system's language as the language used by OpenCOR

    setLocale(SystemLocale);
}

//==============================================================================

void MainWindow::on_actionEnglish_triggered()
{
    // Select English as the language used by OpenCOR

    setLocale(EnglishLocale);
}

//==============================================================================

void MainWindow::on_actionFrench_triggered()
{
    // Select French as the language used by OpenCOR

    setLocale(FrenchLocale);
}

//==============================================================================

void MainWindow::on_actionPlugins_triggered()
{
    // Plugins' preferences

    if (mPluginManager->plugins().count()) {
        // There are some plugins, so we can show the plugins window

        OpenCOR::PluginsWindow pluginsWindow(mPluginManager, this);

        mSettings->beginGroup(pluginsWindow.objectName());
            pluginsWindow.loadSettings(mSettings);
            pluginsWindow.exec();
            pluginsWindow.saveSettings(mSettings);
        mSettings->endGroup();
    } else {
        // There are no plugins, so...

        QMessageBox::warning(this, tr("Plugins"),
                             tr("No plugin could be found."));
    }
}

//==============================================================================

void MainWindow::on_actionPreferences_triggered()
{
    // User's preferences

    OpenCOR::PreferencesWindow preferencesWindow(this);

    preferencesWindow.exec();
}

//==============================================================================

void MainWindow::on_actionHomePage_triggered()
{
    // Look up OpenCOR home page

    QDesktopServices::openUrl(QUrl(OpencorHomepageUrl));
}

//==============================================================================

void MainWindow::on_actionCheckForUpdates_triggered()
{
    // Check for updates

    OpenCOR::CheckForUpdatesWindow checkForUpdatesWindow(this);

    checkForUpdatesWindow.exec();
}

//==============================================================================

void MainWindow::on_actionAbout_triggered()
{
    // Display some information about OpenCOR

    QMessageBox::about(this, tr("About"),
                        "<h1 align=center><b>"+getAppVersion(qApp)+"</b></h1>"
                       +"<h3 align=center><em>"+getOsName()+"</em></h3>"
                       +"<p align=center><em>"+getAppCopyright(true)+"</em></p>"
                       +"<a href=\""+QString(OpencorHomepageUrl)+"\">"+qApp->applicationName()+"</a> "+tr("is a cross-platform <a href=\"http://www.cellml.org/\">CellML</a>-based modelling environment which can be used to organise, edit, simulate and analyse CellML files."));
}

//==============================================================================

void MainWindow::restart(const bool &pSaveSettings) const
{
    // Restart OpenCOR after saving its settings, if required
    // Note: the closeEvent method won't get called when exiting OpenCOR and
    //       this is exactly what we want in case we don't want to save its
    //       settings (e.g. when resetting all)

    if (pSaveSettings)
        saveSettings();

    qApp->exit(NeedRestart);
}

//==============================================================================

void MainWindow::resetAll()
{
    if( QMessageBox::question(this, qApp->applicationName(),
                              tr("You are about to reset <strong>all</strong> of your user settings. Are you sure that this is what you want?"),
                              QMessageBox::Yes|QMessageBox::No,
                              QMessageBox::Yes) == QMessageBox::Yes ) {
        // We want to reset everything, so clear all the user settings and
        // restart OpenCOR (indeed, a restart will ensure that the various dock
        // windows are, for instance, properly reset with regards to their
        // dimensions)

        QSettings(qApp->applicationName()).clear();

        // Restart OpenCOR without first saving its settings

        restart(false);
    }
}

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
