#include "stdafx.h"
#include "StringFunc.h"

void _tcstoupper( TCHAR* str1 )
{
	if ( !str1 ) return;
	
	while ( *str1 )
	{
		int ch = *str1;
		ch = _totupper( ch );
		*str1 = ch;

		str1++;
	}
}

void _tcstolower( TCHAR* str1 )
{
	if ( !str1 ) return;
	
	while ( *str1 )
	{
		int ch = *str1;
		ch = _totlower( ch );
		*str1 = ch;

		str1++;
	}
}