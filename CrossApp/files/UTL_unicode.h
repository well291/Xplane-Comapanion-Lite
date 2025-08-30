#ifndef UTL_unicode_h
#define UTL_unicode_h

typedef	unsigned char	UTF8;	// Definitions for chars in each of the 3 encoding formats.
typedef	unsigned short	UTF16;	// We will use our normal string for UTF8.  We do not make a
typedef unsigned int	UTF32;	// string for UTF32 because it is only used for 1 char at a time.

typedef basic_string<UTF16>	string_utf16;	// Just like an STL string, but made of 16-bit UTF 16 chars.
typedef basic_string<UTF32>	string_utf32;

inline		 UTF8* 	UTF8_align(		 UTF8 * string);
inline const UTF8* 	UTF8_align(const UTF8 * string);
inline		 UTF8* 	UTF8_prev (		 UTF8 * string);
inline const UTF8* 	UTF8_prev (const UTF8 * string);
inline		 UTF8* 	UTF8_next (		 UTF8 * string);
inline const UTF8* 	UTF8_next (const UTF8 * string);

inline xint			UTF8_IsValid(const UTF8 * start, const UTF8 * end);			// Is this a valid UTF8 string?  UTF8 has string bit-patterns; often we can tell if we are UTF8 or not!
inline xint			UTF8_IsValid(const string& utf8_str);						// Just for convenience.
inline xint			UTF8_IsASCII(const string& utf8_str);
inline UTF32		UTF8_decode (const	UTF8 * chars );							// Decode UTF8 - chars must contain enough chars to make one valid unicode char.
inline xint			UTF8_encode (		UTF32  inChar, UTF8		outChars[4]);	// Encode char into buf, return number of bytes was filled.
inline const UTF16* UTF16_decode(const	UTF16* chars , UTF32&	result);		// UTF 16 decoder.  Advance one unicode char, return UTF32 result.
inline xint			UTF16_encode(		UTF32  inChar, UTF16	outChars[2]);	// UTF 16 encoder.  Fill buffer, return how many UTF16s was filled.

void string_utf_8_to_16(const string&		input, string_utf16&	output);	// convert between UTF8 and 16 in STL strings.
void string_utf_16_to_8(const string_utf16& input, string&			output);
void string_utf_8_to_16(const string&		input, wstring&			output);	// convert between UTF8 and 16 in STL strings.
void string_utf_16_to_8(const wstring&		input, string&			output);
void string_utf_32_to_8(const string_utf32& input, string&			output);
void string_utf_32_to_8(const wstring&		input, string&			output);

inline const UTF8* UTF8_align(const UTF8 * string)
{
	while((*string & 0xC0) == 0x80)
		--string;
	return string;
}

inline const UTF8* UTF8_next(const UTF8 * string)
{
	string = UTF8_align(string);
	++string;

	while((*string & 0xC0) == 0x80)
		++string;

	return string;		
}

inline xint	UTF8_IsValid(const UTF8 * s, const UTF8 * e)
{
	while(s < e)
	{
		if ((*s & 0x80) == 0)		{ ++s; continue;	}				// ASCII case - advance one and done		

		xint extra_bytes = 0;											// 3 lead bit patters; calc extra bytes
		     if((*s & 0xE0) == 0xC0){ extra_bytes = 1; }				// 110x xxxx 
		else if((*s & 0xF0) == 0xE0){ extra_bytes = 2; }				// 1111 0xxx
		else if((*s & 0xF8) == 0xF0){ extra_bytes = 3; }				// 1111 10xx
		else return xfals;												// Lead bytes don't match anything legit - not UTF8
		++s;
		while(extra_bytes--)
		{
			if(s >= e)					return xfals;					// End of string without adequate bytes for possible trailing.
			if ((*s++ & 0xC0) != 0x80)	return xfals;					// For each trailing byte, if not 10xxxxxx, fail
		}
	}
	return xtrue;
}

inline xint	UTF8_IsValid(const string& utf8_str)
{
	return UTF8_IsValid((const UTF8 *) utf8_str.c_str(),(const UTF8 *) utf8_str.c_str() + utf8_str.size());
}

inline UTF32 UTF8_decode(const UTF8 * chars)
{
	if((*chars & 0x80) == 0) return *chars;								// Fast case - high bit not set, ASCII, return it.
	
	xint extra_bytes = 0;												// How many more bytes of payload
	xint mask = 0;														// How much of lead bytes is char
	
	if((*chars & 0xE0) == 0xC0)	{ extra_bytes = 1; mask = 0x1F; }		// 110x xxxx	five bits payload, one extra bytes
	if((*chars & 0xF0) == 0xE0)	{ extra_bytes = 2; mask = 0x0F; }		// 1110 xxxx	four bits payload, two extra bytes
	if((*chars & 0xF8) == 0xF0) { extra_bytes = 3; mask = 0x07; }		// 1111 0xxx	three bits payload, three extra bytes
	
	UTF32 total = *chars & mask;										// Grab bits from the first char
	++chars;
		
	while(extra_bytes--) {												// For each additional byte, shift what we had by 6 bits
		total <<= 6;													// and bring in six bits of payload.
		total |= (*chars++ & 0x3F);
	}
	return total;
}
#endif
