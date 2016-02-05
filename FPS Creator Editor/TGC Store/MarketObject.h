#ifndef _H_MARKETOBJECT
#define _H_MARKETOBJECT

#include <windows.h>
//#include <tchar.h>

#define MKT_OBJ_ID		0x00000001
#define MKT_GRP_ID		0x00000002
#define MKT_CATEGORY	0x00000004
#define MKT_TYPE		0x00000008
#define MKT_NAME		0x00000010
#define MKT_AUTHOR		0x00000020
#define MKT_COST		0x00000040
#define MKT_DESC		0x00000080
#define MKT_THUMB		0x00000100
#define MKT_N_IMGS		0x00000200
#define MKT_IMGS		0x00000400
#define MKT_SIZE		0x00000800
#define MKT_T_RAT		0x00001000
#define MKT_C_RAT		0x00002000
#define MKT_PATH		0x00004000
#define MKT_POLYS		0x00008000
#define MKT_SUBCATEGORY	0x00010000
#define MKT_X9			0x00020000
#define MKT_X10			0x00040000
#define MKT_KEYWORD		0x00080000
#define MKT_ALL			MKT_OBJ_ID | MKT_GRP_ID | MKT_CATEGORY | MKT_TYPE | MKT_NAME | MKT_AUTHOR | MKT_COST | MKT_DESC | MKT_THUMB | MKT_N_IMGS \
						| MKT_IMGS | MKT_SIZE | MKT_T_RAT | MKT_C_RAT | MKT_POLYS | MKT_SUBCATEGORY | MKT_X9 | MKT_X10 /*| MKT_PATH*/

struct IXmlReader;

struct MarketGroup
{
public:
	char szName [ 64 ];
	DWORD dwGroupID;
	DWORD dwParentID;
	DWORD dwAmount;

	MarketGroup *pNextGroup;
	MarketGroup *pChildGroup;

	MarketGroup( );
	~MarketGroup( );

	bool FillValues( IXmlReader* pReader );
	bool comp( const MarketGroup* b ) const;
};

class MarketObject
{

public:

	MarketObject( );
	~MarketObject( );

	void Clear( );
	void SetDefaultValues( );
	bool FillValues( IXmlReader* pReader );
	bool SetValue( const char *szAttribute, const char *szValue );

	DWORD GetID( ) const;
	DWORD GetGroupID( ) const;
	const char* GetName( ) const;
	const char* GetType( ) const;
	const char* GetCategory( ) const;
	const char* GetSubCategory( ) const;
	const char* GetAuthor( ) const;
	DWORD GetCost( ) const;
	const char* GetDescription( ) const;
	const char* GetThumbnail( ) const;
	DWORD GetNumImages( ) const;
	const char* GetImage( DWORD dwIndex ) const;
	DWORD GetNumKeywords( ) const;
	const char* GetKeyword( DWORD dwIndex ) const;
	const char* GetVideoLink( ) const;
	bool SearchKeywords( const char *szKeyword, bool bExactMatch = false );
	DWORD GetModelFileSize( ) const;
	DWORD GetTotalRating( ) const;
	DWORD GetRatingCount( ) const;
	DWORD GetRating( ) const;
	DWORD GetPolygons( ) const;
	DWORD GetFileSize( ) const;
	DWORD GetUserDownloads( ) const;
	DWORD GetDownloads( ) const;
	const char* GetPath( ) const;
	bool IsPurchased( ) const;
	bool IsComplete( ) const;
	bool IsAvailable( ) const;
	bool IsApproved( ) const;
	bool IsRated( ) const;
	bool IsX9Compatible() const;
	bool IsX10Compatible() const;
	void UpdateRating( DWORD newrating );

	bool comp( const MarketObject* b ) const;
	void Copy( const MarketObject* b );

	void AddKeyword( const char *szKeyword );
	void ParseKeywords( const char *szXMLKeywords );
	void DeleteKeywordArray( );

	void DeleteImageArray( );
	void FillImageArray( );
	void CompleteObject( char *pszXMLFile );

	MarketObject *pNextObject;

	static const char* GetLastError( );
	static bool LoadObjects( const char *pszXMLFile );
	static bool LoadCompleteList( const char *pszXMLFile );
	static bool LoadGroups( const char *pszXMLFile );
	static bool SetPurchasedObjects( const char *szFilename );
	static bool SetKeywords( const char *pszXMLFile );
	static bool LoadSearchObjects( const char *szFile );
	//static void CopyToSearchObjects( );
	//static void UpdateCartObjects( );
	static MarketObject* GetObjectList( ) { return pAllObjects; }
	static MarketObject* GetSearchList( ) { return pAllSearchObjects; }
	static MarketObject* GetCompleteList( ) { return pCompleteList; }
	static MarketObject* GetCartList( ) { return pAllCartObjects; }
	static MarketGroup* GetGroupList( ) { return pAllGroups; }
	static void ClearObjects( );
	static void ClearSearchObjects( );
	static void ClearCartObjects( );
	static void ClearGroups( );
	static MarketObject* GetObjectByID( DWORD dwObjID, int iGroup  );
	static MarketObject* GetObjectByIndex( int iGroup, DWORD dwIndex );
	//static MarketObject* GetObject( int iGroup, LPCSTR szName );
	static MarketObject* GetCompleteListObject( DWORD dwObjID );
	static MarketObject* GetSearchObjectByIndex( DWORD dwIndex );
	//static MarketObject* GetSearchObject( LPCSTR szName );
	static MarketObject* GetSearchObject( DWORD dwID );
	static MarketObject* GetCartObject( DWORD dwID );
	static MarketObject* GetCartObjectByIndex( DWORD dwIndex );
	//static MarketObject* GetCartObject( LPCSTR szName );
	static MarketGroup* GetGroup( int iGroup );
	static MarketGroup* GetGroup( const char *szGroupName );
	static void AddToCart( const MarketObject *pObject );
	static void RemoveFromCart( DWORD dwID );
	static bool IsObjectInCart( DWORD dwID );
	static int CountObjects( MarketGroup *pGroup = NULL );
	static int CountSearchObjects( );
	static int CountCartObjects( );
	static bool GroupContainsObjects( DWORD dwGroupID );
	static bool CleanupGroups( MarketGroup *pGroup );
	static void Search( LPCSTR szKeyword, bool bAttentionObjectsOnly, bool bExact );

	static bool ConstructGroupPath( DWORD dwID, char *szGroupPath, DWORD dwSize );

	static void RemoveObject( DWORD dwID );

	static void ResetObjectCompleteness( );
	static void SetSortIndex( LPCSTR szOrderBy );

private:

	void SetDescription( const char* szDescription );

	static int iSortItem;
	static MarketObject* pAllObjects;
	static MarketObject* pAllSearchObjects;
	static MarketObject* pCompleteList;
	static MarketObject* pAllCartObjects;
	static MarketGroup* pAllGroups;

	static int *piGroupArrayCounter;
	static DWORD dwMaxGroup;

	DWORD dwID;
	DWORD dwGroupID;
	char szCategory [ 64 ];
	char szSubCategory [ 64 ];
	char szType [ 64 ];
	char szName [ 64 ];
	char szAuthor [ 64 ];
	char szVideoLink [ 255 ];
	DWORD dwCost;
	CString sDescription;
	char *pszThumbnail;
	DWORD dwNumImages;
	char **ppszImages;
	DWORD dwNumKeywords;
	char **ppszKeywords;
	DWORD dwFileSize;
	DWORD dwTotalRating;
	DWORD dwRatingCount;
	DWORD dwPolygons;
	char *pszPath;
	DWORD dwPurchased;
	DWORD dwComplete;
	DWORD dwIsRated;
	DWORD dwUserDownloads;
	DWORD dwDownloads;
	bool bAvailable;
	bool bApproved;
	bool bX9Compatible;
	bool bX10Compatible;

	static char szLastError [ 256 ];

	static MarketGroup* SearchChildGroups( MarketGroup* pGroup, DWORD dwGroup );
	static MarketGroup* SearchChildGroups( MarketGroup* pGroup, const char *szGroupName );

	static DWORD GetHighestGroup( MarketGroup* pGroup );
	static bool FillSearchArray( MarketGroup* pGroup, int *pGroupArray, const char *szKeyword, bool bExact, bool bFound );

};

#endif