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
    clearOutputs();

    QStringList IP_DecimalList = ui->lineEdit_IP->text().split('.');

    // Check input
    if(!checkInput(IP_DecimalList))
    {
        ui->label_net->setText("Not valid input");
        return;
    }

    // Binarize decimal octets
    QStringList IP_BinaryList;
    foreach (QString octet, IP_DecimalList)
        IP_BinaryList.append(QString::fromStdString(bitset<8>(octet.toInt()).to_string()));

    QString IPbinary = IP_BinaryList.join("");

    calculateBase(IPbinary);
    calculateDetails(IP_DecimalList);
}

void win::clearOutputs()
{
    ui->label_type->setText("");
    ui->label_hostQuantity->setText("");
    ui->label_net->setText("");
    ui->label_FH->setText("");
    ui->label_LH->setText("");
    ui->label_broad->setText("");
    ui->label_mask->setText("");

    ui->lineEdit_Kind->setText("");
    ui->lineEdit_Class->setText("");
}

void win::calculateBase(QString& IPbinary)
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

    //#####################################################

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

void win::calculateDetails(QStringList& IP_DecimalList)
{
    // ############## IP KIND ###############
    int mask = ui->lineEdit_Mask->text().toInt();
    int octet_1 = IP_DecimalList[0].toInt();
    int octet_2 = IP_DecimalList[1].toInt();

    /* Private networks:
     * 10.0.0.0/8
     * 172.16-31.0.0/12
     * 192.168.0.0/16
     */
    if((octet_1 == 10 && mask >= 8) ||
            (octet_1 == 172 && octet_2 >=16 && octet_2 <= 31 && mask >= 12) ||
            (octet_1 == 192 && octet_2 == 168 && mask >= 16))
        ui->lineEdit_Kind->setText("Private");
    else
        ui->lineEdit_Kind->setText("Public");
}


bool win::checkInput(QStringList& IP_DecimalList)
{
    // ########### Check mask #############
    int mask = ui->lineEdit_Mask->text().toInt();
    if(mask < 0 || mask > 28){
        qDebug("Mask not valid");
        return false;
    }

    // ############ Check IP #############
    if(IP_DecimalList.length() != 4)
        return false;

    foreach (QString octet_str, IP_DecimalList)
    {
        int octet = octet_str.toInt();
        if(octet < 0 || octet > 255){
            qDebug("IP not valid");
            return false;
        }
    }

    return true;
}
