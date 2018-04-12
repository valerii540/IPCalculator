#ifndef WIN_H
#define WIN_H

#include <QMainWindow>

namespace Ui {
class win;
}

class win : public QMainWindow
{
    Q_OBJECT

public:
    explicit win(QWidget *parent = 0);
    ~win();

private:
    Ui::win *ui;

    void calculate(QString&);
    QString binaryOctetsToDecimal(QString&);
    bool checkInput(QStringList&);

private slots:
    void press_compute();
};

#endif // WIN_H
