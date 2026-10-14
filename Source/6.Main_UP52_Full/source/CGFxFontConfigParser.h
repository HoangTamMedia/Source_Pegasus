
#pragma once

#ifdef LDK_ADD_SCALEFORM

#ifndef INC_FontConfigParser_H
#define INC_FontConfigParser_H

#include "GFile.h"
#include "GString.h"
#include "GFxLoader.h"

class ConfigParser
{
    GFxWStringBuffer Buffer;

    SInt    Pos;
    UInt    Line;

    inline void SkipWhitespace()
    {
        while ((Pos < (SInt)Buffer.GetLength()) &&
            ((Buffer[Pos] == L' ') || (Buffer[Pos] == L'\t')))
            Pos++;
    }

    static inline bool IsFirstIdChar(const wchar_t c)
    {
        return ((c >= L'a') && (c <= L'z')) ||
            ((c >= L'A') && (c <= L'Z')) ||
            (c == L'_');
    }

    static inline bool IsNextIdChar(const wchar_t c)
    {
        return IsFirstIdChar(c) ||
            ((c >= L'0') && (c <= L'9'));
    }
    static inline bool IsNumberChar(const wchar_t c)
    {
        return ((c >= L'0') && (c <= L'9')) || c == L'.';
    }
    static inline bool IsSignChar(const wchar_t c)
    {
        return (c == L'+' || c == L'-');
    }

public:

    GString ParentPath;

    ConfigParser(const char* pfilename);
    ~ConfigParser();

    bool    IsValid() const { return Buffer.GetLength() != 0; }

    enum TokenType
    {

        Tok_Error,
        Tok_EOL,
        Tok_EOF,

        Tok_Char,
        Tok_String,
        Tok_Id,
        Tok_Number,
        Tok_SignedNumber
    };

    struct Token
    {
        TokenType   Type;
        const wchar_t* pData;
        UInt        Length;
        const char* pError;

        TokenType        Set(TokenType t, const wchar_t* pdata = 0, UInt length = 0)
        {
            pError = "";
            Type = t;
            pData = pdata;
            Length = length;
            return t;
        }

        GString   GetString() const
        {
            if (pData)
            {
                GString str;

                for (UInt i=0; i < Length; i++)
                {
                    if (pData[i] != '\n')
                        str.AppendChar(pData[i]);
                }

                return str;
            }
            else
            {
                return GString();
            }
        }

        wchar_t        GetChar() const     { return pData ? pData[0] : 0; }

        Float          GetNumber() const
        {
            GString s1;
            s1.AppendString(pData, Length);
            return (Float)atof(s1.ToCStr());
        }

        bool        IsId() const        { return (Type == Tok_Id); }
        bool        IsString() const    { return (Type == Tok_String); }
        bool        IsNumber() const    { return (Type == Tok_Number); }
        bool        IsSignedNumber() const { return (Type == Tok_SignedNumber); }

        bool        IsId(const wchar_t* pid) const
        {
            GString s1, s2;
            s1.AppendString(pData, Length);
            s2.AppendString(pid);
            return ((Type == Tok_Id) &&
                Length == s2.GetLength() &&
                !s1.CompareNoCase(s2));
        }

        bool        IsChar(const wchar_t c) const
        {
            return ((Type == Tok_Char) && GetChar() == c);
        }

    };

    TokenType   NextToken(Token* ptoken);

    struct TokenLine
    {

        GArray<Token>      Tokens;

        SInt               ErrorIndex;

        UInt               LineIndex;

        bool    HasErrors() { return ErrorIndex != -1; }
    };

    bool        TokenizeLine(TokenLine* pline, UInt max);

    static bool MatchLine(GArray<Token> &tokens, const wchar_t* pformat, ...);
};

class FontTranslator : public GFxTranslator
{
    GHash<GString, GString, GString::HashFunctor> Mappings;
public:
    virtual UInt    GetCaps() const         { return Cap_StripTrailingNewLines; }

    void AddMapping(const GString &key, const GString &value);

    virtual void Translate(TranslateInfo* ptranslateInfo);
};

class FontConfig : public GRefCountBase<FontConfig, GStat_Default_Mem>
{
public:
    enum ProviderType
    {
        Provider_Undefined,
        Provider_None,
        Provider_Win32,
        Provider_FT2
    };

    GString               ConfigName;
    ProviderType            Provider;

    GArray<GString>       FontLibFiles;
    GPtr<GFxFontMap>        pFontMap;

    GPtr<FontTranslator>    pTranslation;

    FontConfig()
    {
        Provider = Provider_Undefined;
        pTranslation = *new FontTranslator();
    }

    void Apply(GFxLoader* ploader);

};

class FontConfigSet
{
public:

    typedef ConfigParser::Token     Token;
    typedef ConfigParser::TokenLine TokenLine;

    GArray<GPtr<FontConfig> >   Configs;

    FontConfigSet();

    void Parse(ConfigParser* pparser);

    UInt GetSize() const { return (UInt)Configs.GetSize(); }
    FontConfig* operator [] (UInt index) { return Configs[index]; }
};

#endif

#endif
