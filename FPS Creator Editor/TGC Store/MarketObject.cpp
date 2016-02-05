#include "stdafx.h"
#include "MarketObject.h"

#include <atlbase.h>
#include "xmllite.h"
#include <math.h>

#include <stdio.h>
#include "..\Editor.h"
//#include "Global.h"
#include "..\Global.h"

void wtoc( CHAR* Dest, const WCHAR* Source )
{
	int i = 0;

	while( Source [ i ] != L'\0' )
	{
		Dest [ i ] = (CHAR) Source [ i ];
		i++;
	}

	Dest [ i ] = '\0';
}

bool MarketGroup::FillValues( IXmlReader* pReader )
{
	HRESULT hr;

	bool bFinished = false;
	const WCHAR* pwszLocalName = 0;
	const WCHAR* pwszValue = 0;
	XmlNodeType nodeType;
	DWORD dwLength = 0;

	while ( !bFinished && ( hr = pReader->Read(&nodeType) ) == S_OK )
	{
		switch (nodeType)
		{
			case XmlNodeType_Element:
				if ( FAILED( hr = pReader->GetLocalName( &pwszLocalName, NULL ) ) )
				{
					return false;
				}

				break;
			case XmlNodeType_EndElement:
				if ( FAILED( hr = pReader->GetLocalName( &pwszLocalName, NULL ) ) )
				{
					return false;
				}
				
				if ( wcscmp( pwszLocalName, L"group" ) == 0 ) bFinished = true;
				pwszLocalName = 0;

				break;
			case XmlNodeType_Text:
				if (FAILED(hr = pReader->GetValue( &pwszValue, NULL ) ) )
				{
					return false;
				}

				dwLength = (DWORD) wcslen( pwszValue );
				if ( dwLength == 0 ) break;
				if ( dwLength > 1024 ) 
				{
					return false;
				}

				{
					char *pszValue = new char [ dwLength + 1 ];
					wtoc( pszValue, pwszValue );

					if ( pwszLocalName )
					{
						dwLength = (DWORD) wcslen( pwszLocalName );
						if ( dwLength == 0 ) break;
						if ( dwLength > 1024 ) 
						{
							return false;
						}

						char *pszLocalName = new char [ dwLength + 1 ];
						wtoc( pszLocalName, pwszLocalName );

						if ( strcmp( pszLocalName, "group_id" ) == 0 )
						{
							int iValue = atoi( pszValue );
							if ( iValue >= 0 ) dwGroupID = iValue;
						}
						else if ( strcmp( pszLocalName, "groupname" ) == 0 )
						{
							DWORD dwLength = (DWORD) strlen( pszValue );
							if ( dwLength < 63 ) strcpy_s( szName, 64, pszValue );
							else
							{
								strncpy_s( szName, 64, pszValue, 63 );
								szName [ 63 ] = '\0';
							}
						}
						else if ( strcmp( pszLocalName, "parent_id" ) == 0 )
						{
							int iValue = atoi( pszValue );
							if ( iValue >= 0 ) dwParentID = iValue;
						}
						else if ( strcmp( pszLocalName, "amount" ) == 0 )
						{
							int iValue = atoi( pszValue );
							if ( iValue >= 0 ) dwAmount = iValue;
						}

						delete [] pszLocalName;
					}
					else
					{
						MessageBox( NULL, "Value with no var name", "Warning", 0 );
					}

					delete [] pszValue;
				}
				
				break;
			default : break;
		}
	}

	return bFinished;
}

bool MarketGroup::comp( const MarketGroup* b ) const
{
	return strcmp( b->szName, szName ) > 0;
}

MarketGroup::MarketGroup( )
{ 
	dwGroupID = 0; 
	dwParentID = 0; 
	pNextGroup = NULL; 
	pChildGroup = NULL; 
	dwAmount = 0;
	strcpy_s( szName, 64, "" ); 
}

MarketGroup::~MarketGroup( )
{
	while ( pChildGroup )
	{
		MarketGroup *pGroup = pChildGroup;
		pChildGroup = pChildGroup->pNextGroup;

		delete pGroup;
	}
}

char MarketObject::szLastError [ 256 ];
MarketObject* MarketObject::pAllObjects = NULL;
MarketObject* MarketObject::pAllSearchObjects = NULL;
MarketObject* MarketObject::pCompleteList = NULL;
MarketObject* MarketObject::pAllCartObjects = NULL;
MarketGroup* MarketObject::pAllGroups = NULL;
int MarketObject::iSortItem = 0;
int *MarketObject::piGroupArrayCounter = NULL;
DWORD MarketObject::dwMaxGroup = 0;

const char *MarketObject::GetLastError( )
{
	return szLastError;
}

MarketObject::MarketObject( )
{
	pNextObject = 0;

	SetDefaultValues( );
}

MarketObject::~MarketObject( )
{
	Clear( );
}

bool MarketObject::comp( const MarketObject* b ) const
{
	float fRating = 0;
	if ( dwRatingCount > 0 ) fRating = (dwTotalRating / (float) dwRatingCount);

	float fbRating = 0;
	if ( b->dwRatingCount > 0 ) fbRating = (b->dwTotalRating / (float) b->dwRatingCount);

	CString sName = szName;

	switch( iSortItem )
	{
		case 0: return sName.CompareNoCase( b->szName ) < 0; break;
		case 1: return dwCost < b->dwCost; break;
		case 2: return dwCost > b->dwCost; break;
		case 3: return fRating < fbRating; break;
		case 4: return fRating > fbRating; break;
		default: return false;
	}

	return false;
}

void MarketObject::SetSortIndex( LPCSTR szOrderBy )
{
	iSortItem = 0;

	if ( szOrderBy && strlen( szOrderBy ) > 0 )
	{	
		if ( strcmp( szOrderBy, GetLanguageData( "TGC Store", "SortName" ) ) == 0 ) iSortItem = 0;
		else if ( strcmp( szOrderBy, GetLanguageData( "TGC Store", "SortCostL" ) ) == 0 ) iSortItem = 1;
		else if ( strcmp( szOrderBy, GetLanguageData( "TGC Store", "SortCostH" ) ) == 0 ) iSortItem = 2;
		else if ( strcmp( szOrderBy, GetLanguageData( "TGC Store", "SortRatingL" ) ) == 0 ) iSortItem = 3;
		else if ( strcmp( szOrderBy, GetLanguageData( "TGC Store", "SortRatingH" ) ) == 0 ) iSortItem = 4;
	}
}

void MarketObject::Clear( )
{
	//if ( pszDescription ) delete [] pszDescription;
	sDescription.ReleaseBuffer();

	if ( piGroupArrayCounter ) delete [] piGroupArrayCounter;
	piGroupArrayCounter = NULL;

	if ( pszThumbnail ) delete [] pszThumbnail;
	if ( pszPath ) delete [] pszPath;
	if ( ppszImages && dwNumImages > 0 )
	{
		for ( DWORD i = 0; i < dwNumImages; i++ ) 
		{
			if ( ppszImages [ i ] ) delete [] (ppszImages [ i ]);
			ppszImages [ i ] = 0;
		}

		delete [] ppszImages;
	}

	if ( ppszKeywords && dwNumKeywords > 0 )
	{
		for ( DWORD i = 0; i < dwNumKeywords; i++ ) 
		{
			if ( ppszKeywords [ i ] ) delete [] (ppszKeywords [ i ]);
			ppszKeywords [ i ] = 0;
		}

		delete [] ppszKeywords;
	}

	pszThumbnail = NULL;
	pszPath = NULL;
	ppszImages = NULL;
	dwNumImages = 0;
	ppszKeywords = NULL;
	dwNumKeywords = 0;

	SetDefaultValues( );
}

void MarketObject::SetDefaultValues( )
{
	dwID = 0;
	dwGroupID = 0;
	strcpy_s( szName, 64, "" );
	strcpy_s( szType, 64, "" );
	strcpy_s( szCategory, 64, "" );
	strcpy_s( szSubCategory, 64, "" );
	strcpy_s( szAuthor, 64, "" );
	strcpy_s( szVideoLink, 255, "" );
	dwCost = 0;
//	pszDescription = NULL;
	pszThumbnail = NULL;
	dwNumImages = 0;
	ppszImages = NULL;
	dwNumKeywords = 0;
	ppszKeywords = NULL;
	dwFileSize = 0;
	dwTotalRating = 0;
	dwRatingCount = 0;
	pszPath = NULL;
	dwPurchased = 0;
	dwIsRated = 0;
	dwPolygons = 0;
	dwUserDownloads = 0;
	dwDownloads = 0;

	dwComplete = 0;

	bAvailable = true;
	bApproved = true;

	bX9Compatible = false;
	bX10Compatible = false;
}

DWORD MarketObject::GetID( ) const
{
	return dwID;
}

DWORD MarketObject::GetGroupID( ) const
{
	return dwGroupID;
}

const char* MarketObject::GetName( ) const
{
	return szName;
}

const char* MarketObject::GetType( ) const
{
	return szType;
}

const char* MarketObject::GetCategory( ) const
{
	return szCategory;
}

const char* MarketObject::GetSubCategory( ) const
{
	return szSubCategory;
}

const char* MarketObject::GetAuthor( ) const
{
	return szAuthor;
}

DWORD MarketObject::GetCost( ) const
{
	return dwCost;
}

DWORD MarketObject::GetPolygons( ) const
{
	return dwPolygons;
}

DWORD MarketObject::GetFileSize( ) const
{
	return dwFileSize;
}

DWORD MarketObject::GetUserDownloads( ) const
{
	return dwUserDownloads;
}

DWORD MarketObject::GetDownloads( ) const
{
	return dwDownloads;
}

const char* MarketObject::GetDescription( ) const
{
	return sDescription.GetString();
}

const char* MarketObject::GetThumbnail( ) const
{
	return pszThumbnail;
}

const char* MarketObject::GetVideoLink( ) const
{
	return szVideoLink;
}

DWORD MarketObject::GetNumImages( ) const
{
	return dwNumImages;
}

const char* MarketObject::GetImage( DWORD dwIndex ) const
{
	if ( dwIndex >= 0 && dwIndex < dwNumImages && ppszImages )
	{
		return ppszImages [ dwIndex ];
	}
	else return NULL;
}

DWORD MarketObject::GetNumKeywords( ) const
{
	return dwNumKeywords;
}

const char* MarketObject::GetKeyword( DWORD dwIndex ) const
{
	if ( dwIndex >= 0 && dwIndex < dwNumKeywords && ppszKeywords )
	{
		return ppszKeywords [ dwIndex ];
	}
	else return NULL;
}

bool MarketObject::SearchKeywords( const char *szKeyword, bool bExactMatch )
{
	if ( !ppszKeywords ) return false;

	char str [ 30 ];

	if ( bExactMatch )
	{
		for ( DWORD i = 0; i < dwNumKeywords; i++ )
		{
			strcpy_s( str, 30, ppszKeywords [ i ] ); strlower( str );
			if ( strcmp( szKeyword, str ) == 0 ) return true;	
		}
	}
	else
	{
		for ( DWORD i = 0; i < dwNumKeywords; i++ )
		{
			strcpy_s( str, 30, ppszKeywords [ i ] ); strlower( str );
			if ( strstr( str, szKeyword ) ) return true;	
		}
	}

	return false;
}

DWORD MarketObject::GetModelFileSize( ) const
{
	return dwFileSize;
}

DWORD MarketObject::GetTotalRating( ) const
{
	return dwTotalRating;
}

DWORD MarketObject::GetRatingCount( ) const
{
	return dwRatingCount;
}

DWORD MarketObject::GetRating( ) const
{
	if ( dwRatingCount == 0 ) return 0;

	float fRating = dwTotalRating / (float) dwRatingCount;
	if ( fRating > 5 ) fRating = 5;
	if ( fRating < 0 ) fRating = 0;

	return (DWORD) ceil(fRating-0.5);
}

const char* MarketObject::GetPath( ) const
{
	return pszPath;
}

bool MarketObject::IsPurchased( ) const
{
	return dwPurchased > 0;
}

bool MarketObject::IsComplete( ) const
{
	return !( (~dwComplete) & MKT_ALL );
}

bool MarketObject::IsAvailable( ) const
{
	return bAvailable;
}

bool MarketObject::IsApproved( ) const
{
	return bApproved;
}

bool MarketObject::IsRated( ) const
{
	return dwIsRated > 0;
}

bool MarketObject::IsX9Compatible( ) const
{
	return bX9Compatible;
}

bool MarketObject::IsX10Compatible( ) const
{
	return bX10Compatible;
}

void MarketObject::UpdateRating( DWORD newrating )
{
	dwTotalRating = dwTotalRating + newrating;
	dwRatingCount++;
	dwIsRated = 1;
}

void MarketObject::AddKeyword( const char *szKeyword )
{
	dwNumKeywords++;
	char **szNewList = new char* [ dwNumKeywords ];

	DWORD dwLength = strlen( szKeyword ) + 1;
	szNewList [ dwNumKeywords - 1 ] = new char [ dwLength ];
	strcpy_s( szNewList [ dwNumKeywords - 1 ], dwLength, szKeyword );

	if ( ppszKeywords )
	{
		for ( DWORD i = 0 ; i < dwNumKeywords - 1; i++ )
		{
			szNewList [ i ] = ppszKeywords [ i ];
		}

		delete [] ppszKeywords;
	}

	ppszKeywords = szNewList;

	dwComplete |= MKT_KEYWORD;
}

void MarketObject::ParseKeywords( const char *szXMLKeywords )
{
	DeleteKeywordArray( );

	const char *szKeyword = strstr( szXMLKeywords, "<keyword>" );
	while ( szKeyword )
	{
		szKeyword += strlen( "<keyword>" );
		const char* szEndKeyword = strstr( szKeyword, "</keyword>" );
		if ( szEndKeyword )
		{
			DWORD dwLength = (DWORD) (szEndKeyword - szKeyword);
			
			char *szNewKeyword = new char [ dwLength + 1 ];
			strncpy_s( szNewKeyword, dwLength + 1, szKeyword, dwLength );
			szNewKeyword [ dwLength ] = '\0';
			
			AddKeyword( szNewKeyword );

			delete [] szNewKeyword;

			szKeyword = szEndKeyword + strlen( "</keyword>" );
		}

		szKeyword = strstr( szKeyword, "<keyword>" );
	}
}

void MarketObject::DeleteKeywordArray( )
{
	if ( ppszKeywords )
	{
		for ( DWORD i = 0 ; i < dwNumKeywords; i++ ) delete [] ppszKeywords [ i ];
		delete [] ppszKeywords;
		ppszKeywords = NULL;
	}

	dwNumKeywords = 0;
	dwComplete &= ~MKT_KEYWORD;
}

void MarketObject::DeleteImageArray( )
{
	if ( ppszImages )
	{
		for ( DWORD i = 0 ; i < dwNumImages; i++ ) delete [] ppszImages [ i ];
		delete [] ppszImages;
		ppszImages = NULL;
	}

	dwNumImages = 0;
	dwComplete &= ~MKT_IMGS;
}

void MarketObject::FillImageArray( )
{
	ppszImages = new char* [ dwNumImages ];

	for ( DWORD i = 0 ; i < dwNumImages; i++ )
	{
		ppszImages [ i ] = new char [ MAX_PATH ];
		sprintf_s( ppszImages [ i ], MAX_PATH, "Images\\%s_%06d_%02d.jpg", szName, dwID, i+1 );
	}

	dwComplete |= MKT_IMGS;
}

void MarketObject::SetDescription( const char* szDescription )
{
	sDescription = "";
	const char *szPtr = szDescription;

	DWORD dwLength = (DWORD) strlen( szDescription );
	if ( dwLength <= 0 ) return;

	const char *szNewLine = strchr( szPtr, '\n' );
	while ( szNewLine )
	{
		DWORD dwSubLength = (DWORD) (szNewLine - szPtr);
		if ( dwSubLength > 0 )
		{
			sDescription.Append( szPtr, dwSubLength );
			if ( *(szNewLine - 1) != '\r' ) sDescription += "\r\n";
			else sDescription += "\n";
		}
		else
		{
			sDescription += "\r\n";
		}

		szPtr = szNewLine + 1;
		szNewLine = strchr( szPtr, '\n' );
	}

	sDescription += szPtr;
}

bool MarketObject::SetValue( const char *szAttribute, const char *szValue )
{
	if ( strcmp( szAttribute, "object_id" ) == 0 )
	{
		if ( dwComplete &= MKT_OBJ_ID ) return true;

		int iValue = atoi( szValue );
		if ( iValue >= 0 ) dwID = iValue;
		dwComplete |= MKT_OBJ_ID;

		if ( dwComplete & MKT_NAME )
		{
			if ( pszThumbnail ) delete [] pszThumbnail;
			pszThumbnail = new char [ MAX_PATH ];
			sprintf_s( pszThumbnail, MAX_PATH, "%s_%06d_thumb.jpg", szName, dwID );

			dwComplete |= MKT_THUMB;
		}

		if ( (dwComplete & MKT_N_IMGS) && (dwComplete & MKT_NAME) && !(dwComplete & MKT_IMGS) )
		{
			FillImageArray( );
		}
	}
	else if ( strcmp( szAttribute, "group_id" ) == 0 )
	{
		int iValue = atoi( szValue );
		if ( iValue >= 0 ) dwGroupID = iValue;
		dwComplete |= MKT_GRP_ID;
	}
	else if ( strcmp( szAttribute, "name" ) == 0 )
	{
		DWORD dwLength = (DWORD) strlen( szValue );
		if ( dwLength < 63 ) strcpy_s( szName, 64, szValue );
		else
		{
			strncpy_s( szName, 64, szValue, 63 );
			szName [ 63 ] = '\0';
		}
		dwComplete |= MKT_NAME;

		if ( dwComplete & MKT_OBJ_ID )
		{
			if ( pszThumbnail ) delete [] pszThumbnail;
			pszThumbnail = new char [ MAX_PATH ];
			sprintf_s( pszThumbnail, MAX_PATH, "%s_%06d_thumb.jpg", szName, dwID );

			dwComplete |= MKT_THUMB;
		}

		if ( (dwComplete & MKT_N_IMGS) && (dwComplete & MKT_OBJ_ID) && !(dwComplete & MKT_IMGS) )
		{
			FillImageArray( );
		}
	}
	else if ( strcmp( szAttribute, "typename" ) == 0 )
	{
		DWORD dwLength = (DWORD) strlen( szValue );
		if ( dwLength < 63 ) strcpy_s( szType, 64, szValue );
		else
		{
			strncpy_s( szType, 64, szValue, 63 );
			szType [ 63 ] = '\0';
		}
		dwComplete |= MKT_TYPE;
	}
	else if ( strcmp( szAttribute, "catname" ) == 0 )
	{
		DWORD dwLength = (DWORD) strlen( szValue );
		if ( dwLength < 63 ) strcpy_s( szCategory, 64, szValue );
		else
		{
			strncpy_s( szCategory, 64, szValue, 63 );
			szCategory [ 63 ] = '\0';
		}
		dwComplete |= MKT_CATEGORY;
	}
	else if ( strcmp( szAttribute, "subcatname" ) == 0 )
	{
		DWORD dwLength = (DWORD) strlen( szValue );
		if ( dwLength < 63 ) strcpy_s( szSubCategory, 64, szValue );
		else
		{
			strncpy_s( szSubCategory, 64, szValue, 63 );
			szSubCategory [ 63 ] = '\0';
		}
		dwComplete |= MKT_SUBCATEGORY;
	}
	else if ( strcmp( szAttribute, "authorname" ) == 0 )
	{
		DWORD dwLength = (DWORD) strlen( szValue );
		if ( dwLength < 63 ) strcpy_s( szAuthor, 64, szValue );
		else
		{
			strncpy_s( szAuthor, 64, szValue, 63 );
			szAuthor [ 63 ] = '\0';
		}
		dwComplete |= MKT_AUTHOR;
	}
	else if ( strcmp( szAttribute, "cost" ) == 0 )
	{
		int iValue = atoi( szValue );
		if ( iValue >= 0 ) dwCost = iValue;
		dwComplete |= MKT_COST;
	}
	else if ( strcmp( szAttribute, "description" ) == 0 )
	{
		SetDescription( szValue );
		/*
		DWORD dwLength = (DWORD) strlen( szValue );
		if ( dwLength > 0 )
		{
			if ( pszDescription ) delete [] pszDescription;
			pszDescription = new char [ dwLength + 1 ];
			strcpy_s( pszDescription, dwLength + 1, szValue );
			pszDescription [ dwLength ] = '\0';
		}
		*/
		dwComplete |= MKT_DESC;
	}
	/*
	else if ( strcmp( szAttribute, "thumbnail" ) == 0 )
	{
		DWORD dwLength = (DWORD) strlen( szValue );
		if ( dwLength > 0 )
		{
			if ( pszThumbnail ) delete [] pszThumbnail;
			pszThumbnail = new char [ dwLength + 1 ];
			strcpy_s( pszThumbnail, dwLength + 1, szValue );
			pszThumbnail [ dwLength ] = '\0';
		}
	}
	*/
	/*
	else if ( strncmp( szAttribute, "pic", 3 ) == 0 )
	{
		DWORD dwLength = (DWORD) strlen( szValue );
		if ( !ppszImages )
		{
			dwNumImages = 0;
			ppszImages = new char* [ 1 ];
		}
		else
		{
			char** pNewArray = new char* [ dwNumImages + 1 ];
			for ( DWORD i = 0 ; i < dwNumImages; i++ ) pNewArray [ i ] = ppszImages [ i ];
			
			delete [] ppszImages;
			ppszImages = pNewArray;
		}

		ppszImages [ dwNumImages ] = new char [ dwLength + 1 ];
		strcpy_s( ppszImages [ dwNumImages ], dwLength + 1, szValue );
		ppszImages [ dwNumImages ][ dwLength ] = '\0';

		dwNumImages++;
	}
	*/
	else if ( strcmp( szAttribute, "filesize" ) == 0 )
	{
		int iValue = atoi( szValue );
		if ( iValue >= 0 ) dwFileSize = iValue;
		dwComplete |= MKT_SIZE;
	}
	else if ( strcmp( szAttribute, "numimages" ) == 0 )
	{
		DeleteImageArray( );
		int iValue = atoi( szValue );
		if ( iValue >= 0 ) dwNumImages = iValue;
		dwComplete |= MKT_N_IMGS;

		if ( (dwComplete & MKT_NAME) && (dwComplete & MKT_OBJ_ID) )
		{
			FillImageArray( );
		}
	}
	else if ( strcmp( szAttribute, "totalrating" ) == 0 )
	{
		int iValue = atoi( szValue );
		if ( iValue >= 0 ) dwTotalRating = iValue;
		dwComplete |= MKT_T_RAT;
	}
	else if ( strcmp( szAttribute, "ratingcount" ) == 0 )
	{
		int iValue = atoi( szValue );
		if ( iValue >= 0 ) dwRatingCount = iValue;
		dwComplete |= MKT_C_RAT;
	}
	else if ( strcmp( szAttribute, "polygons" ) == 0 )
	{
		int iValue = atoi( szValue );
		if ( iValue >= 0 ) dwPolygons = iValue;
		dwComplete |= MKT_POLYS;
	}
	else if ( strcmp( szAttribute, "userdownloads" ) == 0 )
	{
		int iValue = atoi( szValue );
		if ( iValue >= 0 ) dwUserDownloads = iValue;
	}
	else if ( strcmp( szAttribute, "downloads" ) == 0 )
	{
		int iValue = atoi( szValue );
		if ( iValue >= 0 ) dwDownloads = iValue;
	}
	else if ( strcmp( szAttribute, "path" ) == 0 )
	{
		DWORD dwLength = (DWORD) strlen( szValue );
		if ( dwLength > 0 && dwLength < MAX_PATH )
		{
			if ( pszPath ) delete [] pszPath;
			pszPath = new char [ dwLength + 1 ];
			strcpy_s( pszPath, dwLength + 1, szValue );
			pszPath [ dwLength ] = '\0';
		}
		dwComplete |= MKT_PATH;
	}
	else if ( strcmp( szAttribute, "purchased" ) == 0 )
	{
		int iValue = atoi( szValue );
		if ( iValue >= 0 ) dwPurchased = iValue;
	}
	else if ( strcmp( szAttribute, "userrating" ) == 0 )
	{
		int iValue = atoi( szValue );
		if ( iValue >= 0 ) dwIsRated = iValue;
	}
	else if ( strcmp( szAttribute, "available" ) == 0 )
	{
		if ( strcmp( szValue, "y" ) == 0 ) bAvailable = true;
		else if ( strcmp( szValue, "n" ) == 0 ) bAvailable = false;
		//else MessageBox( NULL, "Invalid availability value", "Warning", 0 );
	}
	else if ( strcmp( szAttribute, "approved" ) == 0 )
	{
		if ( strcmp( szValue, "y" ) == 0 ) bApproved = true;
		else if ( strcmp( szValue, "n" ) == 0 ) bApproved = false;
		//else MessageBox( NULL, "Invalid approval value", "Warning", 0 );
	}
	else if ( strcmp( szAttribute, "x9compatible" ) == 0 )
	{
		if ( strcmp( szValue, "y" ) == 0 ) bX9Compatible = true;
		else if ( strcmp( szValue, "n" ) == 0 ) bX9Compatible = false;
		else MessageBox( NULL, "Invalid X9 value", "Warning", 0 );
		dwComplete |= MKT_X9;
	}
	else if ( strcmp( szAttribute, "x10compatible" ) == 0 )
	{
		if ( strcmp( szValue, "y" ) == 0 ) bX10Compatible = true;
		else if ( strcmp( szValue, "n" ) == 0 ) bX10Compatible = false;
		else MessageBox( NULL, "Invalid X10 value", "Warning", 0 );
		dwComplete |= MKT_X10;
	}
	else if ( strcmp( szAttribute, "keyword" ) == 0 )
	{
		AddKeyword( szValue );
		dwComplete |= MKT_KEYWORD;
	}
	else if ( strcmp( szAttribute, "videolink" ) == 0 )
	{
		DWORD dwLength = (DWORD) strlen( szValue );
		if ( dwLength < 255 ) strcpy_s( szVideoLink, 255, szValue );
		else
		{
			strncpy_s( szVideoLink, 255, szValue, 254 );
			szVideoLink [ 254 ] = '\0';
		}
	}
	else return false;	//unrecognised

	return true;
}

bool MarketObject::FillValues( IXmlReader* pReader )
{
	HRESULT hr;

	bool bFinished = false;
	const WCHAR* pwszLocalName = 0;
	const WCHAR* pwszValue = 0;
	XmlNodeType nodeType;
	DWORD dwLength = 0;

	while ( !bFinished && ( hr = pReader->Read(&nodeType) ) == S_OK )
	{
		switch (nodeType)
		{
			case XmlNodeType_Element:
				if ( FAILED( hr = pReader->GetLocalName( &pwszLocalName, NULL ) ) )
				{
					sprintf_s( szLastError, 256, "Error getting local name, error is %08.8lx", hr);
					return false;
				}

				/*
				if ( wcscmp( pwszLocalName, L"images" ) == 0 )
				{
					//images are present, clear any current images
					if ( ppszImages && dwNumImages > 0 )
					{
						for ( DWORD i = 0; i < dwNumImages; i++ ) 
						{
							if ( ppszImages [ i ] ) delete [] ppszImages [ i ];
							ppszImages [ i ] = 0;
						}

						delete [] ppszImages;
					}

					ppszImages = NULL;
					dwNumImages = 0;
				}
				*/

				if ( wcscmp( pwszLocalName, L"keywords" ) == 0 )
				{
					//keywords are present, clear any current keywords
					DeleteKeywordArray();
				}
				
				break;
			case XmlNodeType_EndElement:
				if ( FAILED( hr = pReader->GetLocalName( &pwszLocalName, NULL ) ) )
				{
					sprintf_s( szLastError, 256, "Error getting local name, error is %08.8lx", hr );
					return false;
				}
				
				if ( wcscmp( pwszLocalName, L"object" ) == 0 ) bFinished = true;
				pwszLocalName = 0;

				break;
			case XmlNodeType_Text:
				if (FAILED(hr = pReader->GetValue( &pwszValue, NULL ) ) )
				{
					sprintf_s( szLastError, 256, "Error getting value, error is %08.8lx", hr );
					return false;
				}

				dwLength = (DWORD) wcslen( pwszValue );
				if ( dwLength == 0 ) break;
				if ( dwLength > 1024 ) 
				{
					sprintf_s( szLastError, 256, "Value string overflow, length: %d", dwLength );
					return false;
				}

				{
					char *pszValue = new char [ dwLength + 1 ];
					wtoc( pszValue, pwszValue );

					if ( pwszLocalName )
					{
						dwLength = (DWORD) wcslen( pwszLocalName );
						if ( dwLength == 0 ) break;
						if ( dwLength > 1024 ) 
						{
							sprintf_s( szLastError, 256, "Element string overflow, length: %d", dwLength );
							return false;
						}

						char *pszLocalName = new char [ dwLength + 1 ];
						wtoc( pszLocalName, pwszLocalName );

						SetValue( pszLocalName, pszValue );

						delete [] pszLocalName;
					}
					else
					{
						MessageBox( NULL, "Value with no var name", "Warning", 0 );
					}

					delete [] pszValue;
				}
				
				break;
			default : break;
		}
	}

	return bFinished;
}

void MarketObject::CompleteObject( char *pszXMLFile )
{
	strcpy_s( szVideoLink, 255, "" ); 

	HRESULT hr;
    CComPtr<IStream> pFileStream;
    CComPtr<IXmlReader> pReader;
    XmlNodeType nodeType;
    const WCHAR* pwszLocalName;

	EnterCriticalSection( &theApp.m_csDirectoryChanges );

	char dir [ MAX_PATH ];
	GetCurrentDirectory( MAX_PATH, dir );
	SetCurrentDirectory( theApp.m_szDirectory );

	if ( FAILED( hr = SHCreateStreamOnFile( pszXMLFile, STGM_READ, &pFileStream ) ) )
    {
        sprintf_s( szLastError, 256, "Error creating file reader, error is %08.8lx", hr );
		LeaveCriticalSection( &theApp.m_csDirectoryChanges );
        return;
    }

	SetCurrentDirectory( dir );

	LeaveCriticalSection( &theApp.m_csDirectoryChanges );

    if (FAILED(hr = CreateXmlReader(__uuidof(IXmlReader), (void**) &pReader, NULL)))
    {
        sprintf_s( szLastError, 256, "Error creating xml reader, error is %08.8lx", hr);
        return;
    }

    if (FAILED(hr = pReader->SetProperty(XmlReaderProperty_DtdProcessing, DtdProcessing_Prohibit)))
    {
        sprintf_s( szLastError, 256, "Error setting XmlReaderProperty_DtdProcessing, error is %08.8lx", hr);
        return;
    }

    if (FAILED(hr = pReader->SetInput(pFileStream)))
    {
        sprintf_s( szLastError, 256, "Error setting input for reader, error is %08.8lx", hr);
        return;
    }

    //read until there are no more nodes
    while (S_OK == (hr = pReader->Read(&nodeType)))
    {
        switch (nodeType)
        {
        case XmlNodeType_Element:
            if (FAILED(hr = pReader->GetLocalName(&pwszLocalName, NULL)))
            {
                sprintf_s( szLastError, 256, "Error getting local name, error is %08.8lx", hr);
                return;
            }
            
            if ( !pReader->IsEmptyElement() )
			{
				if ( wcscmp( pwszLocalName, L"object" ) == 0 )
				{
					bool bSuccess = FillValues( pReader );
					if ( !bSuccess )
					{
						sprintf_s( szLastError, 256, "Error filling market object: %s", MarketObject::GetLastError( ));
					}
				}
			}
            break;
		}
    }
}

void MarketObject::ResetObjectCompleteness( )
{
	MarketObject *pObject = pAllObjects;

	while ( pObject )
	{
		pObject->dwComplete = 0;		

		pObject = pObject->pNextObject;
	}


	pObject = pAllSearchObjects;

	while ( pObject )
	{
		pObject->dwComplete = 0;		

		pObject = pObject->pNextObject;
	}
}

bool MarketObject::LoadGroups( const char *pszXMLFile )
{
	ClearGroups( );

	HRESULT hr;
    CComPtr<IStream> pFileStream;
    CComPtr<IXmlReader> pReader;
    XmlNodeType nodeType;
    const WCHAR* pwszLocalName;

	EnterCriticalSection( &theApp.m_csDirectoryChanges );

	char dir [ MAX_PATH ];
	GetCurrentDirectory( MAX_PATH, dir );
	SetCurrentDirectory( theApp.m_szDirectory );

	if ( FAILED( hr = SHCreateStreamOnFile( pszXMLFile, STGM_READ, &pFileStream ) ) )
    {
        sprintf_s( szLastError, 256, "Error creating file reader, error is %08.8lx", hr );
		LeaveCriticalSection( &theApp.m_csDirectoryChanges );
        return false;
    }

	SetCurrentDirectory( dir );

	LeaveCriticalSection( &theApp.m_csDirectoryChanges );

    if (FAILED(hr = CreateXmlReader(__uuidof(IXmlReader), (void**) &pReader, NULL)))
    {
        sprintf_s( szLastError, 256, "Error creating xml reader, error is %08.8lx", hr);
        return false;
    }

    if (FAILED(hr = pReader->SetProperty(XmlReaderProperty_DtdProcessing, DtdProcessing_Prohibit)))
    {
        sprintf_s( szLastError, 256, "Error setting XmlReaderProperty_DtdProcessing, error is %08.8lx", hr);
        return false;
    }

    if (FAILED(hr = pReader->SetInput(pFileStream)))
    {
        sprintf_s( szLastError, 256, "Error setting input for reader, error is %08.8lx", hr);
        return false;
    }

    //read until there are no more nodes
    while (S_OK == (hr = pReader->Read(&nodeType)))
    {
        switch (nodeType)
        {
        case XmlNodeType_Element:
            if (FAILED(hr = pReader->GetLocalName(&pwszLocalName, NULL)))
            {
                sprintf_s( szLastError, 256, "Error getting local name, error is %08.8lx", hr);
                return false;
            }
            
            if ( !pReader->IsEmptyElement() )
			{
				if ( wcscmp( pwszLocalName, L"group" ) == 0 )
				{
					MarketGroup *pNewGroup = new MarketGroup( );
					
					bool bSuccess = pNewGroup->FillValues( pReader );
					if ( !bSuccess )
					{
						sprintf_s( szLastError, 256, "Error creating market group");
					}
					else
					{
						pNewGroup->pNextGroup = pAllGroups;
						pAllGroups = pNewGroup;
					}
				}
			}
            break;
		}
    }

	MarketGroup *pGroup = pAllGroups;
	MarketGroup *pPrevGroup = NULL;

	//organise the groups as their list order is not defined
	while ( pGroup )
	{
		if ( pGroup->dwParentID > 0 )
		{
			MarketGroup *pParentGroup = GetGroup( pGroup->dwParentID );

			if ( pParentGroup && pParentGroup != pGroup )
			{
				if ( pPrevGroup ) pPrevGroup->pNextGroup = pGroup->pNextGroup;
				else pAllGroups = pGroup->pNextGroup;

				pGroup->pNextGroup = pParentGroup->pChildGroup;
				pParentGroup->pChildGroup = pGroup;
				
				if ( pPrevGroup ) pGroup = pPrevGroup->pNextGroup;
				else pGroup = pAllGroups;
				continue;
			}
		}

		pPrevGroup = pGroup;
		pGroup = pGroup->pNextGroup;
	}

	//sort the main group list by name
	MarketGroup *pNewGroupList = NULL;
	while ( pAllGroups )
	{
		MarketGroup *pGroup = pAllGroups;
		MarketGroup *pPrevGroup = NULL;
		MarketGroup *pGreatestGroup = NULL;
		MarketGroup *pGreatestPrevGroup = NULL;

		//find the next last group (by alphabetical order)
		while ( pGroup )
		{
			if ( !pGreatestGroup || strcmp( pGroup->szName, pGreatestGroup->szName ) > 0 ) 
			{
				pGreatestPrevGroup = pPrevGroup;
				pGreatestGroup = pGroup;
			}

			pPrevGroup = pGroup;
			pGroup = pGroup->pNextGroup;
		}

		if ( pGreatestPrevGroup ) pGreatestPrevGroup->pNextGroup = pGreatestGroup->pNextGroup;
		else pAllGroups = pGreatestGroup->pNextGroup;

		pGreatestGroup->pNextGroup = pNewGroupList;
		pNewGroupList = pGreatestGroup;
	}

	//sort the child groups and remove any redundant
	pAllGroups = pNewGroupList;
	pGroup = pAllGroups;
	pPrevGroup = NULL;
	MarketGroup *pNextGroup = NULL;

	while ( pGroup )
	{
		pNextGroup = pGroup->pNextGroup;

		if ( !CleanupGroups( pGroup ) && pGroup->dwAmount == 0 )
		{
			if( pPrevGroup ) pPrevGroup->pNextGroup = pNextGroup;
			else pAllGroups = pNextGroup;

			delete pGroup;
		}
		else pPrevGroup = pGroup;
		
		pGroup = pNextGroup;
	}

	return true;
}

bool MarketObject::LoadObjects( const char *pszXMLFile )
{
	ClearObjects( );

	HRESULT hr;
    CComPtr<IStream> pFileStream;
    CComPtr<IXmlReader> pReader;
    XmlNodeType nodeType;
    const WCHAR* pwszLocalName;

	EnterCriticalSection( &theApp.m_csDirectoryChanges );

	char dir [ MAX_PATH ];
	GetCurrentDirectory( MAX_PATH, dir );
	SetCurrentDirectory( theApp.m_szDirectory );

	if ( FAILED( hr = SHCreateStreamOnFile( pszXMLFile, STGM_READ, &pFileStream ) ) )
    {
        sprintf_s( szLastError, 256, "Error creating file reader, error is %08.8lx", hr );
		LeaveCriticalSection( &theApp.m_csDirectoryChanges );
        return false;
    }

	SetCurrentDirectory( dir );

	LeaveCriticalSection( &theApp.m_csDirectoryChanges );

    if (FAILED(hr = CreateXmlReader(__uuidof(IXmlReader), (void**) &pReader, NULL)))
    {
        sprintf_s( szLastError, 256, "Error creating xml reader, error is %08.8lx", hr);
        return false;
    }

    if (FAILED(hr = pReader->SetProperty(XmlReaderProperty_DtdProcessing, DtdProcessing_Prohibit)))
    {
        sprintf_s( szLastError, 256, "Error setting XmlReaderProperty_DtdProcessing, error is %08.8lx", hr);
        return false;
    }

    if (FAILED(hr = pReader->SetInput(pFileStream)))
    {
        sprintf_s( szLastError, 256, "Error setting input for reader, error is %08.8lx", hr);
        return false;
    }

    //read until there are no more nodes
    while (S_OK == (hr = pReader->Read(&nodeType)))
    {
        switch (nodeType)
        {
        case XmlNodeType_Element:
            if (FAILED(hr = pReader->GetLocalName(&pwszLocalName, NULL)))
            {
                sprintf_s( szLastError, 256, "Error getting local name, error is %08.8lx", hr);
                return false;
            }
            
            if ( !pReader->IsEmptyElement() )
			{
				if ( wcscmp( pwszLocalName, L"object" ) == 0 )
				{
					MarketObject *pNewObject = new MarketObject( );
					
					bool bSuccess = pNewObject->FillValues( pReader );
					if ( !bSuccess )
					{
						sprintf_s( szLastError, 256, "Error creating market object: %s", MarketObject::GetLastError( ));
					}
					else
					{
						pNewObject->pNextObject = pAllObjects;
						pAllObjects = pNewObject;
					}
				}
			}
            break;
		}
    }

	MarketObject *pObject = pAllObjects;
	MarketObject *pNewList = NULL;
	dwMaxGroup = 0;

	//reverse the object list as it was created in reverse order
	while ( pAllObjects )
	{
		if ( pAllObjects->GetGroupID() > dwMaxGroup ) dwMaxGroup = pAllObjects->GetGroupID();

		pAllObjects = pObject->pNextObject;
		pObject->pNextObject = pNewList;
		pNewList = pObject;

		pObject = pAllObjects;
	}

	pAllObjects = pNewList;

	CString sPurchasedList = theApp.m_szDirectory;
	sPurchasedList += "\\Files\\TGCStore\\TEMP\\PurchasedObjects.xml";
	SetPurchasedObjects( sPurchasedList );

	return true;
}

bool MarketObject::SetPurchasedObjects( const char *szFilename )
{
	FILE *pFile = NULL;
	fopen_s( &pFile, szFilename, "rb" );
	if ( !pFile ) 
	{
		return false;
	}
	fseek( pFile, 0, SEEK_END );
	long lSize = ftell( pFile );
	rewind( pFile );

	char *szFileData = new char [ lSize + 1 ];
	fread( szFileData, 1, lSize, pFile );
	fclose( pFile );
	szFileData [ lSize ] = '\0';

	char *szFilePtr = szFileData;

	char szID [ 12 ];
	DWORD dwID = 0;

	char *szObjectID = strchr( szFilePtr, ';' );
	while ( szObjectID )
	{
		DWORD dwLength = (DWORD) (szObjectID - szFilePtr);
		if ( dwLength > 11 ) dwLength = 11;

		strncpy_s( szID, 12, szFilePtr, dwLength );
		szID [ dwLength ] = '\0';
		dwID = atoi( szID );
	
		MarketObject *pObject = MarketObject::GetObjectByID( dwID, 0 );
		if ( pObject ) pObject->SetValue( "purchased", "1" );

		pObject = MarketObject::GetCartObject( dwID );
		if ( pObject ) pObject->SetValue( "purchased", "1" );

		pObject = MarketObject::GetSearchObject( dwID );
		if ( pObject ) pObject->SetValue( "purchased", "1" );

		pObject = MarketObject::GetCompleteListObject( dwID );
		if ( pObject ) pObject->SetValue( "purchased", "1" );

		szFilePtr = szObjectID + 1;
		szObjectID = strstr( szFilePtr, ";" );
	}
	
	delete [] szFileData;

	return true;
}

bool MarketObject::SetKeywords( const char *szFilename )
{
	FILE *pFile = NULL;
	fopen_s( &pFile, szFilename, "rb" );
	if ( !pFile ) return false;
	
	fseek( pFile, 0, SEEK_END );
	long lSize = ftell( pFile );
	rewind( pFile );

	char *szFileData = new char [ lSize + 1 ];
	fread( szFileData, 1, lSize, pFile );
	fclose( pFile );
	szFileData [ lSize ] = '\0';

	char szID [ 12 ];
	char szNumWords [ 12 ];
	char szWord [ 30 ];

	DWORD dwID = 0;
	DWORD dwNumWords = 0;
	
	char *szFilePtr = szFileData;
	
	//search for first delimiter
	char *szObjectID = strchr( szFilePtr, ';' );
	while ( szObjectID )
	{
		DWORD dwLength = (DWORD) (szObjectID - szFilePtr);
		if ( dwLength > 11 ) dwLength = 11;

		strncpy_s( szID, 12, szFilePtr, dwLength );
		szID [ dwLength ] = '\0';
		dwID = atoi( szID );

		szFilePtr = szObjectID + 1;

		char *szNumKeywords = strchr( szFilePtr, ';' );
		if ( !szNumKeywords )
		{
			MessageBox( NULL, "Malformed keywords file 1", "Warning", 0 );
			break;
		}

		dwLength = (DWORD) (szNumKeywords - szFilePtr);
		if ( dwLength > 11 ) dwLength = 11;

		strncpy_s( szNumWords, 12, szFilePtr, dwLength );
		szNumWords [ dwLength ] = '\0';
		dwNumWords = atoi( szNumWords );

		szFilePtr = szNumKeywords + 1;

		MarketObject *pObject = MarketObject::GetCompleteListObject( dwID );
		if ( pObject ) pObject->DeleteKeywordArray( );

		bool bMalformed = false;
		for ( DWORD i = 0; i < dwNumWords; i++ )
		{
			char *szKeyword = strchr( szFilePtr, ';' );
			if ( !szKeyword )
			{
				MessageBox( NULL, "Malformed keywords file 2", "Warning", 0 );
				bMalformed = true;
				break;
			}

			dwLength = (DWORD) (szKeyword - szFilePtr);
			if ( dwLength > 29 ) dwLength = 29;

			strncpy_s( szWord, 30, szFilePtr, dwLength );
			szWord [ dwLength ] = '\0';

			szFilePtr = szKeyword + 1;

			if ( pObject ) pObject->AddKeyword( szWord );
		}

		if ( bMalformed ) break;

		szObjectID = strchr( szFilePtr, ';' );
	}
	
	delete [] szFileData;

	return true;
}

bool MarketObject::CleanupGroups( MarketGroup *pParentGroup )
{
	if ( !pParentGroup->pChildGroup ) return false;

	MarketGroup *pNewGroupList = NULL;
	while ( pParentGroup->pChildGroup )
	{
		MarketGroup *pGroup = pParentGroup->pChildGroup;
		MarketGroup *pPrevGroup = NULL;
		MarketGroup *pGreatestGroup = NULL;
		MarketGroup *pGreatestPrevGroup = NULL;

		//find the next last group (by alphabetical order)
		while ( pGroup )
		{
			if ( !pGreatestGroup || strcmp( pGroup->szName, pGreatestGroup->szName ) > 0 ) 
			{
				pGreatestPrevGroup = pPrevGroup;
				pGreatestGroup = pGroup;
			}

			pPrevGroup = pGroup;
			pGroup = pGroup->pNextGroup;
		}

		if ( pGreatestPrevGroup ) pGreatestPrevGroup->pNextGroup = pGreatestGroup->pNextGroup;
		else pParentGroup->pChildGroup = pGreatestGroup->pNextGroup;

		pGreatestGroup->pNextGroup = pNewGroupList;
		pNewGroupList = pGreatestGroup;
	}

	pParentGroup->pChildGroup = pNewGroupList;

	MarketGroup *pGroup = pParentGroup->pChildGroup;
	MarketGroup *pPrevGroup = NULL;
	MarketGroup *pNextGroup = NULL;

	while ( pGroup )
	{
		pNextGroup = pGroup->pNextGroup;

		if ( !CleanupGroups( pGroup ) && pGroup->dwAmount == 0 )
		{
			if( pPrevGroup ) pPrevGroup->pNextGroup = pNextGroup;
			else pParentGroup->pChildGroup = pNextGroup;

			delete pGroup;
		}
		else pPrevGroup = pGroup;
		
		pGroup = pNextGroup;
	}

	return pParentGroup->pChildGroup != NULL;
}

bool MarketObject::LoadSearchObjects( const char *szFile )
{
	ClearSearchObjects( );

	/*
	EnterCriticalSection( &theApp.m_csDirectoryChanges );
	SetCurrentDirectory( theApp.m_szDirectory );
	FILE *pFile = NULL;
	fopen_s( &pFile, sFilename, "rb" );
	LeaveCriticalSection( &theApp.m_csDirectoryChanges );

	if ( !pFile ) return false;
	
	fseek( pFile, 0, SEEK_END );
	long lSize = ftell( pFile );
	rewind( pFile );

	char *szFileData = new char [ lSize + 1 ];
	fread( szFileData, 1, lSize, pFile );
	fclose( pFile );
	szFileData [ lSize ] = '\0';

	char szID [ 12 ];
	char szNumWords [ 12 ];
	char szWord [ 30 ];

	DWORD dwID = 0;
	DWORD dwNumWords = 0;
	
	char *szFilePtr = szFileData;
	
	//search for first delimiter
	char *szObjectID = strchr( szFilePtr, ';' );
	while ( szObjectID )
	{
		DWORD dwLength = (DWORD) (szObjectID - szFilePtr);
		if ( dwLength > 11 ) dwLength = 11;

		strncpy_s( szID, 12, szFilePtr, dwLength );
		szID [ dwLength ] = '\0';
		dwID = atoi( szID );

		szFilePtr = szObjectID + 1;

		char *szNumKeywords = strchr( szFilePtr, ';' );
		if ( !szNumKeywords )
		{
			MessageBox( NULL, "Malformed keywords file 1", "Warning", 0 );
			break;
		}

		dwLength = (DWORD) (szNumKeywords - szFilePtr);
		if ( dwLength > 11 ) dwLength = 11;

		strncpy_s( szNumWords, 12, szFilePtr, dwLength );
		szNumWords [ dwLength ] = '\0';
		dwNumWords = atoi( szNumWords );

		szFilePtr = szNumKeywords + 1;

		MarketObject *pObject = MarketObject::GetObject( dwID );
		if ( pObject ) pObject->DeleteKeywordArray( );

		bool bMalformed = false;
		for ( DWORD i = 0; i < dwNumWords; i++ )
		{
			char *szKeyword = strchr( szFilePtr, ';' );
			if ( !szKeyword )
			{
				MessageBox( NULL, "Malformed keywords file 2", "Warning", 0 );
				bMalformed = true;
				break;
			}

			dwLength = (DWORD) (szKeyword - szFilePtr);
			if ( dwLength > 29 ) dwLength = 29;

			strncpy_s( szWord, 30, szFilePtr, dwLength );
			szWord [ dwLength ] = '\0';

			szFilePtr = szKeyword + 1;

			if ( pObject ) pObject->AddKeyword( szWord );
		}

		if ( bMalformed ) break;

		szObjectID = strchr( szFilePtr, ';' );
	}					
	
	delete [] szFileData;

   
					MarketObject *pNewObject = new MarketObject( );
					
						pNewObject->pNextObject = pAllSearchObjects;
						pAllSearchObjects = pNewObject;
					

	MarketObject *pObject = pAllSearchObjects;
	MarketObject *pNewList = NULL;

	//reverse the object list as it was created in reverse order
	while ( pAllSearchObjects )
	{
		pAllSearchObjects = pObject->pNextObject;
		pObject->pNextObject = pNewList;
		pNewList = pObject;

		pObject = pAllSearchObjects;
	}

	pAllSearchObjects = pNewList;
*/
	return true;
}

bool MarketObject::LoadCompleteList( const char *pszXMLFile )
{
	while( pCompleteList )
	{
		MarketObject *pObject = pCompleteList;
		pCompleteList = pCompleteList->pNextObject;

		delete pObject;
	}

	HRESULT hr;
    CComPtr<IStream> pFileStream;
    CComPtr<IXmlReader> pReader;
    XmlNodeType nodeType;
    const WCHAR* pwszLocalName;

	EnterCriticalSection( &theApp.m_csDirectoryChanges );

	char dir [ MAX_PATH ];
	GetCurrentDirectory( MAX_PATH, dir );
	SetCurrentDirectory( theApp.m_szDirectory );

	if ( FAILED( hr = SHCreateStreamOnFile( pszXMLFile, STGM_READ, &pFileStream ) ) )
    {
        sprintf_s( szLastError, 256, "Error creating file reader, error is %08.8lx", hr );
		LeaveCriticalSection( &theApp.m_csDirectoryChanges );
        return false;
    }

	SetCurrentDirectory( dir );

	LeaveCriticalSection( &theApp.m_csDirectoryChanges );

    if (FAILED(hr = CreateXmlReader(__uuidof(IXmlReader), (void**) &pReader, NULL)))
    {
        sprintf_s( szLastError, 256, "Error creating xml reader, error is %08.8lx", hr);
        return false;
    }

    if (FAILED(hr = pReader->SetProperty(XmlReaderProperty_DtdProcessing, DtdProcessing_Prohibit)))
    {
        sprintf_s( szLastError, 256, "Error setting XmlReaderProperty_DtdProcessing, error is %08.8lx", hr);
        return false;
    }

    if (FAILED(hr = pReader->SetInput(pFileStream)))
    {
        sprintf_s( szLastError, 256, "Error setting input for reader, error is %08.8lx", hr);
        return false;
    }

    //read until there are no more nodes
    while (S_OK == (hr = pReader->Read(&nodeType)))
    {
        switch (nodeType)
        {
        case XmlNodeType_Element:
            if (FAILED(hr = pReader->GetLocalName(&pwszLocalName, NULL)))
            {
                sprintf_s( szLastError, 256, "Error getting local name, error is %08.8lx", hr);
                return false;
            }
            
            if ( !pReader->IsEmptyElement() )
			{
				if ( wcscmp( pwszLocalName, L"object" ) == 0 )
				{
					MarketObject *pNewObject = new MarketObject( );
					
					bool bSuccess = pNewObject->FillValues( pReader );
					if ( !bSuccess )
					{
						sprintf_s( szLastError, 256, "Error creating market object: %s", MarketObject::GetLastError( ));
					}
					else
					{
						pNewObject->pNextObject = pCompleteList;
						pCompleteList = pNewObject;
					}
				}
			}
            break;
		}
    }

	MarketObject *pObject = pCompleteList;
	MarketObject *pNewList = NULL;
	dwMaxGroup = 0;

	//reverse the object list as it was created in reverse order
	while ( pCompleteList )
	{
		if ( pCompleteList->GetGroupID() > dwMaxGroup ) dwMaxGroup = pCompleteList->GetGroupID();

		pCompleteList = pObject->pNextObject;
		pObject->pNextObject = pNewList;
		pNewList = pObject;

		pObject = pCompleteList;
	}

	pCompleteList = pNewList;

	CString sPurchasedList = theApp.m_szDirectory;
	sPurchasedList += "\\Files\\TGCStore\\TEMP\\PurchasedObjects.xml";
	SetPurchasedObjects( sPurchasedList );

	return true;
}

/*
void MarketObject::CopyToSearchObjects( )
{
	ClearSearchObjects( );

	MarketObject *pObject = pAllObjects;

	while( pObject )
	{
		MarketObject *pNewObject = new MarketObject;
		pNewObject->Copy( pObject );

		pNewObject->pNextObject = pAllSearchObjects;
		pAllSearchObjects = pNewObject;

		pObject = pObject->pNextObject;
	}


	//reverse the object list as it was copied in reverse order
	pObject = pAllSearchObjects;
	MarketObject *pNewList = NULL;

	while ( pAllSearchObjects )
	{
		pAllSearchObjects = pObject->pNextObject;
		pObject->pNextObject = pNewList;
		pNewList = pObject;

		pObject = pAllSearchObjects;
	}

	pAllSearchObjects = pNewList;
}
*/

/*
void MarketObject::UpdateCartObjects( )
{
	MarketObject *pObject = pAllCartObjects;
	MarketObject *pMainObject = NULL;
	MarketObject *pNextObject = NULL;

	while ( pObject )
	{
		pNextObject = pObject->pNextObject;

		pMainObject = MarketObject::GetObject( pObject->GetID() );	
		if ( pMainObject )
		{
			pObject->Copy( pMainObject );
			pObject->pNextObject = pNextObject;
		}
		else
		{
			MarketObject::RemoveFromCart( pObject->GetID() );
		}

		pObject = pNextObject;
	}
}
*/

void MarketObject::Copy( const MarketObject* b )
{
	*this = *b;

	
	DWORD dwLength;
	/*
	if ( b->pszDescription ) 
	{
		dwLength = strlen( b->pszDescription );
		pszDescription = new char [ dwLength+1 ];
		strcpy_s( pszDescription, dwLength+1, b->pszDescription );
	}
	*/
	
	sDescription = b->sDescription;

	if ( b->pszThumbnail ) 
	{
		dwLength = strlen( b->pszThumbnail );
		pszThumbnail = new char [ dwLength+1 ];
		strcpy_s( pszThumbnail, dwLength+1, b->pszThumbnail );
	}

	if ( b->ppszKeywords && b->dwNumKeywords > 0 ) 
	{
		ppszKeywords = new char* [ b->dwNumKeywords ];

		for ( DWORD i = 0; i < b->dwNumKeywords; i++ )
		{
			dwLength = strlen( b->ppszKeywords [ i ] );
			ppszKeywords [ i ] = new char [ dwLength+1 ];
			strcpy_s( ppszKeywords [ i ], dwLength+1, b->ppszKeywords [ i ] );
		}

		dwNumKeywords = b->dwNumKeywords;
	}

	if ( b->ppszImages ) 
	{
		FillImageArray();
	}

	if ( b->pszPath ) 
	{
		dwLength = strlen( b->pszPath );
		pszPath = new char [ dwLength+1 ];
		strcpy_s( pszPath, dwLength+1, b->pszPath );
	}
}

void MarketObject::ClearObjects( )
{
	while( pAllObjects )
	{
		MarketObject *pObject = pAllObjects;
		pAllObjects = pAllObjects->pNextObject;

		delete pObject;
	}
}

void MarketObject::ClearGroups( )
{
	while( pAllGroups )
	{
		MarketGroup *pGroup = pAllGroups;
		pAllGroups = pAllGroups->pNextGroup;

		delete pGroup;
	}
}

void MarketObject::ClearSearchObjects( )
{
	while( pAllSearchObjects )
	{
		MarketObject *pObject = pAllSearchObjects;
		pAllSearchObjects = pAllSearchObjects->pNextObject;

		delete pObject;
	}
}

void MarketObject::ClearCartObjects( )
{
	while( pAllCartObjects )
	{
		MarketObject *pObject = pAllCartObjects;
		pAllCartObjects = pAllCartObjects->pNextObject;

		delete pObject;
	}
}

MarketObject* MarketObject::GetCompleteListObject( DWORD dwObjID )
{
	MarketObject *pObject = pCompleteList;

	DWORD count = 0;

	while ( pObject )
	{
		if ( pObject->GetID() == dwObjID )
		{
			return pObject;
		}
		
		pObject = pObject->pNextObject;
	}

	return NULL;
}

MarketObject* MarketObject::GetObjectByID( DWORD dwObjID, int iGroup )
{
	if ( iGroup == -1 ) return GetSearchObject( dwObjID );
	if ( iGroup == -2 ) return GetCartObject( dwObjID );
	if ( iGroup < 0 && iGroup != -4 ) return NULL;

	MarketObject *pObject = pAllObjects;

	DWORD count = 0;

	while ( pObject )
	{
		if ( pObject->GetID() == dwObjID )
		{
			return pObject;
		}
		
		pObject = pObject->pNextObject;
	}

	return NULL;
}

MarketObject* MarketObject::GetObjectByIndex( int iGroup, DWORD dwIndex )
{
	if ( iGroup == -1 ) return GetSearchObjectByIndex( dwIndex );
	if ( iGroup == -2 ) return GetCartObjectByIndex( dwIndex );
	if ( iGroup < 0 && iGroup != -4 ) return NULL;

	MarketObject *pObject = pAllObjects;

	DWORD count = 0;

	while ( pObject )
	{
		if ( iGroup == -4 || pObject->GetGroupID() == (DWORD) iGroup )
		{
			if ( count == dwIndex ) return pObject;
			count++;
		}
		
		pObject = pObject->pNextObject;
	}

	return NULL;
}

/*
MarketObject* MarketObject::GetObject( int iGroup, LPCSTR szName )
{
	if ( iGroup == -1 ) return GetSearchObject( szName );
	if ( iGroup == -2 ) return GetCartObject( szName );
	if ( iGroup < 0 && iGroup != -4 ) return NULL;

	MarketObject *pObject = pAllObjects;

	//DWORD count = 0;

	while ( pObject )
	{
		if ( (pObject->GetGroupID() == (DWORD) iGroup) && strcmp( pObject->GetName(), szName ) == 0 )
		{
			return pObject;
		}
		
		pObject = pObject->pNextObject;
	}

	return NULL;
}
*/

MarketObject* MarketObject::GetSearchObjectByIndex( DWORD dwIndex )
{
	MarketObject *pObject = pAllSearchObjects;

	DWORD count = 0;

	while ( pObject )
	{
		if ( count == dwIndex ) return pObject;
		count++;
				
		pObject = pObject->pNextObject;
	}

	return NULL;
}

/*
MarketObject* MarketObject::GetSearchObject( LPCSTR szName )
{
	MarketObject *pObject = pAllSearchObjects;

	DWORD count = 0;

	while ( pObject )
	{
		if ( strcmp( pObject->GetName(), szName ) == 0 ) return pObject;
				
		pObject = pObject->pNextObject;
	}

	return NULL;
}
*/

MarketObject* MarketObject::GetSearchObject( DWORD dwObjID )
{
	MarketObject *pObject = pAllSearchObjects;

	DWORD count = 0;

	while ( pObject )
	{
		if ( pObject->GetID() == dwObjID ) return pObject;
		
		pObject = pObject->pNextObject;
	}

	return NULL;
}

MarketObject* MarketObject::GetCartObjectByIndex( DWORD dwIndex )
{
	MarketObject *pObject = pAllCartObjects;

	DWORD count = 0;

	while ( pObject )
	{
		if ( count == dwIndex ) return pObject;
		count++;
				
		pObject = pObject->pNextObject;
	}

	return NULL;
}

MarketObject* MarketObject::GetCartObject( DWORD dwObjID )
{
	MarketObject *pObject = pAllCartObjects;

	DWORD count = 0;

	while ( pObject )
	{
		if ( pObject->GetID() == dwObjID ) return pObject;
		
		pObject = pObject->pNextObject;
	}

	return NULL;
}

/*
MarketObject* MarketObject::GetCartObject( LPCSTR szName )
{
	MarketObject *pObject = pAllCartObjects;

	DWORD count = 0;

	while ( pObject )
	{
		if ( strcmp( pObject->GetName(), szName ) == 0 ) return pObject;
				
		pObject = pObject->pNextObject;
	}

	return NULL;
}
*/

bool MarketObject::GroupContainsObjects( DWORD dwGroupID )
{
	MarketObject *pObject = pAllObjects;

	while ( pObject )
	{
		if ( pObject->GetGroupID() == dwGroupID )
		{
			return true;
		}
		
		pObject = pObject->pNextObject;
	}

	return false;
}

MarketGroup* MarketObject::SearchChildGroups( MarketGroup *pGroup, DWORD dwGroup )
{
	while ( pGroup )
	{
		if ( pGroup->dwGroupID == dwGroup ) return pGroup;
		
		MarketGroup *pFoundGroup = SearchChildGroups( pGroup->pChildGroup, dwGroup );
		if ( pFoundGroup ) return pFoundGroup;

		pGroup = pGroup->pNextGroup;
	}

	return NULL;
}

MarketGroup* MarketObject::GetGroup( int iGroup )
{
	if ( iGroup < 0 ) return NULL;
	return SearchChildGroups( pAllGroups, (DWORD) iGroup );
}

MarketGroup* MarketObject::SearchChildGroups( MarketGroup *pGroup, const char *szGroupName )
{
	while ( pGroup )
	{
		if ( strcmp( pGroup->szName, szGroupName ) == 0 ) return pGroup;
		
		//MarketGroup *pFoundGroup = SearchChildGroups( pGroup->pChildGroup, szGroupName );
		//if ( pFoundGroup ) return pFoundGroup;

		pGroup = pGroup->pNextGroup;
	}

	return NULL;
}

MarketGroup* MarketObject::GetGroup( const char *szGroupName )
{
	if ( !szGroupName || strlen( szGroupName ) == 0 ) return NULL;

	const char *szRemaining = szGroupName;
	const char *szSlash = strchr( szRemaining, '\\' );
	char szTempName [ 256 ];

	MarketGroup *pCurrentGroup = pAllGroups;

	while ( szSlash )
	{
		strncpy_s( szTempName, 256, szRemaining, (DWORD) (szSlash - szRemaining) );
		
		pCurrentGroup = SearchChildGroups( pCurrentGroup, szTempName );
		if ( !pCurrentGroup ) return NULL;
		else pCurrentGroup = pCurrentGroup->pChildGroup;

		szRemaining = szSlash + 1;
		szSlash = strchr( szRemaining, '\\' );
	}

	return SearchChildGroups( pCurrentGroup, szRemaining );
}

bool MarketObject::ConstructGroupPath( DWORD dwID, char *szGroupPath, DWORD dwSize )
{
	MarketGroup *pGroup = GetGroup( dwID );
	if ( !pGroup ) return false;

	CString sGroupPath = pGroup->szName;
	CString sTemp;

	while ( pGroup->dwParentID )
	{
		pGroup = GetGroup( pGroup->dwParentID );
		if ( !pGroup ) break;
		sTemp = sGroupPath;
		
		sGroupPath = pGroup->szName;
		sGroupPath += "\\";
		sGroupPath += sTemp;
	}

	strcpy_s( szGroupPath, dwSize, sGroupPath );

	return true;
}

int MarketObject::CountObjects( MarketGroup *pGroup )
{
	if ( !pGroup ) pGroup = pAllGroups;
	int count = 0;

	while ( pGroup )
	{
		count += pGroup->dwAmount;
		if ( pGroup->pChildGroup ) count += CountObjects( pGroup->pChildGroup );
		pGroup = pGroup->pNextGroup;
	}

	return count;
}

int MarketObject::CountSearchObjects( )
{
	MarketObject *pObject = pAllSearchObjects;
	int count = 0;

	while ( pObject )
	{
		count++;
		pObject = pObject->pNextObject;
	}

	return count;
}

int MarketObject::CountCartObjects( )
{
	MarketObject *pObject = pAllCartObjects;
	int count = 0;

	while ( pObject )
	{
		count++;
		pObject = pObject->pNextObject;
	}

	return count;
}

bool MarketObject::IsObjectInCart( DWORD dwID )
{
	MarketObject *pObject = pAllCartObjects;

	while ( pObject )
	{
		if ( pObject->GetID() == dwID ) return true;
		pObject = pObject->pNextObject;
	}

	return false;
}

void MarketObject::AddToCart( const MarketObject *pObject )
{
	MarketObject *pNewObject = new MarketObject;

	pNewObject->Copy( pObject );
	pNewObject->pNextObject = pAllCartObjects;
	pAllCartObjects = pNewObject;
}

void MarketObject::RemoveFromCart( DWORD dwID )
{
	MarketObject *pObject = pAllCartObjects;
	MarketObject *pPrevObject = NULL;

	while ( pObject )
	{
		if ( pObject->GetID() == dwID )
		{
			if ( pPrevObject ) pPrevObject->pNextObject = pObject->pNextObject;
			else pAllCartObjects = pObject->pNextObject;

			delete pObject;
			return;
		}

		pPrevObject = pObject;
		pObject = pObject->pNextObject;
	}
}

void MarketObject::RemoveObject( DWORD dwID )
{
	MarketObject *pObject = pAllObjects;
	MarketObject *pPrevObject = NULL;

	while ( pObject )
	{
		if ( pObject->GetID() == dwID )
		{
			if ( pPrevObject ) pPrevObject->pNextObject = pObject->pNextObject;
			else pAllObjects = pObject->pNextObject;

			delete pObject;
			break;
		}

		pPrevObject = pObject;
		pObject = pObject->pNextObject;
	}

	pObject = pAllSearchObjects;
	pPrevObject = NULL;

	while ( pObject )
	{
		if ( pObject->GetID() == dwID )
		{
			if ( pPrevObject ) pPrevObject->pNextObject = pObject->pNextObject;
			else pAllSearchObjects = pObject->pNextObject;

			delete pObject;
			break;
		}

		pPrevObject = pObject;
		pObject = pObject->pNextObject;
	}

	pObject = pCompleteList;
	pPrevObject = NULL;

	while ( pObject )
	{
		if ( pObject->GetID() == dwID )
		{
			if ( pPrevObject ) pPrevObject->pNextObject = pObject->pNextObject;
			else pCompleteList = pObject->pNextObject;

			delete pObject;
			break;
		}

		pPrevObject = pObject;
		pObject = pObject->pNextObject;
	}

	pObject = pAllCartObjects;
	pPrevObject = NULL;

	while ( pObject )
	{
		if ( pObject->GetID() == dwID )
		{
			if ( pPrevObject ) pPrevObject->pNextObject = pObject->pNextObject;
			else pAllCartObjects = pObject->pNextObject;

			delete pObject;
			return;
		}

		pPrevObject = pObject;
		pObject = pObject->pNextObject;
	}
}

DWORD MarketObject::GetHighestGroup( MarketGroup* pGroup )
{
	if ( !pGroup ) return 0;
	DWORD iHighest = 0;

	while ( pGroup )
	{
		if ( pGroup->dwGroupID > iHighest ) iHighest = pGroup->dwGroupID;
		if ( pGroup->pChildGroup )
		{
			DWORD iChild = GetHighestGroup( pGroup->pChildGroup );
			if ( iChild > iHighest ) iHighest = iChild;
		}

		pGroup = pGroup->pNextGroup;
	}

	return iHighest;
}

bool MarketObject::FillSearchArray( MarketGroup* pGroup, int *pGroupArray, const char *szKeyword, bool bExact, bool bFound )
{
	if ( !szKeyword ) return false;
	if ( !pGroup ) return false;
	if ( !pGroupArray ) return false;

	char str [ 50 ];

	while ( pGroup )
	{
		pGroupArray [ pGroup->dwGroupID ] = 0;

		bool bMatch = false;

		if ( bFound ) bMatch = true;
		else
		{
			strcpy_s( str, 50, pGroup->szName ); strlower( str );

			if ( bExact ) bMatch = (strcmp( str, szKeyword ) == 0);
			else bMatch = (strstr( str, szKeyword ) != NULL );
		}

		if ( pGroup->pChildGroup ) FillSearchArray( pGroup->pChildGroup, pGroupArray, szKeyword, bExact, bMatch );

		if ( bMatch ) 
		{
			pGroupArray [ pGroup->dwGroupID ] = 1;
		}

		pGroup = pGroup->pNextGroup;
	}

	return true;
}

void MarketObject::Search( LPCSTR szKeyword, bool bAttentionObjectsOnly, bool bExact )
{
	ClearSearchObjects( );

	if ( bAttentionObjectsOnly )
	{
		if ( theApp.m_bAdmin )
		{
			MarketObject *pObject = pCompleteList;
			while( pObject )
			{
				if ( !pObject->IsApproved() || !pObject->IsAvailable() )
				{
					MarketObject *pNewObject = new MarketObject();
					pNewObject->Copy( pObject );
					pNewObject->pNextObject = pAllSearchObjects;
					pAllSearchObjects = pNewObject;
				}
				
				pObject = pObject->pNextObject;
			}
		}
	}
	else
	{
		if ( !szKeyword || strlen(szKeyword) < 1 ) return;
		
		char l_szKeyword [ 50 ];
		strcpy_s( l_szKeyword, 50, szKeyword ); 
		strlower( l_szKeyword );

		int *piGroups = NULL;
		DWORD iHighest = GetHighestGroup( pAllGroups );
		if ( iHighest > 0 )
		{
			piGroups = new int [ iHighest + 1 ];
			for ( DWORD i = 0; i <= iHighest; i++ ) piGroups [ i ] = 0;
		}

		FillSearchArray( pAllGroups, piGroups, l_szKeyword, bExact, false );

		MarketObject *pObject = pCompleteList;
		while( pObject )
		{
			char str [ 50 ];

			strcpy_s( str, 50, pObject->GetName() ); strlower( str );

			bool bNameMatch = false;
			bool bKeywordMatch = false;
			bool bGroupMatch = false;
			bool bRightsMatch = false;

			if ( pObject->IsAvailable() && pObject->IsApproved() ) bRightsMatch = true;
			else
			{
				if ( theApp.m_bAdmin ) bRightsMatch = true;
				else if ( theApp.m_bUploader )
				{
					if ( strcmp( pObject->GetAuthor(), theApp.m_szAuthorName ) == 0 ) bRightsMatch = true;
				}
			}

			if ( bExact )
			{
				bNameMatch = (strcmp( str, l_szKeyword ) == 0);
				bKeywordMatch = pObject->SearchKeywords( l_szKeyword, true );
			}
			else
			{
				bNameMatch = (strstr( str, l_szKeyword ) != NULL );
				bKeywordMatch = pObject->SearchKeywords( l_szKeyword );
			}

			if ( piGroups && pObject->GetGroupID() <= iHighest ) bGroupMatch = (piGroups [ pObject->GetGroupID() ] > 0);

			if ( bRightsMatch && (bNameMatch || bKeywordMatch || bGroupMatch) )
			{
				MarketObject *pNewObject = new MarketObject();
				pNewObject->Copy( pObject );
				pNewObject->pNextObject = pAllSearchObjects;
				pAllSearchObjects = pNewObject;
			}
			
			pObject = pObject->pNextObject;
		}

		if ( piGroups ) delete [] piGroups;
		piGroups = NULL;
	}

	MarketObject *pObject = pAllSearchObjects;
	MarketObject *pNewList = NULL;

	//reverse the object list as it was created in reverse order
	while ( pAllSearchObjects )
	{
		pAllSearchObjects = pObject->pNextObject;
		pObject->pNextObject = pNewList;
		pNewList = pObject;

		pObject = pAllSearchObjects;
	}

	pAllSearchObjects = pNewList;
}