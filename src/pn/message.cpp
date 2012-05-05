/**
  * @file message.cpp
  *
  * @brief Třída definující struktury pro předávání a držení zpráv v komunikaci se serverem
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
