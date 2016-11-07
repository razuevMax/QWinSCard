#ifndef CARDEVENT_H
#define CARDEVENT_H

/// <summary>
/// This class must implement a card detection mechanism.
/// 
/// When card insertion is detected, it must send signal cardInserted()
/// When card removal is detected, it must send signal cardRemoved()
/// 
#include <QObject>
#include <QByteArray>
#include <QHash>
#include <QMutex>
#include "NativeSCard.h"

namespace Smartcards
{

 class CardEvent : public QObject
 {
  Q_OBJECT
 public:
  CardEvent(void);
  ~CardEvent();
  QString lastReader(void) const { return mLastReader; }
  QByteArray lastATR(void) const { return mLastATR; }
 public slots:
  void setCardDetection(bool isDetect);
  void stop(void);
  void run(void);
 protected:
  virtual void	timerEvent(QTimerEvent* e);
 signals:
  void cardInserted(const QString& readerName, const QByteArray& ATR);
  void cardRemoved(const QString& readerName);
  void onStopCardEvents(void);
 private:
  void initStartTokenWatchThread(void);
  void terminateStopSCardWatchThread(void);
  QScopedPointer<Smartcards::WinSCard> m_sCard{ new Smartcards::WinSCard };
  QThread* m_Thread{ nullptr };
  QMutex mMutex;
  QString mLastReader;
  QByteArray mLastATR;
  int tId{ 0 };
  QHash<QString, bool> m_Readers;
  bool m_bRunCardDetection{ false };
 };
}
#endif // CARDEVENT_H
