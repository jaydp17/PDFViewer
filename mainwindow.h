#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <KXmlGuiWindow>
#include <KAction>
#include <KApplication>
#include <KActionCollection>
#include <KStandardAction>
#include <KSelectAction>
#include <KMessageBox>

#include <QFileDialog>
#include <QtGui>

#include "document.h"
#include "pageview.h"
#include "finddockwidget.h"

namespace Ui {
class MainWindow;
}



class MainWindow : public KXmlGuiWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void showPage(int index, int zoomRatioIndex=-1);
    
public slots:
    void openSlot();
    bool openPDF(QString filename);
    void nextPageSlot();
    void prevPageSlot();
    void zoominSlot();
    void zoomoutSlot();
    void zoomresetSlot();
    void showFindDock();
    void forwardSearch(QString text);
    void backwardSearch(QString text);


private:
    Ui::MainWindow *ui;
    void setupActions();
    Document *mDoc;
    PageView *mPageView;
    QPixmap currentPixmap;
    int zoomRatioIndex;
    QDockWidget *dock;

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
    QRectF fixRelativePos(QRectF rect);

    //Dock widgets
    FindDockWidget *fdw;
};

#endif // MAINWINDOW_H
