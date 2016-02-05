#include "stdafx.h"
#include "Global.h"

void strupper( char* str )
{
	if ( !str ) return;
	while ( *str ) { int ch = *str; ch = toupper( ch ); *str = ch; str++; }
}

void strlower( char* str )
{
	if ( !str ) return;
	while ( *str ) { int ch = *str; ch = tolower( ch ); *str = ch; str++; }
}