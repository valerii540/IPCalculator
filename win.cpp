#include "win.h"
#include "ui_win.h"
#include <iostream>
#include <bitset>
using namespace std;

win::win(QWidget *parent) :QMainWindow(parent), ui(new Ui::win)
{
    ui->setupUi(this);
    ui->lineEdit_Mask->setText("24");
    ui->lineEdit_IP->setText("192.168.32.255");

    connect(ui->pushButton_calc, SIGNAL(released()), this, SLOT(press_calculate()));


}

win::~win()
{
    delete ui;
}

void win::press_calculate()
{
    mask = ui->lineEdit_Mask->text().toInt();


    QStringList IPdecimal = ui->lineEdit_IP->text().split('.');
    QStringList IPbinaryList;

    // ########### BINARIZE OCTETS ################
    foreach (QString ip, IPdecimal)
    {
        IPbinaryList.append(QString::fromStdString(bitset<8>(ip.toInt()).to_string()));
    }
    // ############################################

    // ########## DETERMINE ADDRESS TYPE ##########
    QString IPbinary = IPbinaryList.join("");

    determineType(IPbinary);
    // #############################################

    determineNet(IPbinary);



}

void win::determineType(QString& IPbinary)
{

    QString hostPortion = QStringRef(&IPbinary, mask, 32-mask).toUtf8();
    if (hostPortion.contains("0") && hostPortion.contains("1"))
        ui->label_type->setText("Host");
    else if(hostPortion.contains("1") && !hostPortion.contains("0"))
        ui->label_type->setText("Broadcast");
    else
        ui->label_type->setText("Network");
}

void win::determineNet(QString& IPbinary)
{
    //
    QString netAddress = QStringRef(&IPbinary, 0, mask).toString();

    qDebug("Net bits: " + netAddress.toUtf8());

    netAddress += QString("0").repeated(32-mask);

    qDebug("Net address: " + netAddress.toUtf8());

    QStringList netAddressList;
    for (int i = 0; i < 32; i+=8)
    {
        QString intOctet = QString::number(QStringRef(&netAddress, i, 8).toString().toInt(nullptr, 2));
        netAddressList.append(intOctet);
    }


    foreach (auto octet, netAddressList)
    {
        qDebug(octet.toUtf8());
    }

    ui->label_net->setText(netAddressList.join("."));



}


