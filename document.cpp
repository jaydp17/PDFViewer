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

void Document::fillTOC(const QDomNode &parent, QTreeWidget *tree, QTreeWidgetItem *parentItem)
{
    QTreeWidgetItem *newItem = 0;
    QDomNode node = parent.firstChild();
    for(; !node.isNull(); node=node.nextSibling()){
        QDomElement elem = node.toElement();
        if(!parentItem){
            newItem = new QTreeWidgetItem(tree,newItem);
        } else {
            newItem = new QTreeWidgetItem(parentItem,newItem);
        }
        newItem->setText(0,elem.tagName());
        QString destination = elem.attribute(QString::fromLatin1("Destination"));
        if(!destination.isEmpty()){
            QStringList crap = destination.split(";");
            newItem->setText(1,crap[1]);
        }
//        qDebug() << "constins Destination : " << elem.hasAttribute("Destination");
//        qDebug() << "constins DestinationName : " << elem.hasAttribute("DestinationName");
//        qDebug() << "constins ExternalFileName : " << elem.hasAttribute("ExternalFileName");
//        qDebug() << "constins Open : " << elem.hasAttribute("Open");

        if(elem.hasChildNodes()){
            fillTOC(node,tree,newItem);
        }
    }
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

QList<QRectF> Document::selectAllInBetween(QList<Poppler::TextBox *> textList, int index, QRectF fullselection)
{
    QList<QRectF> list;
    Poppler::TextBox *prevBox = textList.at(--index);
    fullselection.setWidth(prevBox->boundingBox().right() - fullselection.left());
    list.append(fullselection);
    return list;
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

QList<QRectF> Document::selectionText(QRectF rect)
{
    QString text = "";
    QList<QRectF> selection;
    QPointF center;
    QRectF fullselection;
    QList<Poppler::TextBox*> textList = pDoc->page(currentPageIndex())->textList();

    foreach(Poppler::TextBox *txtBox, textList){
        if(rect.intersects(txtBox->boundingBox())){
            for(int i=0;i<txtBox->text().size();i++){
                QRectF charBox = txtBox->charBoundingBox(i);
                if(rect.intersects(charBox)){
                    // check on the starting of the word of it's a word on next line
                    if(i == 0){
                        if(!center.isNull() && txtBox->boundingBox().center().y() > center.y()){
                            text += "\n";
                            QList<QRectF> inBetween = selectAllInBetween(textList,textList.indexOf(txtBox),fullselection);
                            selection.append(inBetween);
                            fullselection = QRectF();
                        }
                        if(fullselection.isEmpty()){
                            fullselection = QRectF(txtBox->boundingBox());
                        }
                        center = txtBox->boundingBox().center();
                    }

                    text += txtBox->text().at(i);
                    fullselection.setWidth(charBox.right() - fullselection.left());

                    // check on the end of the word if it has a space after the word
                    if((i+1) == txtBox->text().size()){
                        if(txtBox->hasSpaceAfter()){
                            text += " ";
                        }
                    }
                }
            }
        }
    }
    selection.append(fullselection);
    mSelectedText = text;
    return selection;
}

QString Document::selectedText()
{
    return mSelectedText;
}

void Document::showTOC(QTreeWidget *tree, QTreeWidgetItem *parentItem)
{
    QDomDocument *toc = pDoc->toc();
    if(!toc && toc->isNull()){
        qDebug() << "toc NULL POINTER";
        return;
    }
    this->fillTOC(toc->cloneNode(true),tree,parentItem);
}

