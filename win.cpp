#include "win.h"
#include "ui_win.h"
#include <iostream>
#include <bitset>
#include <math.h>
using namespace std;

win::win(QWidget *parent) :QMainWindow(parent), ui(new Ui::win)
{
    ui->setupUi(this);
    ui->lineEdit_Mask->setText("24");

    connect(ui->pushButton_compute, SIGNAL(released()), this, SLOT(press_compute()));
}

win::~win()
{
    delete ui;
}

void win::press_compute()
{
    QStringList IP_DecimalList = ui->lineEdit_IP->text().split('.');


    // Binarize decimal octets
    QStringList IPbinaryList;
    foreach (QString ip, IP_DecimalList)
        IPbinaryList.append(QString::fromStdString(bitset<8>(ip.toInt()).to_string()));

    QString IPbinary = IPbinaryList.join("");

    calculate(IPbinary);
}

void win::calculate(QString& IPbinary)
{
    unsigned short int mask = ui->lineEdit_Mask->text().toInt();

    // ################## ADDRESS TYPE ################
    QString hostPortion = QStringRef(&IPbinary, mask, 32-mask).toUtf8();

    if (hostPortion.contains("0") && hostPortion.contains("1"))
        ui->label_type->setText("Host");
    else if(hostPortion.contains("1") && !hostPortion.contains("0"))
        ui->label_type->setText("Broadcast");
    else
        ui->label_type->setText("Network");

    // ################## HOSTS QUANTITY ###############
    ui->label_hostQuantity->setText(QString::number(pow(2, 32-mask) - 2));


    // Determine network portion
    QString netPortion = QStringRef(&IPbinary, 0, mask).toString();

    // ############## NETWORK ADDRESS ##################
    QString netAddress = netPortion + QString("0").repeated(32-mask);
    netAddress = binaryOctetsToDecimal(netAddress);
    ui->label_net->setText(netAddress);


    //############# FIRST HOST ADDRESS ##################
    QString firstHostAddress = netPortion + QString("0").repeated(32-mask-1) + "1";
    firstHostAddress = binaryOctetsToDecimal(firstHostAddress);
    ui->label_FH->setText(firstHostAddress);


    //############## LAST HOST ADDRESS ###################
    QString lastHostAddress = netPortion + QString("1").repeated(32-mask-1) + "0";
    lastHostAddress = binaryOctetsToDecimal(lastHostAddress);
    ui->label_LH->setText(lastHostAddress);

    //################ BROADCAST RANGE ####################
    QString broadcastAddress = netPortion + QString("1").repeated(32-mask);
    broadcastAddress = binaryOctetsToDecimal(broadcastAddress);
    ui->label_broad->setText(broadcastAddress);

    //################ FULL MASK ##########################
    QString fullMask = QString("1").repeated(mask) + QString("0").repeated(32-mask);
    fullMask = binaryOctetsToDecimal(fullMask);
    ui->label_mask->setText(fullMask);
}

QString win::binaryOctetsToDecimal(QString& IPbinary)
{
    QStringList netAddressList;
    for (int i = 0; i < 32; i+=8)
    {
        QString intOctet = QString::number(QStringRef(&IPbinary, i, 8).toString().toInt(nullptr, 2));
        netAddressList.append(intOctet);
    }

    return netAddressList.join(".");
}

