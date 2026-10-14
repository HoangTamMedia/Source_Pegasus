
#include "stdafx.h"

#ifdef LDK_ADD_SCALEFORM

#include "CGFxFontConfigParser.h"

#include "GFxFontLib.h"
#include "GFxPlayer.h"
#include "GSysFile.h"

#include <stdarg.h>

ConfigParser::ConfigParser(const char* pfilename)
{
    Pos         = 0;
    Line        = 1;

    ParentPath.AppendString(pfilename);
    GFxURLBuilder::ExtractFilePath(&ParentPath);
    if (ParentPath.CompareNoCase(pfilename) == 0)
    {
        ParentPath = "";
    }

    GSysFile file(pfilename);
    if (file.IsValid())
    {
        SInt flen = 0;
        if ((flen = file.GetLength()) != 0)
        {
            UByte* td = (UByte*) GALLOC(flen, GStat_Default_Mem);
            file.Read(td, flen);

            UInt16* prefix16 = (UInt16*)td;
