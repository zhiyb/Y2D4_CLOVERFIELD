#ifndef CONNECTION_H
#define CONNECTION_H

#include <QtWidgets>
#include <QSerialPort>
#include <QDialog>

#define COMMUNICATION_WAIT	1000

class ConnectionSelection : public QDialog
{
	Q_OBJECT
	friend class MainWindow;
public:
	explicit ConnectionSelection(QWidget *parent = 0);

public slots:
	void accept(void);

private slots:
	void scan(void);

private:
	void normalize(void);

	QLineEdit *leHost, *lePort;
	QLabel *lHost, *lPort;
	QPushButton *pbScan, *pbOpen, *pbCancel;
	QString serialPort;
	int serialSpeed;
};

#endif // CONNECTION_H
