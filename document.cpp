#include "document.h"
#include <QScopedPointer>
#include <QApplication>
#include <QDebug>

Document::Document()
{
}

Document::~Document()
{
    delete pDoc;
}

QString Document::currentFileName()
{
    return mFilename;
}

int Document::currentPageIndex()
{
    return currentPage;
}

bool Document::openFile(QString filename)
{
    this->mFilename = filename;
    pDoc = Poppler::Document::load(filename);
    if(!pDoc || pDoc->isLocked()){
        return false;
    }
    pDoc->setRenderHint(Poppler::Document::TextAntialiasing);
    return true;
}

QImage Document::renderPage(int index)
{
    int scale = 100;
    double scaleF = scale/100;
    QImage img = pDoc->page(index)->renderToImage(scaleF * physicalDpiX(), scaleF * physicalDpiY());
    currentPage = index;
    return img;
}

double Document::physicalDpiX()
{
    qDebug() << qApp->desktop()->physicalDpiX();
    return qApp->desktop()->physicalDpiX();
}

double Document::physicalDpiY()
{
    qDebug() << qApp->desktop()->physicalDpiY();
    return qApp->desktop()->physicalDpiY();
}

int Document::numPages(){
    return pDoc->numPages();
}

QRectF Document::searchForward(QString text)
{
    for(int i=0;i<pDoc->numPages();i++){
        if(pDoc->page(i)->search(text,searchLocation, Poppler::Page::NextResult, Poppler::Page::CaseInsensitive)){
            if( !searchLocation.isNull() ){
                currentPage = i;
                return searchLocation;
            }
        }
    }
    return QRectF();
}
