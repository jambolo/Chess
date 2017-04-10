#include <cstdio>
#include <vector>
#include <cassert>
#include <locale>
#include <iostream>
#include <sstream>

#include "../../Libraries/Misc/etc.h"
#define ASSERT							assert
#define ASSERT_MEMSET_VALID				assert_memset_valid
#define ASSERT_LIMITS					assert_limits

#include "../Move.h"
#include "../TranspositionTable.h"


class Node;
class Branches : public std::vector< Node * > {};
class Sequence : public std::vector< Move > {};

class Node
{
public:

	Move		m_Move;
	std::string	m_Name;
	std::string	m_Id;
	Branches	m_Branches;	
};

//class DataEntry
//{
//public:
//	int			m_Ply;			// Which ply this is on
//	TableEntry	m_TableEntry;	// Transposition table entry
//};
//
//class DataList : public std::vector< TDataEntry >;

int main( int argc, char * * argv )
{
	// Open the file

	FILE * fp = fopen( "ECOextended.txt", "r" );
	if ( !fp )
	{
		fputs( "Can't open 'ECOextended.txt'.\n", stderr );
		exit( 1 );
	}

	// Load the file

	Branches root;
	int line_number = 0;
	enum FileState
	{
		FS_READING_HEADER,
		FS_READING_NAME,
		FS_READING_SEQUENCE
	} file_state = FS_READING_HEADER;

	bool error_detected = false;
	char line[ 1024 ];
	std::ostringstream error_message;
	std::string	eco_code;
	std::string eco_name;

	while ( !error_detected )
	{

		++line_number;

		if ( fgets( line, sizeof line, fp ) == NULL )
			break;

		line[ strlen( line ) - 1 ] = 0;		// Chop off \n

		switch ( file_state )
		{
		case FS_READING_HEADER:
		{
			// Make sure we are really reading a header
			//                                   0123456789 123456789 123456789 123456789 123456789 123456789 123456789 123456
			std::string const header_prototype( "------------------------------------ xnn ------------------------------------" );

			std::string const left_dashes ( "------------------------------------ " );
			std::string const right_dashes( " ------------------------------------" );

			if ( strlen( line ) != 77 )
			{
				error_detected = true;
				error_message << "Invalid line length " << std::ends;
				break;
			}
			if ( left_dashes.compare( 0, 37, &line[0], 37 ) != 0 ||
				 right_dashes.compare( 0, 37, &line[40], 37 ) != 0 ||
			     line[37] < 'A' || line[37] > 'E' ||
				 line[38] < '0' || line[38] > '9' ||
				 line[39] < '0' || line[39] > '9' )
			{
				error_detected = true;
				error_message << "Expecting: " << header_prototype << std::endl
							 << " but found " << std::endl
							 << line << std::ends;
				break;
			}

			// Grab the ECO code

			eco_code = std::string( line, 37, 3 );
			std::cerr << "Reading " << eco_code << std::endl;

			// Next is a name
			
			file_state = FS_READING_NAME;

			break;
		}

		case FS_READING_NAME:
		{
			char * code_start;

			// Scan the line for the first non-space

			for ( code_start = line; isspace( *code_start ) && *code_start != 0; code_start++ )
			{
			}
			
			// Check for blank line which means end of section

			if ( *code_start == 0 )
			{
				// Next is a header
				
				file_state = FS_READING_HEADER;

				break;
			}

			// Now there should be a code with the format xnn followed by a space (and then the name)

			if ( strlen( code_start ) < 4 || !isspace( code_start[3] ) )
			{
				error_detected = true;
				error_message << "Invalid code: " << code_start << std::ends;
				break;
			}

			// The code should match the code for this section

			std::string const this_code( code_start, 3 );
			if ( eco_code.compare( this_code ) != 0 )
			{
				error_detected = true;
				error_message << "ECO code mismatch: expecting '" << eco_code
							 << "', found '" << this_code << "'"
							 << std::ends;
				break;
			}

			// Make sure a name follows the code
			eco_name = &code_start[4];
			if ( eco_name.size() <= 0 )
			{
				error_detected = true;
				error_message << "Missing opening name" << std::ends;
				break;
			}

			// Next is a sequence
			
			file_state = FS_READING_SEQUENCE;

			break;
		}

		case FS_READING_SEQUENCE:
		{
			char * sequence_start;

			for ( sequence_start = line; isspace( *sequence_start ) && *sequence_start != 0; sequence_start++ )
			{
			}
			
			if ( strlen( sequence_start ) < 6 ||
				 sequence_start[0] != '1' || sequence_start[1] != '.' )
			{
				error_detected = true;
				error_message << "Invalid sequence: " << sequence_start << std::ends;
				break;
			}

			// Load the sequence

			char *	p	= &sequence_start[ 2 ];
			int		length		= 0;
			int		chars_left	= strlen( p );

			Sequence sequence( ( chars_left + 1 ) / 5 );

			while ( chars_left >= 4 )
			{
				sequence[ length ].From().m_Column	= *p++ - 'a';
				sequence[ length ].From().m_Row		= *p++ - '1';
				sequence[ length ].To().m_Column	= *p++ - 'a';
				sequence[ length ].To().m_Row		= *p++ - '1';
				chars_left -= 4;

				if ( ( sequence[ length ].From().m_Column < 0 || sequence[ length ].From().m_Column > 7 ) ||
					 ( sequence[ length ].From().m_Row < 0    || sequence[ length ].From().m_Row > 7 ) ||
					 ( sequence[ length ].To().m_Column < 0   || sequence[ length ].To().m_Column > 7 ) ||
					 ( sequence[ length ].To().m_Row < 0      || sequence[ length ].To().m_Row > 7 ) )
				{
					error_detected = true;
					error_message << "Invalid move: " << p - 4 << std::ends;
					break;
				}

				if ( *p != 0 )
				{
					if ( !isspace( *p )
					{
						error_detected = true;
						error_message << "Invalid move: " << p - 4 << std::ends;
						break;
					}
					++p;
					--chars_left;
				}
				++length;
			}

			if ( error_detected )
				break;

			// Put the sequence into the tree

			// Next is a name
			
			file_state = FS_READING_NAME;

			break;
		}

		default:
			assert( false );
			break;
		}
	}

	fclose( fp );

	// Display errors

	if ( error_detected )
	{
		std::cerr << "Error detected in line " << line_number << std::endl
				  << "Read    : '" << line << "'." << std::endl
				  << error_message.str() << std::endl;
		exit( 1 );
	}

	// Output the processed data


	return 0;
}