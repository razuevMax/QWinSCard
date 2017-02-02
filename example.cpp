class QT_token_check : public QMainWindow
{
Q_OBJECT

public:

	QT_token_check(QWidget *parent = nullptr, Qt::WindowFlags = 0);
	
	~QT_token_check();
	
//  ...
  
public slots:

	void cardInserted(const QString& readerName, const QByteArray& ATR);
	
	void cardRemoved(const QString& readerName);
	
private:

 QScopedPointer<CardEvent> mCardEventThread{ nullptr };
 
 QSharedPointer<Smartcards::WinSCard> CardWorkSpace{ new Smartcards::WinSCard };
 
// ...
 
}

//...

QT_token_check::QT_token_check(QWidget *parent, Qt::WindowFlags flags)

	: QMainWindow(parent, flags)
	
{

 ui.setupUi(this);
 
  //Establish context on smart card service
  
  CardWorkSpace->EstablishContext(Smartcards::User);
  
  //Connect signal on smartcards watcher
  
  connect(CardWorkSpace.data(), SIGNAL(cardInserted(const QString&, const QByteArray&)), this, SLOT(cardInserted(const QString&, const QByteArray&)), Qt::QueuedConnection);
  
  connect(CardWorkSpace.data(), SIGNAL(cardRemoved(const QString&)), this, SLOT(cardRemoved(const QString&)), Qt::QueuedConnection);
  
  //...
  
  CardWorkSpace->ReleaseContext();
  
}


void QT_token_check::cardInserted(const QString& readerName, const QByteArray& ATR)

{

 QString reader="Sample Reader"
 
 Smartcards::APDUCommand lastCommand{ 0,0,0,0 };
 
 Smartcards::APDUResponse lastResponse;
 
 quint16 m_lastError{ 0 };
 
 CardWorkSpace->throwErrors(false);
 
 //Connect to reader
 
 CardWorkSpace->Connect(reader,Smartcards::Shared,Smartcards::T0orT1);
 
 //...
 
 // bClass, bIns, bP1, bP2, vData, nLe - are set depending on the desired command APDU (depend on the smart card vendor)
 
 lastCommand.setCommand(bClass, bIns, bP1, bP2, vData, nLe);
 
 //Transmit APDU-command
 
 lastResponse = CardWorkSpace->Transmit(lastCommand);
 
 lastError = lastResponse.getStatus();
 
 //...
 
}