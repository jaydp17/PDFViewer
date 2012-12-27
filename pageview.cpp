#include "pageview.h"
#include <QRect>

PageView::PageView() : QLabel()
{
    this->setAlignment(Qt::AlignCenter);
    this->setCursor(QCursor(Qt::IBeamCursor));
    rubberband = 0;
}

void PageView::setDocumentPointer(Document *p)
{
    mDoc = p;
}

void PageView::highlight(QRectF rect)
{
    QPixmap pxmap = currentPxmap;
    QPainter painter(&pxmap);
    QColor highlightColor(Qt::darkBlue);
    highlightColor.setAlpha(125);
    painter.setPen(highlightColor);
    painter.setBrush(highlightColor);
    painter.fillRect(rect,highlightColor);
    this->setPixmap(pxmap);
}

void PageView::highlight(QList<QRectF> list)
{
    QPixmap pxmap = currentPxmap;
    QPainter painter(&pxmap);
    QColor highlightColor(Qt::darkBlue);
    highlightColor.setAlpha(125);
    painter.setPen(highlightColor);
    painter.setBrush(highlightColor);
    foreach(QRectF rect, list){
        painter.fillRect(rect,highlightColor);
    }
    this->setPixmap(pxmap);
}

void PageView::setCurrentPixmap(QPixmap pxmap)
{
    currentPxmap = pxmap;
    this->setPixmap(pxmap);
}

void PageView::mousePressEvent(QMouseEvent *ev)
{
    // checks if a file is open
    if(!pixmap()){
        return;
    }

    emit getDocumentPointer();

    origin = ev->pos();
    if(!rubberband){
        rubberband = new QRubberBand(QRubberBand::Rectangle,this);
    }
    rubberband->setGeometry(QRect(origin, QSize()));
    rubberband->show();
}

void PageView::mouseMoveEvent(QMouseEvent *ev)
{
    if(!pixmap()){
        return;
    }

    rubberband->setGeometry(QRect(origin, ev->pos()).normalized());
    if(!rubberband->size().isEmpty()){
        QRectF rect(rubberband->pos(),rubberband->size());
        double offsetX = ((this->width() - this->pixmap()->width())/2.0);
        double offsetY = ((this->height() - this->pixmap()->height())/2.0);
        rect.moveLeft(rect.left() - offsetX);
        rect.moveTop(rect.top() - offsetY);
        QList<QRectF> boxes = mDoc->selectionText(rect);
        this->highlight(boxes);
    }
}

void PageView::mouseReleaseEvent(QMouseEvent *ev)
{
    if(!pixmap()){
        return;
    }

    if(!rubberband->size().isEmpty()){
        QRectF rect(rubberband->pos(),rubberband->size());
        double offsetX = ((this->width() - this->pixmap()->width())/2.0);
        double offsetY = ((this->height() - this->pixmap()->height())/2.0);
        rect.moveLeft(rect.left() - offsetX);
        rect.moveTop(rect.top() - offsetY);
        emit rubberBandSelection(rect);
        qDebug() << "***************************";
        qDebug() << mDoc->selectedText();
        qDebug() << "***************************";
    } else {
        this->setPixmap(currentPxmap);
    }
    rubberband->hide();
    emit textSelected();
}




