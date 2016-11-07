# QWinSCard
This project offers WinSCard API in Qt5, which supports PC/SC reader in Windows/Linux platform

This library is built on top of the WinSCard native library (Windows Smart Card SDK) for Windows, on top of the pcsc-lite library for Linux.

A detailed description of all WinSCard API functions is available in the MSDN Library Documentation (https://msdn.microsoft.com/en-us/library/aa374731(v=VS.85).aspx).
A detailed description of pcsc-lite library is available on site https://pcsclite.alioth.debian.org/

This library allows you to transmit and receive application protocol data units (APDUs) specified by ISO/IEC 7816-3 to a smart card in Qt5.

Compilation of verified -
Windows:
MS Visual Studio 2015/Qt 5.7

Qt Creator 4.1.0/Qt 5.7/mingw 5.3 32

Linux (tested on SUSE Linux 14):

Qt Creator 4.1.0/Qt 5.7/gcc 4


# Project directory structure

/windows/ contains the relevant windows project

/windows/qwinscard/ contains the relevant Qt5 pro file, compatible with mingw32 compiler

/linux/ contains the relevant linux version files (temporary only sources, without makefile or pro-file)

# Requirements
c++11 compiler (desirable с++14)

Qt 5

pcsc-lite library for linux/mac

# Sample code

class QT_token_check : public QMainWindow
{
Q_OBJECT

public:

	QT_token_check(QWidget *parent = nullptr, Qt::WindowFlags = 0);
	
	~QT_token_check();
	
  ...
  
public slots:

	void cardInserted(const QString& readerName, const QByteArray& ATR);
	
	void cardRemoved(const QString& readerName);
	
private:

 QScopedPointer<CardEvent> mCardEventThread{ nullptr };
 
 QSharedPointer<Smartcards::WinSCard> CardWorkSpace{ new Smartcards::WinSCard };
 
 ...
 
}

...

QT_token_check::QT_token_check(QWidget *parent, Qt::WindowFlags flags)

	: QMainWindow(parent, flags)
	
{

 ui.setupUi(this);
 
  //Establish context on smart card service
  
  CardWorkSpace->EstablishContext(Smartcards::User);
  
  //Connect signal on smartcards watcher
  
  connect(CardWorkSpace.data(), SIGNAL(cardInserted(const QString&, const QByteArray&)), this, SLOT(cardInserted(const QString&, const QByteArray&)), Qt::QueuedConnection);
  
  connect(CardWorkSpace.data(), SIGNAL(cardRemoved(const QString&)), this, SLOT(cardRemoved(const QString&)), Qt::QueuedConnection);
  
  ...
  
  CardWorkSpace->ReleaseContext();
  
}


void QT_token_check::cardInserted(const QString& readerName, const QByteArray& ATR)

{

 QString reader=...
 
 Smartcards::APDUCommand lastCommand{ 0,0,0,0 };
 
 Smartcards::APDUResponse lastResponse;
 
 quint16 m_lastError{ 0 };
 
 CardWorkSpace->throwErrors(false);
 
 //Connect to reader
 
 CardWorkSpace->Connect(reader,Smartcards::Shared,Smartcards::T0orT1);
 
 ...
 
 // bClass, bIns, bP1, bP2, vData, nLe - are set depending on the desired command APDU (depend on the smart card vendor)
 
 lastCommand.setCommand(bClass, bIns, bP1, bP2, vData, nLe);
 
 //Transmit APDU-command
 
 lastResponse = CardWorkSpace->Transmit(lastCommand);
 
 lastError = lastResponse.getStatus();
 
 ...
 
}

```
