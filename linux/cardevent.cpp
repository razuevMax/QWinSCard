#include "stdafx.h"
#include "cardevent.h"
#include "scardexception.h"
#include "NativeSCard.h"
#include <QThread>

Smartcards::CardEvent::CardEvent(void)
 : QObject()
{
 m_sCard->EstablishContext(Smartcards::User);
 initStartTokenWatchThread();
}

Smartcards::CardEvent::~CardEvent()
{
 terminateStopSCardWatchThread();
 m_sCard->ReleaseContext();
}

void Smartcards::CardEvent::initStartTokenWatchThread()
{
 connect(this, SIGNAL(onStopCardEvents()), this, SLOT(stop()), Qt::DirectConnection);
 m_Thread = new QThread;
 this->moveToThread(m_Thread);
 connect(m_Thread, SIGNAL(started()), this, SLOT(run()), Qt::QueuedConnection);
 connect(m_Thread, SIGNAL(finished()), this, SLOT(deleteLater()), Qt::QueuedConnection);
 connect(m_Thread, SIGNAL(finished()), m_Thread, SLOT(deleteLater()), Qt::QueuedConnection);
 m_Thread->start();
 m_bRunCardDetection = true;
}

void Smartcards::CardEvent::terminateStopSCardWatchThread()
{
 if (true == m_bRunCardDetection) {
  return;
 }
 QMutex lMutex;
 lMutex.lock();
 m_bRunCardDetection = false;
 lMutex.unlock();
 emit onStopCardEvents();
 m_Thread->wait();
 m_Thread = nullptr;
}


void Smartcards::CardEvent::run(void)
{
 DWORD state;
 QByteArray ATR;
 DWORD	nProtocol;
 QStringList readersList;
 QMutex sMutex;
 try
 {
  QMutexLocker locker(&sMutex);
  m_sCard->throwErrors(false);
  readersList = m_sCard->ListReaders();
  int count = readersList.count();
  for (int i = 0; i < count; i++)
  {
   QString readerName = readersList.at(i);
   m_sCard->Connect(readerName, Smartcards::Shared, Smartcards::T0orT1);
   if (!m_sCard->isError())
   {
    ATR = m_sCard->GetCardStatus(state, nProtocol);
    m_sCard->Disconnect(Leave);
    if (state == Smartcards::Specific)
    {
     m_Readers.insert(readerName, true);
     emit cardInserted(readerName, ATR);
    }
   }
   else
    m_Readers.insert(readerName, false);
  }
 }
 catch (...)
 {
  return;
 }
 tId=startTimer(500);
}

void Smartcards::CardEvent::setCardDetection(bool isDetect)
{
 QMutex mutex;
 mutex.lock();
 m_bRunCardDetection=isDetect;
 mutex.unlock();
}

void Smartcards::CardEvent::stop( void )
{
 setCardDetection(false);
 thread()->quit();
}

void Smartcards::CardEvent::timerEvent( QTimerEvent* e )
{
 Q_UNUSED(e)
 if(m_sCard.isNull() || !m_bRunCardDetection)
  return;
 try
 {
  QMutexLocker locker(&mMutex);
  DWORD state;
  QByteArray ATR;
  DWORD	nProtocol;
  QStringList readers;
  m_sCard->EstablishContext(Smartcards::User);
  readers=m_sCard->ListReaders();
  if (m_sCard->isError())
   return;
  int count = readers.count();
  for (int i = 0; i < count; i++)
  {
   QString readerName = readers.at(i);
   if (m_Readers.contains(readerName))
   {
    m_sCard->Connect(readerName, Smartcards::Shared, Smartcards::T0orT1);
    if(!m_sCard->isError())
    {
     ATR = m_sCard->GetCardStatus(state, nProtocol);
     if (state == Smartcards::Specific && !m_Readers[readerName])
     {
      m_Readers[readerName] = true;
      m_sCard->Disconnect(Smartcards::Leave);
      emit cardInserted(readerName, ATR);
     }
    }
    else
    {
     if (m_Readers[readerName])
     {
      m_Readers[readerName] = false;
      emit cardRemoved(readerName);
     }
    }
   }
   else
   {
    m_sCard->Connect(readerName, Smartcards::Shared, Smartcards::T0orT1);
    if (!m_sCard->isError())
    {
     ATR = m_sCard->GetCardStatus(state, nProtocol);
     if (state == Smartcards::Specific)
     {
      m_Readers[readerName] = true;
      m_sCard->Disconnect(Smartcards::Leave);
      m_sCard->ReleaseContext();
      emit cardInserted(readerName, ATR);
     }
     else
      m_Readers[readerName] = false;
    }
    else
     m_Readers[readerName] = false;
   }
  }
  QStringList readersNames = m_Readers.keys();
  count = readersNames.count();
  for (int i = 0; i < count; i++)
  {
   QString readerName = readersNames.at(i);
   if (!readers.contains(readerName))
   {
    m_Readers.remove(readerName);
    emit cardRemoved(readerName);
   }
  }
 }
 catch (...)
 {
  
 }
}

