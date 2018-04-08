#include "win.h"
#include "ui_win.h"
#include <iostream>
#include <bitset>
using namespace std;

win::win(QWidget *parent) :QMainWindow(parent), ui(new Ui::win)
{
    ui->setupUi(this);
    ui->lineEdit_Mask->setText("24");
    //ui->lineEdit_IP->setText("192.168.32.255");

    connect(ui->pushButton_calc, SIGNAL(released()), this, SLOT(press_calculate()));
}

win::~win()
{
    delete ui;
}

void win::press_calculate()
{
    // Get mask(CIDR) and IP from UI
    mask = ui->lineEdit_Mask->text().toInt();
    IP_DecimalList = ui->lineEdit_IP->text().split('.');


    // Binarize decimal octets
    QStringList IPbinaryList;
    foreach (QString ip, IP_DecimalList)
        IPbinaryList.append(QString::fromStdString(bitset<8>(ip.toInt()).to_string()));

    // Join binarized octets
    QString IPbinary = IPbinaryList.join("");

    // Determine IP type
    determineType(IPbinary);

    // Determine Network address
    determineNet(IPbinary);

    //TODO: 1 add determining First host address
    //TODO: 2 add determining Last host address
    //TODO: 3 add determining Broadcast address
    //TODO: 4 add determining Dot-Decimal Subnet Mask address
}

void win::determineType(QString& IPbinary)
{
    // Determine host portion
    QString hostPortion = QStringRef(&IPbinary, mask, 32-mask).toUtf8();

    // Check host portion and output the result
    if (hostPortion.contains("0") && hostPortion.contains("1"))
        ui->label_type->setText("Host");
    else if(hostPortion.contains("1") && !hostPortion.contains("0"))
        ui->label_type->setText("Broadcast");
    else
        ui->label_type->setText("Network");

    //TODO: 99 add a validation check -> add "not valid" branch
}

void win::determineNet(QString& IPbinary)
{
    // Determine network portion
    QString netAddress = QStringRef(&IPbinary, 0, mask).toString();

    // Add host portion as zeros
    netAddress += QString("0").repeated(32-mask);

    // Spliting into decimal ocgtets
    QStringList netAddressList;
    for (int i = 0; i < 32; i+=8)
    {
        QString intOctet = QString::number(QStringRef(&netAddress, i, 8).toString().toInt(nullptr, 2));
        netAddressList.append(intOctet);
    }

    // Output the result
    ui->label_net->setText(netAddressList.join("."));
}


