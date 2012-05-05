/**
  * @file message.cpp
  *
  * @brief Trida definujci struktury pro predavani a drzeni zprav v komunikaci se serverem
  * @author xsebek02 xsimon14
  */
#include "message.h"

/**
  * Konstruktor tridy Message
  */
Message::Message()
{
    command = Message::ERROR;
}
