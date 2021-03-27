#include "table_dialog.h"
#include "ui_table_dialog.h"

table_dialog::table_dialog(analyzer *alz,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::table_dialog)
{
    ui->setupUi(this);
    setup_table(alz);
}

table_dialog::~table_dialog()
{
    delete ui;
}

void table_dialog::setup_table(analyzer *alz)
{
    QStringList header;
    ui->orderTable->setColumnCount(7);
    ui->orderTable->setRowCount(0);
    header << "Node" << "Lambda" << "Throughput"<<"Goodput"<<"Sent Data"<<"Jitter Sum"<<"Dropped Packets";
    ui->orderTable->setHorizontalHeaderLabels(header);

    // insere linha a linha os valores da tabela
    for(int i=0; i<alz->getNodes().size();i++){
        ui->orderTable->insertRow(ui->orderTable->rowCount());
        int currentLine = ui->orderTable->rowCount() -1;

        QTableWidgetItem* itemnode = new QTableWidgetItem;
        itemnode->setData(Qt::DisplayRole,i);
        ui->orderTable->setItem(currentLine,NODE,itemnode);

        QTableWidgetItem* itemlambda = new QTableWidgetItem;
        itemlambda->setData(Qt::DisplayRole,(alz->getLambda(i)));
        ui->orderTable->setItem(currentLine,LAMBDA,itemlambda);

        QTableWidgetItem* itemthrpt = new QTableWidgetItem;
        itemthrpt->setData(Qt::DisplayRole,(alz->getThrpt(i)));
        ui->orderTable->setItem(currentLine,THROUGHPUT,itemthrpt);

        QTableWidgetItem* itemgood = new QTableWidgetItem;
        itemgood->setData(Qt::DisplayRole,(alz->getGood(i)));
        ui->orderTable->setItem(currentLine,GOODPUT,itemgood);

        QTableWidgetItem* itemsent = new QTableWidgetItem;
        int senttotal = alz->getSentDatatotal(i);
        itemsent->setData(Qt::DisplayRole,(senttotal));
        ui->orderTable->setItem(currentLine,SENTDATA,itemsent);


        QTableWidgetItem* itemjitter = new QTableWidgetItem;
        itemjitter->setData(Qt::DisplayRole,(alz->getJitterSum(i)));
        ui->orderTable->setItem(currentLine,JITTERSUM,itemjitter);

        QTableWidgetItem* itemdropped = new QTableWidgetItem;
        itemdropped->setData(Qt::DisplayRole,(alz->getPckt(i)));
        ui->orderTable->setItem(currentLine,DROPPED,itemdropped);

    }
    ui->orderTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}
void table_dialog::on_save_button_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this,tr("Open File"),tr("/home/"),tr("PDF files (*.pdf)"));
    const int columns = ui->orderTable->columnCount();
    const int rows = ui->orderTable->rowCount();
    QTextDocument doc;
    QTextCursor cursor(&doc);
    QTextTableFormat tableFormat;
    tableFormat.setHeaderRowCount(1);
    tableFormat.setAlignment(Qt::AlignHCenter);
    tableFormat.setCellPadding(0);
    tableFormat.setCellSpacing(0);
    tableFormat.setBorder(1);
    tableFormat.setBorderBrush(QBrush(Qt::SolidPattern));
    tableFormat.clearColumnWidthConstraints();
    QTextTable *textTable = cursor.insertTable(rows + 1, columns, tableFormat);
    QTextCharFormat tableHeaderFormat;
    tableHeaderFormat.setBackground(QColor("#DADADA"));
    for (int i = 0; i < columns; i++) {
        QTextTableCell cell = textTable->cellAt(0, i);
        cell.setFormat(tableHeaderFormat);
        QTextCursor cellCursor = cell.firstCursorPosition();
        cellCursor.insertText(ui->orderTable->horizontalHeaderItem(i)->data(Qt::DisplayRole).toString());
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            QTableWidgetItem *item = ui->orderTable->item(i, j);
            if (!item || item->text().isEmpty()) {
                ui->orderTable->setItem(i, j, new QTableWidgetItem("0"));
            }

            QTextTableCell cell = textTable->cellAt(i+1, j);
            QTextCursor cellCursor = cell.firstCursorPosition();
            cellCursor.insertText(ui->orderTable->item(i, j)->text());
        }
    }
    cursor.movePosition(QTextCursor::End);
    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QPrinter::A4);
    printer.setOrientation(QPrinter::Landscape);
    printer.setOutputFileName(filename);
    doc.setDocumentMargin(0);
    doc.setTextWidth(5);
    doc.print(&printer);
}
