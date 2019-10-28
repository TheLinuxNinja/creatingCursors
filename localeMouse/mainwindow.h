#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class mainWindow;
}



class mainWindow : public QMainWindow{
    Q_OBJECT

public:
    explicit mainWindow(QWidget *parent = 0);
    ~mainWindow();


private:
    Ui::mainWindow *ui;
protected:
     //bool eventFilter(QObject *obj, QEvent *event);
private:

private slots:
     void updateWindow();
     void updateLang();
};

#endif // MAINWINDOW_H
