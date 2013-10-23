#ifdef INTERFACE_DEFINITION
    #define PURE = 0
#else
    #define PURE
#endif

    virtual void changeEvent(QEvent *pEvent) PURE;

    virtual void updateGui(Plugin *pViewPlugin, const QString &pFileName) PURE;

    virtual void initializeView() PURE;
    virtual void finalizeView() PURE;

    virtual QWidget * viewWidget(const QString &pFileName) PURE;
    virtual QWidget * removeViewWidget(const QString &pFileName) PURE;
    virtual QString viewName() const PURE;

    virtual QIcon fileTabIcon(const QString &pFileName) const PURE;

    virtual bool saveFile(const QString &pOldFileName,
                          const QString &pNewFileName) PURE;

    virtual void fileOpened(const QString &pFileName) PURE;
    virtual void fileSaved(const QString &pFileName) PURE;
    virtual void fileRenamed(const QString &pOldFileName,
                             const QString &pNewFileName) PURE;
    virtual void fileClosed(const QString &pFileName) PURE;

    virtual bool canClose() PURE;

#undef PURE