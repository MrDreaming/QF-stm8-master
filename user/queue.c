#include "queue.h"
static uint16_t m_ReadPtr = 0;
static uint16_t m_WritePtr = 0;
static uint16_t m_MsgCount = 0;
static QUEUE_TYPE  m_MsgBuf[MASSAGE_MAX] = {0};

bool InsertMsgQueue(QUEUE_TYPE value)
{
    bool bret = false;
    if(m_MsgCount < MASSAGE_MAX)
    {
        m_MsgBuf[m_WritePtr] = value;
        m_WritePtr++;
        m_WritePtr &= MASSAGE_MASK;
        m_MsgCount++;
        bret = true;
    }  
    return bret;
}

uint16_t GetMsgQueue(QUEUE_TYPE *value)
{
    uint16_t index;
    uint16_t Maxcount = m_MsgCount;
    for(index = 0; index < Maxcount; ++index)
    {
       value[index] = m_MsgBuf[m_ReadPtr];
       m_MsgCount--;
       m_ReadPtr++;
       m_ReadPtr &= MASSAGE_MASK;
    }
    return index;
}