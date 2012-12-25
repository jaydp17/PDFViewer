#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

double zoomRatios[] = {0.25, 0.5, 0.75, 1, 1.25, 1.5, 1.75, 2, 2.5, 3 , 3.5, 4};
int zoomResetIndex = 3;

MainWindow::MainWindow(QWidget *parent) :
    KXmlGuiWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mPageView = new PageView;
    ui->scrollArea->setWidget(mPageView);
    ui->scrollArea->setBackgroundRole(QPalette::Dark);
    this->setCentralWidget(ui->scrollArea);
    mDoc = new Document();

    setupActions();
    updateActions();
    zoomRatioIndex = zoomResetIndex;
    setupGUI(Default,"pdfviewerui.rc");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showPage(int index, int zoomRatioIndex)
{
    mPageView->setPixmap(QPixmap::fromImage(mDoc->renderPage(index,zoomRatios[zoomRatioIndex])));
    updateActions();
}

void MainWindow::openSlot()
{
    QString filename = QFileDialog::getOpenFileName(this,"Select a pdf file",QDir::homePath(),"PDF (*.pdf)");
    if(!filename.isEmpty())
        openPDF(filename);
}

bool MainWindow::openPDF(QString filename)
{
    mDoc->openFile(filename);
    showPage(0);
    // return false if some problem occurs
    return true;
}

void MainWindow::nextPageSlot()
{
    showPage(mDoc->currentPageIndex()+1,zoomRatioIndex);
}

void MainWindow::prevPageSlot()
{
    showPage(mDoc->currentPageIndex()-1,zoomRatioIndex);
}

void MainWindow::zoominSlot()
{
    if(zoomRatios[++zoomRatioIndex] < 4){
        showPage(mDoc->currentPageIndex(),zoomRatioIndex);
        zoominAction->setEnabled(true);
    } else {
        showPage(mDoc->currentPageIndex(),zoomRatioIndex);
        zoominAction->setEnabled(false);
    }
}

void MainWindow::zoomoutSlot()
{
    if(zoomRatios[--zoomRatioIndex] > 0.25){
        showPage(mDoc->currentPageIndex(),zoomRatioIndex);
        zoomoutAction->setEnabled(true);
    } else {
        showPage(mDoc->currentPageIndex(),zoomRatioIndex);
        zoomoutAction->setEnabled(false);
    }
}

void MainWindow::zoomresetSlot()
{
}

void MainWindow::setupActions()
{
    KStandardAction::open(this,SLOT(openSlot()),actionCollection());
    prevPageAction = KStandardAction::documentBack(this,SLOT(prevPageSlot()),actionCollection());
    nextPageAction = KStandardAction::documentForward(this,SLOT(nextPageSlot()),actionCollection());
    zoominAction = KStandardAction::zoomIn(this,SLOT(zoominSlot()),actionCollection());
    zoomoutAction = KStandardAction::zoomOut(this,SLOT(zoomoutSlot()),actionCollection());
}

bool MainWindow::nextPageExists()
{
    if(mDoc->currentPageIndex()+1 < mDoc->numPages()){
        nextPageAction->setEnabled(true);
        return true;
    } else {
        nextPageAction->setEnabled(false);
        return false;
    }
}


bool MainWindow::prevPageExists()
{
    if(mDoc->currentPageIndex()-1 >= 0){
        prevPageAction->setEnabled(true);
        return true;
    } else {
        prevPageAction->setEnabled(false);
        return false;
    }
}

void MainWindow::updateActions()
{
    if(!mPageView->pixmap()){
        prevPageAction->setEnabled(false);
        nextPageAction->setEnabled(false);
        zoominAction->setEnabled(false);
        zoomoutAction->setEnabled(false);
    } else {
        nextPageExists();
        prevPageExists();
        zoomoutAction->setEnabled(zoomRatios[--zoomRatioIndex] > 0.25);
        zoominAction->setEnabled(zoomRatios[++zoomRatioIndex] < 4);
    }
}
