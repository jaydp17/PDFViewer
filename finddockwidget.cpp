#include "finddockwidget.h"
#include "ui_finddockwidget.h"

FindDockWidget::FindDockWidget(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::FindDockWidget)
{
    ui->setupUi(this);
    connect(ui->searchBox,SIGNAL(returnPressed()),SLOT(searchReturnPressed()));
    connect(ui->btnPrev,SIGNAL(clicked()),SLOT(searchPrev()));
    connect(ui->btnNext,SIGNAL(clicked()),SLOT(searchNext()));
    connect(ui->casesensitive,SIGNAL(toggled(bool)),SLOT(caseSensChanged(bool)));
    connect(ui->fulldoc,SIGNAL(toggled(bool)),SLOT(fullDocChanged(bool)));

}

FindDockWidget::~FindDockWidget()
{
    delete ui;
}

Poppler::Page::SearchMode FindDockWidget::caseSensitivity()
{
    if(ui->casesensitive->isChecked()){
        return Poppler::Page::CaseSensitive;
    } else {
        return Poppler::Page::CaseInsensitive;
    }
}

bool FindDockWidget::isFullDocumentSearch()
{
    return ui->fulldoc->isChecked();
}

void FindDockWidget::show()
{
    // Grabs focus
    ui->searchBox->setFocus();
    QDockWidget::show();
}


void FindDockWidget::searchReturnPressed()
{
    emit search(ui->searchBox->text());
}

void FindDockWidget::searchNext()
{
    emit nextClicked(ui->searchBox->text());
}


void FindDockWidget::searchPrev()
{
    emit prevClicked(ui->searchBox->text());
}

void FindDockWidget::caseSensChanged(bool checked)
{
    emit caseSensToggled(checked);
}

void FindDockWidget::fullDocChanged(bool checked)
{
    emit fullDocToggled(checked);
}
