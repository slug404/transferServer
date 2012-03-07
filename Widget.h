#ifndef WIDGET_H
#define WIDGET_H

#include "ui_Widget.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>

class Widget : public QWidget, private Ui::Widget
{
	Q_OBJECT
	
public:
	explicit Widget(QWidget *parent = 0);
private slots:
	void on_pushButtonListen_clicked();
	void slotNewConnection();
	void slotReadyRead();
	void slotDisplayError(QAbstractSocket::SocketError);
private:
	void initSetting();
	void initData();
	void initNetwork();
	void initGui();
private:
	QTcpServer *tcpServer;
	QTcpSocket *tcpSocket;
	qint64 totalBytes;  //存放总大小信息
	qint64 bytesReceived;  //已收到数据的大小
	qint64 fileNameSize;  //文件名的大小信息
	QString fileName;   //存放文件名
	QFile *localFile;   //本地文件
	QByteArray inBlock;   //数据缓冲区
};

#endif // WIDGET_H
