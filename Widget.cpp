#include "Widget.h"
#include <QDebug>
#include <QHostAddress>
Widget::Widget(QWidget *parent) :
	QWidget(parent)
{
	setupUi(this);
	initSetting();
	initData();
	initGui();
}

void Widget::initSetting()
{
}

void Widget::initData()
{
	totalBytes = 0;
	bytesReceived = 0;
	fileNameSize = 0;
	bytesReceived =0;
}

void Widget::initNetwork()
{
	tcpServer = new QTcpServer(this);
	if(!tcpServer->listen(QHostAddress::Any, 8600))
	{
		qDebug()<<"tcpServer监听出错";
		qDebug()<<tcpServer->errorString();
	}
	connect(tcpServer,SIGNAL(newConnection()),this,SLOT(slotNewConnection()));
}

void Widget::initGui()
{
	progressBar->reset();
}

void Widget::on_pushButtonListen_clicked()
{
	initNetwork();
}

void Widget::slotNewConnection()
{
	tcpSocket = tcpServer->nextPendingConnection();
	connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(slotReadyRead()));
	connect(tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(slotDisplayError(QAbstractSocket::SocketError)));
}

void Widget::slotReadyRead()
{
	QDataStream in(tcpSocket);
	in.setVersion(QDataStream::Qt_4_7);

	if(bytesReceived <= sizeof(qint64)*2)
	{ //如果接收到的数据小于16个字节，那么是刚开始接收数据，我们保存到//来的头文件信息

		if((tcpSocket->bytesAvailable() >= sizeof(qint64)*2) && (fileNameSize == 0))
		{ //接收数据总大小信息和文件名大小信息
			in >> totalBytes >> fileNameSize;
			bytesReceived += sizeof(qint64) * 2;
		}

		if((tcpSocket->bytesAvailable() >= fileNameSize) && (fileNameSize != 0))
		{  //接收文件名，并建立文件
			in >> fileName;
			labelFileName->setText(tr("接收文件 %1 …").arg(fileName));
			bytesReceived += fileNameSize;
			localFile = new QFile(fileName);
			if(!localFile->open(QFile::WriteOnly))
			{
				qDebug() << "文件无法打开!";
				return;
			}
		}
		else
		{
			return;
		}
	}

	if(bytesReceived < totalBytes)
	{  //如果接收的数据小于总数据，那么写入文件
		bytesReceived += tcpSocket->bytesAvailable();
		inBlock = tcpSocket->readAll();
		localFile->write(inBlock);
		inBlock.resize(0);
	}

	progressBar->setMaximum(totalBytes);
	progressBar->setValue(bytesReceived);
	//更新进度条
	if(bytesReceived == totalBytes)
	{ //接收数据完成时
		//tcpSocket->close();
		localFile->close();
		labelFileName->setText("文件接受完毕");
	}
}

void Widget::slotDisplayError(QAbstractSocket::SocketError)
{
	qDebug()<<"服务端的TcpSocket出错";
	qDebug() << tcpSocket->errorString();
	tcpSocket->close();
	progressBar->reset();
}
