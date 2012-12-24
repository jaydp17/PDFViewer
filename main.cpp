#include <KApplication>
#include <KAboutData>
#include <KCmdLineArgs>
#include <KCmdLineOptions>
#include "mainwindow.h"

int main(int argc, char *argv[]){
    KAboutData aboutData("pdfviewer","pdfviewer",ki18n("PDFViewer"),"0.1",ki18n("A simple pdf viewer"),KAboutData::License_LGPL_V3);
    KCmdLineArgs::init(argc,argv,&aboutData);

    KApplication app;

    KCmdLineOptions options;
    options.add("+[URL]",ki18n("Document to open"));
    KCmdLineArgs::addCmdLineOptions(options);

    KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
    MainWindow *mw = new MainWindow();
    if(args->count() != 0){
        // Open a document
    }

    mw->show();
    return app.exec();
}

