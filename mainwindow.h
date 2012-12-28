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
#include <QSplitter>
#include <QTreeWidget>

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
    void zoomSelectChanged();
    void zoomresetSlot();
    void showFindDock();
    void forwardSearch(QString text);
    void backwardSearch(QString text);
    void showSelectedText(QRectF rect);
    void gotoPageSlot();


private:
    Ui::MainWindow *ui;
    void setupActions();
    void setupTextDock();
    void showTOC();
    Document *mDoc;
    PageView *mPageView;
    int zoomRatioIndex;
    QDockWidget *textDock;
    QTextEdit *le;

    //KActions
    KAction *openAction;
    KAction *closeAction;
    KAction *reloadAction;
    KAction *quitAction;
    KAction *prevPageAction;
    KAction *nextPageAction;
    KAction *zoominAction;
    KAction *zoomoutAction;
    KSelectAction *zoomSelectAction;
    KSelectAction *pageSelectAction;


    bool nextPageExists();
    bool prevPageExists();
    void updateActions();
    QRectF fixRelativePos(QRectF rect);

    //Dock widgets
    FindDockWidget *fdw;
    QSplitter *splitter;
    QTreeWidget *tree;

private slots:
    void sendDocumentPointer();
    void tocClicked(QTreeWidgetItem *item,int col);
};

#endif // MAINWINDOW_H
