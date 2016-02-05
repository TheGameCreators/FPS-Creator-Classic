#pragma once

class HashItem
{

public:

	HashItem() { pNextItem = 0; sPath = ""; iExpanded = 0; }
	~HashItem() {}

	HashItem *pNextItem;
	CString sPath;
	int iExpanded;
};

class HashTree
{

public:

	HashTree();
	~HashTree();

	void SetTreeSize( DWORD size );
	int GetExpanded( const char *szPath );
	void SetExpanded( const char *szPath, int expanded );

protected:

	void DeleteTree();
	int Hash( const char *szString );

	HashItem **pList;
	DWORD dwSize;
};
