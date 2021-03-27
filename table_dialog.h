#ifndef TABLE_DIALOG_H
#define TABLE_DIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QTableWidget>
#include "mainwindow.h"

namespace Ui {
class table_dialog;
}

class table_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit table_dialog(analyzer *alz, QWidget *parent = nullptr);
    ~table_dialog();

private slots:
    void on_save_button_clicked();

private:
    Ui::table_dialog *ui;
    enum Coluna{
        NODE, LAMBDA, THROUGHPUT,GOODPUT,SENTDATA,JITTERSUM,DROPPED
    };
    void setup_table(analyzer *alz);
};

#endif // TABLE_DIALOG_H
