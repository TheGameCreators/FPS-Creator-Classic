#include "stdafx.h"
#include "HashTree.h"

HashTree::HashTree()
{
	dwSize = 100;
	pList = new HashItem* [ dwSize ];

	for ( DWORD i = 0; i < dwSize; i++ ) pList [ i ] = 0;
}

HashTree::~HashTree()
{
	if ( pList ) delete [] pList;
	pList = 0;
}

void HashTree::DeleteTree()
{
	HashItem *pItem = NULL;

	for ( DWORD i = 0; i < dwSize; i++ )
	{
		while ( pList [ i ] ) 
		{
			pItem = pList [ i ];
			pList [ i ] = pList [ i ]->pNextItem;
			delete pItem;
		}
	}

	if ( pList ) delete [] pList;
	pList = 0;
	dwSize = 0;
}

void HashTree::SetTreeSize( DWORD size )
{
	if ( size < 1 ) size = 1;
	if ( size > 100000 ) size = 100000;

	if ( pList ) DeleteTree();

	dwSize = size;
	pList = new HashItem* [ dwSize ];
	for ( DWORD i = 0; i < dwSize; i++ ) pList [ i ] = 0;
}

int HashTree::GetExpanded( const char *szPath )
{
	int index = Hash( szPath );
	int iExpanded = 0;

	HashItem *pItem = pList [ index ];
	
	while ( pItem )
	{
		if ( pItem->sPath.Compare( szPath ) == 0 )
		{
			iExpanded = pItem->iExpanded;
			break;
		}

		pItem = pItem->pNextItem;
	}

	return iExpanded;
}

void HashTree::SetExpanded( const char *szPath, int expanded )
{
	int index = Hash( szPath );

	bool bFound = false;
	HashItem *pItem = pList [ index ];

	while ( pItem )
	{
		if ( pItem->sPath.Compare( szPath ) == 0 )
		{
			bFound = true;
			pItem->iExpanded = expanded;
			break;
		}

		pItem = pItem->pNextItem;
	}

	if ( !bFound )
	{
		HashItem *pNewItem = new HashItem();
		pNewItem->pNextItem = pList [ index ];
		pNewItem->sPath = szPath;
		pNewItem->iExpanded = expanded;
		pList [ index ] = pNewItem;
	}
}

int HashTree::Hash( const char *szString )
{
	if ( !szString ) return 0;

	int total = 0;
	const char *ptr = szString;
	while ( *ptr )
	{
		total += (int) *ptr;
		ptr++;
	}

	total %= dwSize;

	return total;
}

