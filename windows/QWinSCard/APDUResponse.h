#ifndef APDURESPONSE_H
#define APDURESPONSE_H
#include <winscard.h>
#include <QByteArray>

namespace Smartcards
{
/// <summary>
/// This class represents the APDU response sent by the card
/// </summary>
class APDUResponse
{
 QByteArray	m_vData;
 BYTE	m_bSw1{ 0 };
 BYTE	m_bSw2{ 0 };
public:
 /// <summary>
 ///	Status bytes length
 /// </summary>
 static const int SW_LENGTH = 2;
 /// <summary>
 /// Constructor from the byte data sent back by the card
 /// </summary>
 /// <param name="baData">Buffer of data from the card</param>
 APDUResponse(const QByteArray& vData= QByteArray());
 APDUResponse(const APDUResponse& response);
 APDUResponse(BYTE sw1, BYTE sw2);
 APDUResponse& operator=(const APDUResponse& other);
 ~APDUResponse(void);
 /// <summary>
 /// Response data get property. Contains the data sent by the card minus the 2 status bytes (SW1, SW2)
 /// null if no data were sent by the card
 /// </summary>
 QByteArray getData(void) const { return m_vData; }
 void setData(const QByteArray& vData);
 /// <summary>
 /// SW1 byte get property
 /// </summary>
 BYTE getSW1(void) const { return m_bSw1; }
 /// <summary>
 /// SW2 byte get property
 /// </summary>
 BYTE getSW2(void) const { return m_bSw2; }
 /// <summary>
 /// Status get property
 /// </summary>
 WORD getStatus(void) const { return static_cast<WORD>((static_cast<short>(m_bSw1) << 8) + static_cast<short>(m_bSw2)); }
 /// <summary>
 ///format to a string the APDUResponse object
 /// </summary>
 /// <returns></returns>
 QString toString(void) const;
};

}
#endif