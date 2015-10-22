#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

///
///
/// CALIBRATION
///
///


void MainWindow::on_apply_clicked()
{

}

void MainWindow::on_reset_clicked()
{
    ui->di_nrLevel->setValue(0);
    ui->di_volumeLevel->setValue(0);
    ui->di_ampliLevel->setValue(0);
    ui->di_signalEcho->setValue(0);
    ui->hs_balanceLevel->setValue(50);
    ui->sb_bufferSize->setValue(0);
    ui->sb_sampleRate->setValue(0);
    ui->pb_left->setValue(0);
    ui->pb_right->setValue(0);
}

void MainWindow::on_pb_right_valueChanged(int v)
{
    /*int value = (ui->lcdNumber->value() +
                 ui->lcdNumber_2->value() +
                 ui->lcdNumber_3->value() +
                 ui->lcdNumber_4->value()) / 4 +
                ui->sb_bufferSize->value() / 2 +
                ui->sb_sampleRate->value() / 2;
    value = value * ui->hs_balanceLevel->value() / 100;
    ui->pb_right->setValue(value);*/
}

void MainWindow::on_pb_left_valueChanged(int v)
{
    /*int value = (ui->lcdNumber->value() +
                 ui->lcdNumber_2->value() +
                 ui->lcdNumber_3->value() +
                 ui->lcdNumber_4->value()) / 4 +
                ui->sb_bufferSize->value() / 2 +
                ui->sb_sampleRate->value() / 2;

    value = value * (100 - ui->hs_balanceLevel->value()) / 100;
    ui->pb_left->setValue(value);*/
}

void MainWindow::on_pushButton_5_clicked()
{

}

void MainWindow::on_pushButton_4_clicked()
{
    /*QMessageBox msgBox;
    msgBox.setText("The configuration is going to be set.");
    msgBox.setInformativeText("Do you want to apply your changes?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();*/
}

///
///
/// SONAR
///
///
void MainWindow::on_pushButton_7_clicked()
{
    int selected = ui->listWidget_2->currentRow();
    if (selected == 0)
    {
        ui->dial_5->setValue(30);
        ui->dial_6->setValue(40);
        ui->dial_7->setValue(50);
        ui->checkBox->setChecked(true);
        ui->checkBox_2->setChecked(true);
        ui->checkBox_3->setChecked(false);
        ui->checkBox_4->setChecked(true);
    }
    else if (selected == 1)
    {
        ui->dial_5->setValue(25);
        ui->dial_6->setValue(89);
        ui->dial_7->setValue(60);
        ui->checkBox->setChecked(false);
        ui->checkBox_2->setChecked(false);
        ui->checkBox_3->setChecked(false);
        ui->checkBox_4->setChecked(true);}
    else if (selected == 2)
    {
        ui->dial_5->setValue(50);
        ui->dial_6->setValue(50);
        ui->dial_7->setValue(50);
        ui->checkBox->setChecked(true);
        ui->checkBox_2->setChecked(true);
        ui->checkBox_3->setChecked(true);
        ui->checkBox_4->setChecked(true);}
    else if (selected == 3)
    {
        ui->dial_5->setValue(90);
        ui->dial_6->setValue(85);
        ui->dial_7->setValue(92);
        ui->checkBox->setChecked(false);
        ui->checkBox_2->setChecked(true);
        ui->checkBox_3->setChecked(false);
        ui->checkBox_4->setChecked(false);
    }
}

void MainWindow::on_pushButton_6_clicked()
{
    QMessageBox msgBox;
    msgBox.setText("The configuration has been modified.");
    msgBox.setInformativeText("Do you want to save your changes?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();
}

///
///
/// SONOBUOY
///
///
void MainWindow::on_pushButton_clicked()
{
    QString separator = "_";
    QString id;
    id = "BUOY_type" +
         ui->comboBoxBuoyType->currentText() + separator +
         ui->spinBox->text() + separator +
         ui->spinBox_2->text() + separator +
         ui->spinBox_3->text() + separator +
         ui->spinBox_4->text() + separator +
         ui->spinBox_5->text() + separator +
         ui->spinBox_6->text() + separator +
         ui->spinBox_7->text() + separator +
         ui->spinBox_8->text() + ";";

    ui->listWidget->addItem(id);

}

void MainWindow::on_pushButton_2_clicked()
{
    ui->comboBoxBuoyType->setCurrentIndex(0);
    ui->spinBox->setValue(0);
    ui->spinBox_2->setValue(0);
    ui->spinBox_3->setValue(0);
    ui->spinBox_4->setValue(0);
    ui->spinBox_5->setValue(0);
    ui->spinBox_6->setValue(0);
    ui->spinBox_7->setValue(0);
    ui->spinBox_8->setValue(0);
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->listWidget->takeItem(ui->listWidget->currentRow());
}

int balance = 0;
void MainWindow::on_hs_balanceLevel_sliderMoved(int balance)
{
    //control.setBalanceLevel(balance);
    //power = control.calculatePowerOutput();
    //setChannelsValue(power.left, power.right);
    //ui->pb_left->setValue(power.left);
}
