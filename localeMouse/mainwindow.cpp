#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qstring.h"
#include <QWidget>
#include <QDebug>
#include <QTimer>
#include <QFile>

#include <QX11Info>
#include <X11/Xlib.h>

#include <X11/extensions/Xcomposite.h>
#include <X11/extensions/Xfixes.h>
#include <X11/extensions/shape.h>

#include <XKeyboard.h>
#include <XKeyboard.cpp>


static Display *display;
static XFixesCursorImage *xfcursorImage;
static bool hidden = false;


mainWindow::mainWindow(QWidget *parent):QMainWindow(parent), ui(new Ui::mainWindow) {
    qDebug()<< "say work!";
    display = QX11Info::display();


    //setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground);//full transparent bkg
    setAttribute(Qt::WA_TransparentForMouseEvents, true);
    setAttribute(Qt::WA_QuitOnClose);

    setWindowFlags(
        Qt::FramelessWindowHint |
        Qt::WindowStaysOnTopHint |
        /*Qt::SubWindow|*/
        /*Qt::Dialog|*/
        Qt::ToolTip//hide from taskbar, use as tooltip (Qt::Tool - hard step moving)
    );
    setCursor(Qt::CrossCursor);
    ui->setupUi(this);

    //qApp->installEventFilter(this);

    //first init
    //updateWindow();
    //updateLang();

    QTimer* moveTimer = new QTimer(this);
    connect(moveTimer, SIGNAL(timeout()), this, SLOT(updateWindow()));
    moveTimer->start(1000/60);//TODO: vsync?

    QTimer* langTimer = new QTimer(this);
    connect(langTimer, SIGNAL(timeout()), this, SLOT(updateLang()));
    langTimer->start(1000/15);



    QWidget::show();
}



static QString oldCurType = "old",
               curType    = "new";


static QString oldLang    = "old",
               lang       = "now",
               langFull   = "full";


static int posX = 0, posY = 0,
           oldX = 0, oldY = 0;


static bool langChanged = false,
            moved       = false,
            curChanged  = false;
static bool bypass = false;
void mainWindow::updateWindow() {

    QPoint p = QCursor::pos();
    posX = p.x();
    posY = p.y();

    if(posX != oldX || posY != oldY) {
        //qDebug() << "changing pos: " << posX << "x" << posY;
        oldX = posX;
        oldY = posY;
        moved = true;
        if (bypass == true){ //сброс флажка, посчитал что лучше на следующем цикле скидывать что бы не сломать логику в конце
            curChanged = true;
            bypass     = false;
            moved      = false; //что бы не дергался когда курсор убираешь
        }
    }//end if


    if(langChanged == true ){//принудительно обрабатываем смену курсора (а так же, что бы не ломать остальную логику, то вешаем флажок на смену и курсора в тч.)
        bypass     = true;
        curChanged = true;
    }

    //когда курсор сменился
    if(curChanged == true ) {
        //ui->cursorLabel->setText(curType);

        if(bypass == false && (curType == "blank" || curType == "") && (oldCurType == "xterm" || oldCurType == "ibeam" || oldCurType == "text")) { //TODO using lang update...  this updating lags
            //TODO: сделать проверку на байпас не здесь (курсор пропадает раньше чем сменяется язык), а задать тут флаг что сменился курсор
            //и из функции детекта смены языка производить манипуляции. я боюсь, что при запуске чего то.. скажем полноэкранного в консоли получится флаг по экрану летающий
            bypass = true;//working in 99%
            moved      = false;
            qDebug() << "∟cancel hiding after switch(?):" << oldCurType << "->" << curType << endl;//cancel hiding flag after layout switch in textbox and changing cursor to blank
        }

        if(bypass || curType == "xterm" || curType == "text" || curType == "ibeam" ) {
            hidden = false;
        } else {
            hidden = true;
        }
    }//end if

    //курсор переместился
    if(moved == true) {
        setGeometry(posX+10, posY-25,16,16);//+10 right -25 do down //TODO: хорошо бы учитывать грани экрана, но чувствую работать будет как прячузтй окна таскбар в мате и кедах
    }

    //смена состояния окна
    if(moved == true || curChanged == true) {
        if(hidden != true) {
            //setWindowOpacity(0.85);//legacy or for advanced WM
            //ui->inTextCheckBox->setChecked(true);
            show();
        } else {
            //setGeometry(posX+5, posY-5-17, 1, 1);//legacy
            //setWindowOpacity(0.0);//legacy or for advanced WM
            //ui->inTextCheckBox->setChecked(false);
            hide();
        }
    }//end if

    //если был сменён язык - смена флага
    if(langChanged == true) {
        //ui->langFullLabel->setText(langFull);
        //ui->langLabel->setText(lang);

        if(!QFile::exists(QString(":/res/%1.png").arg(lang))) {
            qDebug() << "lang not exist: " << lang << "full:" << langFull;
            lang = "empty";
            //ui->inTextCheckBox->setChecked(false);
            hide();
        }
        ui->flagLabel->setPixmap( QPixmap( QString(":/res/%1.png").arg(lang) ) );//TODO: придумать чем заменить и как паки реализовать

    }//end if

    //reset feature bool flags.. keep using for advanced logic...
    curChanged  = false;
    moved       = false;
    langChanged = false;
}


void mainWindow::updateLang() {
    if(langChanged != true) {

        XKeyboard xkb= XKeyboard();
        if(xkb.groupCount() < 2) {

            qDebug() << "fuck u";
            qDebug() << "groupCount()"        <<                        xkb.groupCount();
            qDebug() << "currentGroupNum"     <<                        xkb.currentGroupNum();
            qDebug() << "currentGroupVariant" << QString::fromStdString(xkb.currentGroupVariant());
            qDebug() << "currentGroupName"    << QString::fromStdString(xkb.currentGroupName());
            qDebug() << "currentGroupSymbol"  << QString::fromStdString(xkb.currentGroupSymbol());

            XKeyboard xkb = XKeyboard();
            return;
        }



        lang = QString::fromStdString(xkb.currentGroupSymbol()); //return somethings like "us" //need optimize? or 10per sec is ok?
        if(oldLang != lang) {
            langFull = QString::fromStdString(xkb.currentGroupName()); //return somethings like "English"
            qDebug() << "lang changed: " << oldLang << "to" << lang << "full:" << langFull;
            langChanged = true;
            oldLang = lang;
        }
    }

    if(curChanged != true) {
        //update cursor
        xfcursorImage = XFixesGetCursorImage(display);
        XFree (xfcursorImage);//prevent memory leak

        //TODO: not use too many qstrings... but i want to sleep
        QString tmpNowCurType = curType;
        QString newCurType    = xfcursorImage->name;
        if(tmpNowCurType != newCurType) {
            qDebug() << "changed: from" << curType << "to" << newCurType << ". PS: old used is:" << tmpNowCurType;
            curChanged = true;
            curType    = newCurType;//???
            oldCurType = tmpNowCurType;
        }
    }
}


mainWindow::~mainWindow() {
    qDebug() << "Goodbye!";
    delete ui;
}
