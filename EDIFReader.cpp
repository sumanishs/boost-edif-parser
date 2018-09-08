#include "EDIFReader.hpp"
#include <boost/bind.hpp>
#include "EDIFParserFunctors.hpp"
//
//  This grammar contains the matter that is to be skipped by parser.
//
struct skip_grammar : public grammar<skip_grammar>
{
    template <typename ScannerT>
    struct definition
    {
        definition(skip_grammar const&)
        {
			EOL = ch_p('\n'); 
            skip
                =   comment_p("//")                
                |   comment_p("/*", "*/")      
				|   EOL  
				;
        }

        rule<ScannerT> skip;
		rule<ScannerT> EOL;
        rule<ScannerT> const&
        start() const { return skip; }
    };
};



struct edif_grammar : public boost::spirit::grammar<edif_grammar>
{
    
    template <typename ScannerT>
    struct definition
    {

        definition(edif_grammar const& self ) : EDIF("edif"),
        EDIFVERSION("edifVersion"), EDIFLEVEL("edifLevel"), STATUS("status"), WRITTEN("written"),
        TIMESTAMP("timeStamp"), PROGRAM("program"), PROGVERSION("Version"), DATAORIGIN("dataOrigin"),
        AUTHOR("author"), INT("int"), LOGIC_WIDTH("LOGIC_WIDTH"), 
        LOGIC_HEIGHT("LOGIC_HEIGHT"), FOR("for"), LOCATION("location")
        {
            using namespace phoenix;
            LEFT_BRACE  =   ch_p('{') [PrintChar()];
            RIGHT_BRACE =   ch_p('}') [PrintChar()];
            LEFT_PARAN  =   ch_p('(') [PrintChar()];
            RIGHT_PARAN  =   ch_p(')') [PrintChar()];
			SPACE		= *space_p [PrintChar()];
            any_string  = ch_p('"') 
                          >> string_val [PrintStrLit()]
                          >> ch_p('"')
                          ;

            string_val = *(anychar_p - ch_p('"'));

            edifversion_section
                = LEFT_PARAN
                  >> EDIFVERSION [PrintTag()]
                  >> SPACE
                  >> int_p [PrintInteger()] 
                  >> SPACE
                  >> int_p [PrintInteger()]
                  >> SPACE
                  >> int_p [PrintInteger()]
                  >> RIGHT_PARAN
                  ;   

            ediflevel_section
                =  LEFT_PARAN
                    >> SPACE
                    >> EDIFLEVEL [PrintTag()]
                    >> SPACE
                    >> int_p [PrintInteger()]
                    >> SPACE
                    >> RIGHT_PARAN
                    ;

            timestamp_section
                = LEFT_PARAN
                    >> SPACE
                    >> TIMESTAMP [PrintTag()]
                    >> SPACE
                    >> int_p [PrintInteger()]
                    >> SPACE
                    >> int_p [PrintInteger()]
                    >> SPACE
                    >> int_p [PrintInteger()]
                    >> SPACE
                    >> int_p [PrintInteger()]
                    >> SPACE
                    >> int_p [PrintInteger()]
                    >> SPACE
                    >> int_p [PrintInteger()]
                    >> SPACE
                    >> RIGHT_PARAN
                    ;
                    

            written_section
                = LEFT_PARAN
                    >> SPACE
                    >> WRITTEN [PrintTag()]
                    >> SPACE
                    >> timestamp_section
                    >> SPACE
                    >> program_section
                    >> SPACE
                    >> dataorigin_setion
                    >> SPACE
                    >> author_section
                    >> SPACE
                    >> RIGHT_PARAN
                    ;
    
            status_section
                =  LEFT_PARAN
                    >> SPACE
                    >> STATUS [PrintTag()]
                    >> SPACE 
                    >> written_section
                    >> RIGHT_PARAN
                    ;

            edif_name 
                =   *(anychar_p - space_p)
                    ;
            
            edif_section
                =   LEFT_PARAN
                    >> EDIF	[PrintTag()] 
					>> SPACE 
					>> edif_name [PrintStr()]
                    >> SPACE 
					>> edifversion_section
                    >> SPACE
                    >> ediflevel_section
                    >> SPACE
                    >> status_section
                    >> SPACE 
					>> RIGHT_PARAN
                    ;

            top =  edif_section 
                   >> end_p ;
        }

        strlit<> EDIF, EDIFVERSION, EDIFLEVEL, STATUS, WRITTEN, TIMESTAMP, PROGRAM, PROGVERSION,
                  DATAORIGIN, AUTHOR, INT, LOGIC_WIDTH, LOGIC_HEIGHT, FOR, LOCATION;

        rule<ScannerT>  top;

        rule<ScannerT> 
                space, edif_name, edif_section, edifversion_section,
                ediflevel_section, status_section, written_section, timestamp_section,
                program_section, programversion_section, dataorigin_setion, author_section;
 
        rule<ScannerT>
               any_string, string_val; 
        
        rule<ScannerT> SPACE, LEFT_BRACE, RIGHT_BRACE, LEFT_PARAN, RIGHT_PARAN;
        rule<ScannerT> const&
        start() const { return top; }
    };
};

bool
EDIFReader::Read(string fileName)
{
    edif_grammar g;
    ifstream in(fileName.c_str());
    if (!in)
        return false;
    in.unsetf(ios::skipws); //  Turn off white space skipping on the stream

    CharList vec;
    std::copy(istream_iterator<char>(in), istream_iterator<char>(), std::back_inserter(vec));

    ConstCharIter start = vec.begin();
    ConstCharIter end = vec.end();

    skip_grammar skip;
    
    parse_info<vector<char>::const_iterator> result = parse(start, end, g, skip);

    std::cout << std::endl << "[" << result.full << "]" << std::endl;
	
    if (result.full) {
        std::cout << "Parsing Done...." << std::endl;
        return true;
    }  else {
        for (int i = 0; i < 50; i++)
        {
            if (result.stop == end) {
                break;
            }
            std::cout << "Parsing error:" << *result.stop++;
        }
        //std::cout << endl;
        return false;
    }
}


