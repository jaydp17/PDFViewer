#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <KXmlGuiWindow>
#include <KAction>
#include <KApplication>
#include <KActionCollection>
#include <KStandardAction>
#include <QFileDialog>
#include <KSelectAction>

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
    void showPage(int index, int zoomRatioIndex=3);
    
public slots:
    void openSlot();
    bool openPDF(QString filename);
    void nextPageSlot();
    void prevPageSlot();
    void zoominSlot();
    void zoomoutSlot();
    void zoomresetSlot();


private:
    Ui::MainWindow *ui;
    void setupActions();
    Document *mDoc;
    PageView *mPageView;
    int zoomRatioIndex;

    //KActions
    KAction *openAction;
    KAction *closeAction;
    KAction *reloadAction;
    KAction *quitAction;
    KAction *prevPageAction;
    KAction *nextPageAction;
    KAction *zoominAction;
    KAction *zoomoutAction;


    bool nextPageExists();
    bool prevPageExists();
    void updateActions();

};

#endif // MAINWINDOW_H
