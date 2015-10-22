#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;

private slots:
    void on_pushButton_3_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_7_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pb_right_valueChanged(int value);
    void on_pb_left_valueChanged(int value);
    void on_hs_balanceLevel_sliderMoved(int position);
    void on_apply_clicked();
    void on_reset_clicked();
};

#endif // MAINWINDOW_H
