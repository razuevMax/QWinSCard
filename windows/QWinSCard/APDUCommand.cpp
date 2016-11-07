#include <QTextStream>
#include <qalgorithms.h>

#include "APDUCommand.h"

using namespace Smartcards;

APDUCommand::APDUCommand(BYTE bCla, BYTE bIns, BYTE bP1, BYTE bP2, const QByteArray& vData, BYTE nLe)
 : m_bClass(bCla), m_bIns(bIns), m_bP1(bP1), m_bP2(bP2), m_nLe(nLe), m_vData(vData)
{
}

APDUCommand::APDUCommand(BYTE bCla, BYTE bIns, BYTE bP1, BYTE bP2, std::initializer_list<char> ilist, BYTE nLe)
 : m_bClass(bCla), m_bIns(bIns), m_bP1(bP1), m_bP2(bP2), m_nLe(nLe)
{
 qCopy(ilist.begin(), ilist.end(), m_vData.begin());
}

APDUCommand::~APDUCommand(void)
{
}

void APDUCommand::setCommand(BYTE bCla, BYTE bIns, BYTE bP1, BYTE bP2, const QByteArray& vData, BYTE nLe)
{
 m_bClass = bCla;
 m_bIns = bIns;
 m_bP2 = bP2;
 m_bP1 = bP1;
 m_nLe = nLe;
 setData(vData);
}

QString APDUCommand::toString() const
{
 QString strData;
 QTextStream sDataStream(&strData);
 sDataStream.setIntegerBase(16);
 sDataStream.setNumberFlags(QTextStream::UppercaseDigits);
 BYTE bLc = 0, bP3 = m_nLe;
 int count = m_vData.size();
 if (count)
 {
  sDataStream.setFieldWidth(2);
  int nI = 0;
  while (nI < count)
  {
   int di = 0;
   while (di<8 && nI<count)
   {
    sDataStream << qSetPadChar('0') << static_cast<BYTE>(m_vData.at(nI++)) << qSetPadChar(' ') << ' ';
    di++;
   }
   sDataStream << endl;
  }
  bLc = static_cast<BYTE>(m_vData.size());
  bP3 = bLc;
 }
 QString result;
 sDataStream.flush();
 sDataStream.setString(&result);
 sDataStream.setIntegerBase(16);
 sDataStream.setNumberFlags(QTextStream::UppercaseDigits);
 sDataStream.setFieldWidth(2);
 sDataStream << endl << "Class=" << qSetPadChar('0') << m_bClass << qSetPadChar(' ') << " Ins=" << qSetPadChar('0') << m_bIns << qSetPadChar(' ') << " P1=" <<
  qSetPadChar('0') << m_bP1 << qSetPadChar(' ') << " P2=" << qSetPadChar('0') << m_bP2 << qSetPadChar(' ') << " P3=" << qSetPadChar('0') << bP3 << qSetPadChar(' ') << " Data=" << endl << strData;
 sDataStream.flush();
 return result;
}

QByteArray APDUCommand::createCommandVector(void) const
{
 // Build the command APDU
 QByteArray commandVector;
 int dataSize = m_vData.size();
 commandVector.push_back(static_cast<char>(m_bClass));
 commandVector.push_back(static_cast<char>(m_bIns));
 commandVector.push_back(static_cast<char>(m_bP1));
 commandVector.push_back(static_cast<char>(m_bP2));
 if (m_vData.isEmpty())
  commandVector.push_back(static_cast<char>(m_nLe));
 else
 {
  commandVector.push_back(static_cast<char>(dataSize));
  commandVector.reserve(APDU_MIN_LENGTH + dataSize+1);
  commandVector.append(m_vData);
 }
 return commandVector;
}
