#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QSerialPort>

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();
	bool success(void) {return init;}

private slots:
	void send(void);
	void receive(void);

private:
	bool init;
	QSerialPort *sp;
	QTextEdit *leIn, *leOut;
	QPushButton *pbSend;
};

#endif // MAINWINDOW_H
