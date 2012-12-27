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
    //this->setCentralWidget(ui->scrollArea);
    tree = new QTreeWidget(this);
    tree->setColumnCount(2);
    tree->header()->hide();
    tree->header()->setStretchLastSection(false);
    tree->header()->setResizeMode(0,QHeaderView::Stretch);
    tree->header()->setResizeMode(1,QHeaderView::ResizeToContents);
    splitter = new QSplitter(this);
    //this->setCentralWidget(splitter);
    splitter->addWidget(tree);
    splitter->addWidget(ui->scrollArea);
    splitter->setStretchFactor(0,0);
    splitter->setStretchFactor(1,1);

    ui->horizontalLayout->addWidget(splitter);
    mDoc = new Document();

    setupActions();
    setupTextDock();
    updateActions();
    zoomRatioIndex = zoomResetIndex;
    setupGUI(Default,"pdfviewerui.rc");
    fdw = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showPage(int index, int zoomRatioIndex)
{
    if(zoomRatioIndex == -1){
        zoomRatioIndex = this->zoomRatioIndex;
    }
    QPixmap pxmap = QPixmap::fromImage(mDoc->renderPage(index,zoomRatios[zoomRatioIndex]));
    mPageView->setCurrentPixmap(pxmap);
    ui->scrollArea->verticalScrollBar()->setValue(0);
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
    showTOC();
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
    showPage(mDoc->currentPageIndex(),++zoomRatioIndex);
}

void MainWindow::zoomoutSlot()
{
    showPage(mDoc->currentPageIndex(),--zoomRatioIndex);
}

void MainWindow::zoomSelectChanged()
{
    int index = zoomSelectAction->currentItem();
    zoomRatioIndex = index;
    showPage(mDoc->currentPageIndex(),index);
}

void MainWindow::zoomresetSlot()
{
    showPage(mDoc->currentPageIndex(),zoomResetIndex);
}

void MainWindow::showFindDock()
{
    if(!fdw){
        fdw = new FindDockWidget(this);
        this->addDockWidget(Qt::BottomDockWidgetArea,fdw);
        connect(fdw,SIGNAL(search(QString)),SLOT(forwardSearch(QString)));
        connect(fdw,SIGNAL(nextClicked(QString)),SLOT(forwardSearch(QString)));
        connect(fdw,SIGNAL(prevClicked(QString)),SLOT(backwardSearch(QString)));
    }

    // Show findDock only if a file is open
    if(!mPageView->pixmap()){
        fdw->hide();
    } else {
        fdw->show();
    }

}

void MainWindow::forwardSearch(QString text)
{
    QRectF result;
    if(fdw->isFullDocumentSearch()){
        result = mDoc->searchDocument(text,Poppler::Page::NextResult,fdw->caseSensitivity());
    } else {
        result = mDoc->searchPage(text,Poppler::Page::NextResult,fdw->caseSensitivity());
    }

    if(result.isEmpty()){
        return;
    }

    this->showPage(mDoc->currentPageIndex());
    mPageView->highlight(result);
    ui->scrollArea->ensureVisible(result.x(),result.y(),50,mPageView->height()/4);
}

void MainWindow::backwardSearch(QString text)
{
    QRectF result;
    if(fdw->isFullDocumentSearch()){
        result = mDoc->searchDocument(text,Poppler::Page::PreviousResult,fdw->caseSensitivity());
    } else {
        result = mDoc->searchPage(text,Poppler::Page::PreviousResult,fdw->caseSensitivity());
    }

    if(result.isEmpty()){
        return;
    }

    this->showPage(mDoc->currentPageIndex());
    mPageView->highlight(result);
}

void MainWindow::showSelectedText(QRectF rect)
{
//    QString text = mDoc->selectionText(rect);
//    le->setText(text);
//    textDock->show();
}

void MainWindow::setupActions()
{
    KStandardAction::open(this,SLOT(openSlot()),actionCollection());
    prevPageAction = KStandardAction::documentBack(this,SLOT(prevPageSlot()),actionCollection());
    nextPageAction = KStandardAction::documentForward(this,SLOT(nextPageSlot()),actionCollection());

    zoominAction = KStandardAction::zoomIn(this,SLOT(zoominSlot()),actionCollection());
    zoomSelectAction = new KSelectAction(this);
    zoomSelectAction->setItems(QStringList() << "25%"<< "50%"<< "75%"<< "100%"<< "125%"<< "150%"<< "175%"<< "200%"<< "250%"<< "300%" << "350%"<< "400%");
    zoomSelectAction->setCurrentItem(zoomResetIndex);
    connect(zoomSelectAction,SIGNAL(triggered(int)),SLOT(zoomSelectChanged()));
    actionCollection()->addAction("zoom_select",zoomSelectAction);
    zoomoutAction = KStandardAction::zoomOut(this,SLOT(zoomoutSlot()),actionCollection());

    KStandardAction::find(this,SLOT(showFindDock()),actionCollection());
    KStandardAction::quit(kapp,SLOT(quit()),actionCollection());


}

void MainWindow::setupTextDock()
{
    le = new QTextEdit(this);
    le->setReadOnly(true);
    textDock = new QDockWidget("Selected text",this);
    textDock->setObjectName("select_text");
    textDock->setFloating(false);
    textDock->setAllowedAreas(Qt::BottomDockWidgetArea);
    textDock->setWidget(le);
    this->addDockWidget(Qt::BottomDockWidgetArea,textDock);
    connect(mPageView,SIGNAL(rubberBandSelection(QRectF)),SLOT(showSelectedText(QRectF)));

    connect(mPageView,SIGNAL(getDocumentPointer()),SLOT(sendDocumentPointer()));
}

void MainWindow::showTOC()
{
    mDoc->showTOC(tree,0);
    connect(tree,SIGNAL(itemClicked(QTreeWidgetItem*,int)),SLOT(tocClicked(QTreeWidgetItem*,int)));
}

void MainWindow::tocClicked(QTreeWidgetItem *item, int col)
{
    int pageNumber = item->text(1).toInt();
    showPage(--pageNumber);
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
        zoomSelectAction->selectableActionGroup()->setEnabled(false);
        textDock->close();
    } else {
        nextPageExists();
        prevPageExists();
        zoomSelectAction->selectableActionGroup()->setEnabled(true);
        zoomoutAction->setEnabled((zoomRatioIndex==0)?false:true);
        zoominAction->setEnabled((zoomRatios[zoomRatioIndex]==4)?false:true);
        zoomSelectAction->setCurrentItem(zoomRatioIndex);
    }
}

QRectF MainWindow::fixRelativePos(QRectF rect)
{
    QRectF r(rect);
    // This need to be implemented using Qmatrix
    //    r.moveLeft(r.left() + (mPageView->width() - currentPixmap.width()) / 2.0);
    //    r.moveTop(r.top() + (mPageView->height() - currentPixmap.height()) / 2.0);
    return r;
}

void MainWindow::sendDocumentPointer()
{
    mPageView->setDocumentPointer(mDoc);
}
