
#ifndef INC_FxPlayerLog_H
#define INC_FxPlayerLog_H

#include "GFxLog.h"
#include "GMsgFormat.h"

#ifdef GFC_OS_WIN32
#include <Windows.h>
#endif

#if defined (GFC_OS_WIN32) || defined(GFC_OS_LINUX) || defined(GFC_OS_UNIX) || defined(GFC_OS_MAC)

#if !defined(GFC_OS_WIN32)
#include <unistd.h>
#endif

class GFxPlayerLog : public GFxLog
{
public:

    enum { Log_MaxBuffer = 2048 };
    char    LastMessageBuffer[Log_MaxBuffer];
    int     LastMessageLength;
    int     LastMessageCounter;
    int     CounterStrLength;
    bool    LastMessageHadNewline;
    bool    IsFileMode;

#ifdef GFC_OS_WIN32

    enum ConsoleColor
    {
        ConsoleColor_Gray   = FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE,
        ConsoleColor_White  = FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE|FOREGROUND_INTENSITY,
        ConsoleColor_Yellow = FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_INTENSITY,
        ConsoleColor_Cyan   = FOREGROUND_GREEN|FOREGROUND_BLUE|FOREGROUND_INTENSITY
    };

    HANDLE          hOutput;
    ConsoleColor    Color, LastColor;

    bool GetFileMode()
    {
        hOutput    = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD Mode;
        return GetConsoleMode(hOutput, &Mode) == 0;
    }

    void BeginColor()
    {
        if (!IsFileMode && Color != LastColor)
        {
            ::SetConsoleTextAttribute(hOutput, (WORD)Color);
            LastColor = Color;
        }
    }
    void EndColor()
    {
    }

#else

    enum ConsoleColor
    {
        ConsoleColor_Gray   = 0,
        ConsoleColor_White  = 37 | 0x280100,
        ConsoleColor_Yellow = 33 | 0x280100,
        ConsoleColor_Cyan   = 36 | 0x100,
    };

    ConsoleColor    Color, LastColor;

    bool GetFileMode()
    {
        return 0==isatty(1);
    }

    void BeginColor()
    {
        if (IsFileMode)
            return;

        if (Color & 0xff0000)
            printf("\x1b[%02d;%02d;%02dm", (Color >> 8) & 0xff, (Color >> 16), Color & 0xff);
        else
            printf("\x1b[%02d;%02dm", (Color >> 8) & 0xff, Color & 0xff);
    }
    void EndColor()
    {
        if (IsFileMode)
            return;

        printf("\x1b[00m");
    }

#endif

    void SetTextColor(ConsoleColor attributes)
    {
        Color = attributes;
    }

    GFxPlayerLog()
    {
        LastMessageLength = 0;
        LastMessageHadNewline = 1;

        IsFileMode = GetFileMode();

        Color = LastColor = ConsoleColor_Gray;
    }

    ~GFxPlayerLog()
    {

        if (LastMessageLength && LastMessageHadNewline)
        {
            if (IsFileMode && (LastMessageCounter > 1))
                DisplayCounter(LastMessageCounter);
            LogPrint("\n",1);
        }
        SetTextColor(ConsoleColor_Gray);
        BeginColor();
    }

    void LogPrint (const char* buff, UInt length = 0)
    {
#ifdef GFC_OS_WIN32
        if (IsFileMode)
        {
            fputs(buff,stdout);
        }
        else
        {
            if (!length)
                length = (UInt)G_strlen(buff);

            HANDLE hConsoleOutput = GetStdHandle( STD_OUTPUT_HANDLE );

            wchar_t buf[256];
            wchar_t* pbuf;
            if (length < sizeof(buf)/sizeof(buf[0]))
                pbuf = buf;
            else
                pbuf = (wchar_t*)GALLOC(sizeof(buf[0])*(length + 1), GStat_Default_Mem);

            length = (UInt)GUTF8Util::DecodeString(pbuf, buff, length);
            DWORD ret;
            WriteConsoleW(hConsoleOutput, pbuf, length, &ret, NULL);

            if (pbuf != buf)
                GFREE(pbuf);
        }
#else
        GUNUSED(length);
        fputs(buff,stdout);
#endif
    }

    static bool MessageNeedsCounting(LogMessageType messageType)
    {
        return (messageType == Log_ScriptError) || (messageType == Log_ScriptWarning);
    }

    static int  DisplayCounter(int counter, int backspaceCount = 0)
    {
        char    backspaceBuff[16], counterBuff[32];
        char*   pbackspace;
        int     i, counterLength;

        for (i = backspaceCount, pbackspace = backspaceBuff; i > 0; pbackspace++,  i--)
            *pbackspace = '\b';
        *pbackspace = 0;

        counterLength = (int)G_Format(
            GStringDataPtr(counterBuff, sizeof(counterBuff)),
            "{0} (x{1})",
            backspaceBuff,
            counter
            );

        printf("%s", counterBuff);
        return counterLength - backspaceCount;
    }

    void    DisplayCountedMessage(LogMessageType messageType, const char* pfmt, va_list argList)
    {
        BeginColor();
        char    messageBuffer[Log_MaxBuffer];
        int     messageLength;

        if (!MessageNeedsCounting(messageType))
        {
            if (LastMessageLength && LastMessageHadNewline)
            {

                LogPrint("\n",1);
                LastMessageLength = 0;
            }

            messageLength = (int)G_vsprintf(messageBuffer, Log_MaxBuffer-1, pfmt, argList);

            LogPrint(messageBuffer, messageLength);
            EndColor();
            return;
        }

        messageLength = (int)G_vsprintf(messageBuffer, Log_MaxBuffer-1, pfmt, argList);

        if (messageLength > 0)
        {
            messageBuffer[messageLength] = 0;

            if (!LastMessageHadNewline)
            {

                LogPrint(messageBuffer, messageLength);
                if (messageBuffer[messageLength-1] == '\n')
                {
                    LastMessageHadNewline = 1;
                    LastMessageLength = 0;
                }
            }
            else
            {
                bool haveNewline = 0;
                if (messageBuffer[messageLength-1] == '\n')
                {
                    messageBuffer[messageLength-1] = 0;
                    haveNewline = 1;
                }

                if (!LastMessageLength || !haveNewline)
                {

                    LogPrint(messageBuffer, messageLength);
                    G_strcpy(LastMessageBuffer, Log_MaxBuffer, messageBuffer);
                    LastMessageLength       = messageLength;
                    LastMessageCounter      = 1;
                    LastMessageHadNewline   = haveNewline;
                    CounterStrLength        = 0;
                }
                else
                {

                    if (G_strcmp(LastMessageBuffer, messageBuffer) == 0)
                    {

                        LastMessageCounter++;

                        if (!IsFileMode)
                            CounterStrLength = DisplayCounter(LastMessageCounter, CounterStrLength);
                    }
                    else
                    {

                        if (IsFileMode && (LastMessageCounter > 1))
                            DisplayCounter(LastMessageCounter);

                        LogPrint("\n",1);
                        LogPrint(messageBuffer, messageLength);

                        G_strcpy(LastMessageBuffer, Log_MaxBuffer, messageBuffer);
                        LastMessageLength       = messageLength;
                        LastMessageCounter      = 1;
                        LastMessageHadNewline   = 1;
                        CounterStrLength        = 0;
                    }
                }
            }
        }

        EndColor();
    }

    virtual void    LogMessageVarg(LogMessageType messageType, const char* pfmt, va_list argList)
    {

        if (messageType & Log_Channel_Script)
        {
            if (messageType == Log_ScriptMessage)
                SetTextColor(ConsoleColor_Yellow);
            else
                SetTextColor(ConsoleColor_White);
        }
        else if (messageType == Log_Message)
        {

            SetTextColor(ConsoleColor_Cyan);
        }

        DisplayCountedMessage(messageType, pfmt, argList);

        if ((messageType & Log_Channel_Script) || (messageType == Log_Message))
            SetTextColor(ConsoleColor_Gray);
    }
};

#else

class GFxPlayerLog : public GFxLog
{
public:

    virtual void    LogMessageVarg(LogMessageType messageType, const char* pfmt, va_list argList)
    {

        vprintf(pfmt, argList);
    }
};

#endif

#endif
