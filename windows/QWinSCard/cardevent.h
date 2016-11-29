//! \file cardevent.h
//! \brief Header file for CardEvent class.
//! \details File is contains class CardEvent must implement a card detection mechanism.
#ifndef CARDEVENT_H
#define CARDEVENT_H

#include <QObject>
#include <QMutex>
#include <QHash>
#include "NativeSCard.h"

//! \namespace Smartcards
//! \brief Namespace of smartcard classes
namespace Smartcards
{
 /*! 
  \brief This class must implement a card detection mechanism.
  When card insertion is detected, it must send signal cardInserted()
  When card removal is detected, it must send signal cardRemoved()
 */
 class CardEvent : public QObject
 {
  Q_OBJECT
 public:
  //! \fn CardEvent::CardEvent()
  //! \brief Default constructor.
  CardEvent(void);
  //! \fn CardEvent::~CardEvent()
  //! \brief Destructor.
  ~CardEvent();
  //! \fn QString lastReader(void) const
  //! \brief Get last connected reader name.
  //! \return Reader name connected last
  QString lastReader(void) const { return mLastReader; }
  //! \fn QByteArray lastATR(void) const
  //! \brief Get last connected card ATR.
  //! \return Byte array contains ATR last connected card
  QByteArray lastATR(void) const { return mLastATR; }
 public slots:
 //! \fn CardEvent::setCardDetection(bool isDetect)
 //! \brief Enable/Disable detection mechanism Qt-slot
 //! \param[in] isDetect Detect or undetect cards
 void setCardDetection(bool isDetect);
 //! \fn CardEvent::stop( void )
 //! \brief stop detection mechanism Qt-slot
 void stop(void);
  //! \fn CardEvent::run(void)
  //! \brief Run detection mechanism Qt-slot
 void run(void);
 protected:
  //! \fn CardEvent::timerEvent( QTimerEvent* e )
  //! \brief Qt timer event function
  //! \details Contains main cycle of detection mechanism
  //! \param[in] e pointer to Qt timer event class, ignored.
  virtual void	timerEvent(QTimerEvent* e);
 signals:
  //! \fn void cardInserted(const QString& readerName, const QByteArray& ATR)
  //! \brief Qt-signal transmitted when card inserted
  //! \param[in] readerName Reader name where inserted card
  //! \param[in] ATR Byte array contains ATR inserted card
  void cardInserted(const QString& readerName, const QByteArray& ATR);
  //! \fn void cardInserted(const QString& readerName, const QByteArray& ATR)
  //! \brief Qt-signal transmitted when card removed
  //! \param[in] readerName Reader name where removed card
  void cardRemoved(const QString& readerName);
  //! \fn void cardInserted(const QString& readerName, const QByteArray& ATR)
  //! \brief Qt-signal transmitted before stopping card detection mechanism
  void onStopCardEvents(void);
 private:
  //! \fn CardEvent::initStartTokenWatchThread()
  //! \brief internal function for initialize card detection mechanism
  void initStartTokenWatchThread(void);
  //! \fn CardEvent::terminateStopSCardWatchThread()
  //! \brief internal function for terminate card detection mechanism
  void terminateStopSCardWatchThread(void);
  QScopedPointer<Smartcards::WinSCard> m_sCard{ new Smartcards::WinSCard };//!< Pointer to WinSCard interface
  QThread* m_Thread{ nullptr };//!< Pointer to QThread
  QMutex mMutex;//!< mutex for thread working
  QString mLastReader;//!< last connected reader name
  QByteArray mLastATR;//!< last connected card ATR.
  int tId{ 0 };//!< inner Qt timer id
  QHash<QString, bool> m_Readers;//!< container for tracking readers
  bool m_bRunCardDetection{ false };//!< card detection running flag
 };
}
#endif // CARDEVENT_H
