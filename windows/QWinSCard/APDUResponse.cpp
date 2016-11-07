#include <winscard.h>
#include <QtCore/QByteArray>
#include <QtCore/QTextStream>

#include "APDUResponse.h"

using namespace Smartcards;

APDUResponse::APDUResponse(const QByteArray& vData)
{
 setData(vData);
}

APDUResponse::APDUResponse( const APDUResponse& response )
{
 m_bSw1=response.m_bSw1;
 m_bSw2=response.m_bSw2;
 m_vData=response.m_vData;
}

APDUResponse& APDUResponse::operator=(const APDUResponse& other)
{
 if(this!=&other)
 {
  m_bSw1=other.m_bSw1;
  m_bSw2=other.m_bSw2;
  m_vData=other.m_vData;
 }
 return *this;
}

APDUResponse::APDUResponse(BYTE sw1, BYTE sw2)
{
 m_bSw1=sw1;
 m_bSw2=sw2;
}

APDUResponse::~APDUResponse(void)
{
}

QString APDUResponse::toString( void ) const
{
 QString result;
 QTextStream sDataStream(&result);
 // Display SW1 SW2
 sDataStream.setIntegerBase(16);
 sDataStream.setNumberFlags(QTextStream::UppercaseDigits);
 sDataStream.setFieldWidth(4);
 sDataStream << endl << "SW=" << qSetPadChar('0') << getStatus() << qSetPadChar(' ') << ' ';
 int count = m_vData.size();
 if (count)
 {
  sDataStream << endl << "Data=";
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
 }
 sDataStream.flush();
 return result;
}

void APDUResponse::setData( const QByteArray& vData )
{
 if(vData.isEmpty())
  {
   m_vData.clear();
   m_bSw1=0;
   m_bSw2=0;
   return;
  }
 int size=vData.size();
 if (size>SW_LENGTH)
  m_vData = vData.left(size- SW_LENGTH);
 m_bSw1 = static_cast<BYTE>(vData.at(size - SW_LENGTH));
 m_bSw2 = static_cast<BYTE>(vData.at(size - SW_LENGTH + 1));
}
