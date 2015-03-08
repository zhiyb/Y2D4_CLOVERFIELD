#include "mainwindow.h"
#include "connection.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ConnectionSelection sel(this);
	if (!(init = sel.exec()))
		return;
	sp = new QSerialPort(this);
	sp->setPortName(sel.serialPort);
	sp->setBaudRate(sel.serialSpeed);
	sp->open(sp->ReadWrite);
	if (!sp->isOpen()) {
		init = false;
		QMessageBox::critical(this, tr("Error"), tr("Cannot open selected serial port"));
		return;
	}
	connect(sp, SIGNAL(error(QSerialPort::SerialPortError)), qApp, SLOT(quit()));

	QWidget *w = new QWidget(this);
	QLayout *l = new QVBoxLayout(w);
	setCentralWidget(w);

	leIn = new QTextEdit;
	leIn->setInputMethodHints(Qt::ImhDigitsOnly);
	l->addWidget(leIn);

	pbSend = new QPushButton(tr("&Send"));
	pbSend->setDefault(true);
	l->addWidget(pbSend);
	connect(pbSend, SIGNAL(clicked()), this, SLOT(send()));

	leOut = new QTextEdit;
	leOut->setInputMethodHints(Qt::ImhDigitsOnly);
	l->addWidget(leOut);
	connect(sp, SIGNAL(readyRead()), this, SLOT(receive()));
}

MainWindow::~MainWindow()
{
}

void MainWindow::send(void)
{
	QStringList slist = leIn->toPlainText().split(' ', QString::SkipEmptyParts);
	QVector<char> vec;
	for (int idx = 0; idx < slist.count(); idx++) {
		QString token = slist.at(idx);
		bool ok;
		int i = token.toInt(&ok, 16);
		if (!ok || i < 0 || i >= 256) {
			QMessageBox::critical(this, tr("Error"), tr("Error translating %1").arg(token));
			return;
		}
		vec.append(i);
	}
	sp->write(vec.constData(), vec.size());
}

void MainWindow::receive(void)
{
	while (sp->bytesAvailable()) {
		char c = 0;
		sp->read(&c, 1);
		leOut->setText(leOut->toPlainText().append(tr("%1 ").arg((unsigned int)(unsigned char)c, 0, 16)));
	}
}
