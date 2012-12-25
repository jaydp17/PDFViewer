#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <KXmlGuiWindow>
#include <KAction>
#include <KApplication>
#include <KActionCollection>
#include <KStandardAction>
#include <QFileDialog>

#include "document.h"
#include "pageview.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public KXmlGuiWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void showPage(int index);
    
public slots:
    void openSlot();
    bool openPDF(QString filename);
    void nextPageSlot();
    void prevPageSlot();

private:
    Ui::MainWindow *ui;
    void setupActions();
    Document *mDoc;
    PageView *mPageView;

    //KActions
    KAction *openAction;
    KAction *closeAction;
    KAction *reloadAction;
    KAction *quitAction;
    KAction *prevPageAction;
    KAction *nextPageAction;


};

#endif // MAINWINDOW_H
