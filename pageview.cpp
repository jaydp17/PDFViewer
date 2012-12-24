#include "pageview.h"

PageView::PageView() : QLabel()
{
    this->setAlignment(Qt::AlignCenter);
}

void PageView::mousePressEvent(QMouseEvent *ev)
{
    origin = ev->pos();
    if(!rubberband){
        rubberband = new QRubberBand(QRubberBand::Rectangle,this);
    }
    rubberband->setGeometry(QRect(origin, QSize()));
    rubberband->show();
}

void PageView::mouseMoveEvent(QMouseEvent *ev)
{
    rubberband->setGeometry(QRect(origin, ev->pos()).normalized());
}

void PageView::mouseReleaseEvent(QMouseEvent *ev)
{
    if(!rubberband->size().isEmpty()){
        QRectF rect(rubberband->rect());
        rect.moveLeft(rect.left() - (width() - pixmap()->width()) / 2.0);
        rect.moveTop(rect.top() - (height() - pixmap()->height()) / 2.0);
        emit rubberBandSelection(rect);
    }
    rubberband->hide();
}


