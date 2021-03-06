#include <QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QQuickTextDocument>
#include <QObject>
#include <QIcon>
#include "highlighter.h"
#include "dragdrophandler.h" // dragdrop file handler

int main(int argc, char ** argv)
{
    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/idcplc.ico"));

    QFontDatabase::addApplicationFont(":/Consolas.ttf");
    QQmlApplicationEngine engine(QUrl("qrc:/main.qml"));
    if (engine.rootObjects().isEmpty())
        return -1;

    // Cari sourceView object di qml hirarki untuk dipasangkan highlighter.
    Highlighter *highlighter;
    QObject *root = engine.rootObjects()[0];
    QObject *sourceView = root->findChild <QObject*>("sourceView");
    if (sourceView != NULL)
    {
        QQuickTextDocument *doc = qvariant_cast<QQuickTextDocument*>(sourceView->property("textDocument"));
        if (doc != NULL)
        {
            highlighter = new Highlighter(doc->textDocument());

            // Override tab width menjadi 30 device unit.
            QTextOption textOptions = doc->textDocument()->defaultTextOption();
            textOptions.setTabStop(30);
            doc->textDocument()->setDefaultTextOption(textOptions);
        }
    }

    // untuk dragdrop file
    DragDropHandler ddh(root);
    QObject *dropArea = sourceView->findChild <QObject*>("dropArea");
    QObject::connect(dropArea, SIGNAL(dropSignal(QString)), 
                     &ddh, SLOT(runHandler(QString)));
    
    return app.exec();
}

