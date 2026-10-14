
#ifndef _CKXML_H
#define _CKXML_H

#pragma once

class CkByteData;
#include "CkString.h"

#include "CkObject.h"

#pragma pack (push, 8)

class CkXml : public CkObject
{
    protected:
	void *m_impl;
	bool m_utf8;

	CkXml(const CkXml &) { }
	CkXml &operator=(const CkXml &) { return *this; }
	CkXml(void *impl) : m_impl(impl) { }

	unsigned long nextIdx(void);
	unsigned long m_resultIdx;
	CkString m_resultString[10];

    public:
	void *getImpl(void) const { return m_impl; }

	CkXml();
	virtual ~CkXml();

	bool SetBinaryContent(const char *data, unsigned long dataLen,bool zipFlag, bool encryptFlag, const char *password);

	bool get_Utf8(void) const;
	void put_Utf8(bool b);

        CkXml *HttpGet(const char *url);
        CkXml *HttpPost(const char *url);

	bool SetBinaryContent(const unsigned char *data, unsigned long dataLen,bool zipFlag, bool encryptFlag, const char *password);
	bool SetBinaryContent(const CkByteData &data, bool zipFlag, bool encryptFlag, const char *password);

	bool GetBinaryContent(CkByteData &data, bool unzipFlag, bool decryptFlag, const char *password);

	bool ZipTree();
	bool ZipContent();
	bool UnzipTree();
	bool UnzipContent();

	bool EncryptContent(const char *password);
	bool DecryptContent(const char *password);

	void GetRoot2(void);

	CkXml *GetRoot(void);
	CkXml *GetChildWithTag(const char *tag);
	CkXml *GetChildWithContent(const char *content);
	CkXml *GetChildExact(const char *tag, const char *content);

	long get_TreeId(void);
	CkXml *PreviousSibling(void);
	CkXml *NextSibling(void);
	bool PreviousSibling2(void);
	bool NextSibling2(void);
	CkXml *LastChild(void);
	CkXml *FirstChild(void);
	bool Clear();
	void Copy(CkXml *node);
	CkXml *GetParent(void);
	bool GetParent2(void);
	bool FirstChild2(void);
	bool LastChild2(void);

	CkXml *SearchForAttribute(const CkXml *after, const char *tag, const char *attr, const char *valuePattern);
	CkXml *SearchAllForContent(const CkXml *after, const char *contentPattern);
	CkXml *SearchForContent(const CkXml *after, const char *tag, const char *contentPattern);
	CkXml *SearchForTag(const CkXml *after, const char *tag);

	bool SearchForAttribute2(const CkXml *after, const char *tag, const char *attr, const char *valuePattern);
	bool SearchAllForContent2(const CkXml *after, const char *contentPattern);
	bool SearchForContent2(const CkXml *after, const char *tag, const char *contentPattern);
	bool SearchForTag2(const CkXml *after, const char *tag);
	bool GetNthChildWithTag2(const char *tag, long n);

	CkXml *SearchForAttribute(const CkXml &after, const char *tag, const char *attr, const char *valuePattern){ return SearchForAttribute(&after,tag,attr,valuePattern); }
	CkXml *SearchAllForContent(const CkXml &after, const char *contentPattern){ return SearchAllForContent(&after,contentPattern); }
	CkXml *SearchForContent(const CkXml &after, const char *tag, const char *contentPattern){ return SearchForContent(&after,tag,contentPattern); }
	CkXml *SearchForTag(const CkXml &after, const char *tag){ return SearchForTag(&after,tag); }

	bool SearchForAttribute2(const CkXml &after, const char *tag, const char *attr, const char *valuePattern){ return SearchForAttribute2(&after,tag,attr,valuePattern); }
	bool SearchAllForContent2(const CkXml &after, const char *contentPattern){ return SearchAllForContent2(&after,contentPattern); }
	bool SearchForContent2(const CkXml &after, const char *tag, const char *contentPattern){ return SearchForContent2(&after,tag,contentPattern); }
	bool SearchForTag2(const CkXml &after, const char *tag){ return SearchForTag2(&after,tag); }

	CkXml *FindChild(const char *tag);
	CkXml *FindOrAddNewChild(const char *tag);
	CkXml *NewChildBefore(int index, const char *tag, const char *content);
	CkXml *NewChildAfter(int index, const char *tag, const char *content);
	CkXml *NewChild(const char *tag, const char *content);
	void NewChild2(const char *tag, const char *content);
	void NewChildInt2(const char *tag, int value);
	CkXml *GetNthChildWithTag(const char *tag, long n);
	long NumChildrenHavingTag(const char *tag);
	CkXml *ExtractChildByName(const char *tag, const char *attrName, const char *attrValue);

	CkXml *ExtractChildByIndex(long index);
	void RemoveFromTree(void);
	CkXml *GetChild(long index);
	bool GetChild2(long index);
	bool AddChildTree(const CkXml *tree);
	bool SwapTree(const CkXml *tree);
	bool SwapNode(const CkXml *node);

	bool AddChildTree(const CkXml &tree) { return AddChildTree(&tree); }

	bool InsertChildTreeAfter(int index, const CkXml &tree);
	bool InsertChildTreeBefore(int index, const CkXml &tree);

	bool SwapTree(const CkXml &tree) { return SwapTree(&tree); }
	bool SwapNode(const CkXml &node) { return SwapNode(&node); }

	bool HasAttrWithValue(const char *name, const char *value);

	bool GetAttrValue(const char *name, CkString &str);
	bool GetAttributeValue(long index, CkString &str);
	bool GetAttributeName(long index, CkString &str);

	bool RemoveAllAttributes();

	void RemoveChild(const char *tag);

	bool RemoveAttribute(const char *name);
	bool AddAttribute(const char *name, const char *value);
	bool AppendToContent(const char *str);

	bool GetXml(CkString &str);

	bool SaveXml(const char *fileName);
	bool LoadXmlFile(const char *fileName);
	bool LoadXml(const char *xmlData);
	bool LoadXmlFile2(const char *fileName, bool autoTrim);
	bool LoadXml2(const char *xmlData, bool autoTrim);

	const char *get_Version(void) const;
	bool get_Version(CkString &str);

	bool get_Cdata();
	bool put_Cdata(bool newVal);

	bool get_SortCaseInsensitive();
	void put_SortCaseInsensitive(bool newVal);

	long get_NumChildren();

	void get_Content(CkString &str);
	void put_Content(const char *newVal);

	bool GetChildContent(const char *tag, CkString &str);

	void UpdateChildContent(const char *tag, const char *value);

	bool ContentMatches(const char *pattern, bool caseSensitive);

	bool ChildContentMatches(const char *tag, const char *pattern, bool caseSensitive);

	void get_Tag(CkString &str);
	void put_Tag(const char *newVal);

	long get_NumAttributes();

	bool get_Standalone(void);
	void put_Standalone(bool newVal);

	void get_Encoding(CkString &str);
	void put_Encoding(const char *newVal);

	bool SaveLastError(const char *filename);
        void LastErrorXml(CkString &str);
        void LastErrorHtml(CkString &str);
        void LastErrorText(CkString &str);

	bool QEncodeContent(const char *charset, const CkByteData &db);
	bool BEncodeContent(const char *charset, const CkByteData &db);
	bool DecodeContent(CkByteData &db);

	bool UpdateAttribute(const char *attrName, const char *attrValue);

	bool GetChildIntValue(const char *tag, int &value);

	void SortByTag(long ascending);
	void SortByContent(long ascending);
	void SortByAttribute(const char *attrName, long ascending);

	void SortRecordsByContent(const char *sortTag, long ascending);
	void SortRecordsByAttribute(const char *sortTag, const char *attrName, long ascending);
	CkXml *FindNextRecord(const char *tag, const char *contentPattern);

	bool HasChildWithTag(const char *tag);

	bool GetChildBoolValue(const char *tag);
	int GetChildIntValue(const char *tag);

	void RemoveChildWithContent(const char *content);
	bool HasChildWithTagAndContent(const char *tag, const char *content);
	bool HasChildWithContent(const char *content);
	void RemoveAllChildren(void);
	void RemoveChildByIndex(long index);
	void AccumulateTagContent(const char *tag, const char *skipTags, CkString &str);
	bool GetChildContentByIndex(long index, CkString &str);
	bool GetChildTag(long index, CkString &str);
	void AddToAttribute(const char *name, long amount);
	void AddToContent(long amount);
	void AddToChildContent(const char *tag, long amount);
	void AddOrUpdateAttributeI(const char *name, long value);
	void AddOrUpdateAttribute(const char *name, const char *value);
	bool HasAttribute(const char *name);

	const char *tag(void);
	const char *content(void);
	const char *encoding(void);
	const char *getXml(void);
        const char *lastErrorText(void);
        const char *lastErrorXml(void);
        const char *lastErrorHtml(void);
	const char *getChildContent(const char *tag);
	const char *getAttrValue(const char *name);
	const char *getAttributeValue(long index);
	const char *getAttributeName(long index);
	const char *version(void);
	const char *accumulateTagContent(const char *tag, const char *skipTags);
	const char *getChildContentByIndex(long index);
	const char *getChildTag(long index);

	const char *childContent(const char *tag){ return getChildContent(tag); }
	const char *attr(const char *name){ return getAttrValue(name); }
	const char *attrValue(long index){ return getAttributeValue(index); }
	const char *attrName(long index){ return getAttributeName(index); }
	const char *xml(void){ return getXml(); }

	bool TagEquals(const char *tag);

	int get_ContentInt(void);
	void put_ContentInt(int newVal);

	bool AddAttributeInt(const char *name, int value);

	bool DecodeEntities(const char *str, CkString &outStr);
	const char *decodeEntities(const char *str);

	bool FindChild2(const char *tag);

	int GetAttrValueInt(const char *name);

	int GetAttributeValueInt(int index);

	bool GetChildTagByIndex(int index, CkString &outStr);
	const char *getChildTagByIndex(int index);

	bool SaveBinaryContent(const char *filename, bool unzipFlag, bool decryptFlag, const char *password);

	bool SetBinaryContentFromFile(const char *filename, bool zipFlag, bool encryptFlag, const char *password);

	void SortByAttributeInt(const char *attrName, bool ascending);

	bool UpdateAttributeInt(const char *attrName, int value);

	void UpdateChildContentInt(const char *tag, int value);

	void AddStyleSheet(const char *styleSheet);

	void SortRecordsByContentInt(const char *sortTag, bool ascending);

	bool TagContent(const char *tag, CkString &outStr);
	const char *tagContent(const char *tag);

	CkXml *GetSelf(void);

	void get_DocType(CkString &str);
	const char *docType(void);
	void put_DocType(const char *newVal);

	bool ChilkatPath(const char *cmd, CkString &outStr);
	const char *chilkatPath(const char *cmd);

};
#pragma pack (pop)

#endif
