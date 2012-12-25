#include "document.h"
#include <QScopedPointer>
#include <QApplication>
#include <QDebug>
#include <QtConcurrentRun>
#include <KMessageBox>
#include <KApplication>

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

QImage Document::renderPage(int index, double scaleFactor)
{
    QFuture<QImage> future = QtConcurrent::run(this,&Document::renderPrivate,index,scaleFactor);
    return future.result();
}

QImage Document::renderPrivate(int index, double scaleFactor)
{
    double scaleF = scaleFactor;
    //QImage img = pDoc->page(index)->renderToImage(scaleF * physicalDpiX(), scaleF * physicalDpiY());
    QImage img = pDoc->page(index)->renderToImage(scaleF * 72.0, scaleF * 72.0);
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

QRectF Document::searchDocument(QString text, Poppler::Page::SearchDirection direction, Poppler::Page::SearchMode caseSensitivity)
{
    // if it's a new search start from the top of the page
    if(prevSearchtext != text){
        searchLocation = QRectF();
    }

    if(direction == Poppler::Page::NextResult){
        for(int i=currentPage;i<pDoc->numPages();i++){
            if(pDoc->page(i)->search(text,searchLocation, direction, caseSensitivity)){
                if( !searchLocation.isNull() ){
                    currentPage = i;
                    prevSearchtext = text;
                    return searchLocation;
                }
            }
            if(direction == Poppler::Page::NextResult){
                direction = Poppler::Page::FromTop;
            }

        }
    } else{
        for(int i=currentPage;i>=0;i--){
            if(pDoc->page(i)->search(text,searchLocation, direction, caseSensitivity)){
                if( !searchLocation.isNull() ){
                    currentPage = i;
                    prevSearchtext = text;
                    return searchLocation;
                }
            }
        }
    }

    // checking for the end of the document
    if(prevSearchtext == text && direction != Poppler::Page::PreviousResult){
        int response = KMessageBox::questionYesNo(0,"Reached the end of the document. Do you want to continue from the top?","Search Result");
        if(response == KStandardGuiItem::Yes){
            currentPage = 0;
            searchLocation = QRectF();
            prevSearchtext = text;
            return searchDocument(text,direction,caseSensitivity);
        }
    } else {
        KMessageBox::information(0,"No results found :(","Search result");
    }
    prevSearchtext = text;
    return QRectF();
}

QRectF Document::searchPage(QString text, Poppler::Page::SearchDirection direction, Poppler::Page::SearchMode caseSensitivity)
{
    // if it's a new search start from the top of the page
    if(prevSearchtext != text){
        searchLocation = QRectF();
    }

    if(pDoc->page(currentPage)->search(text,searchLocation, direction, caseSensitivity)){
        if( !searchLocation.isNull() ){
            prevSearchtext = text;
            return searchLocation;
        }
    }

    // checking for the end of the document
    if(prevSearchtext == text){
        int response = KMessageBox::questionYesNo(0,"Reached the end of the document. Do you want to continue from the top?","Search Result");
        if(response == KStandardGuiItem::Yes){
            searchLocation = QRectF();
            prevSearchtext = text;
            return searchPage(text,direction,caseSensitivity);
        }
    } else {
        KMessageBox::information(0,"No results found :(","Search result");
    }
    prevSearchtext = text;
    return QRectF();
}

QString Document::selectionText(QRectF rect)
{
    return NULL;
}


