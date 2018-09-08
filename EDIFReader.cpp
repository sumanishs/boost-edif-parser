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
        AUTHOR("author"), KEYWORDMAP("keywordMap"), KEYWORDLEVEL("keywordLevel"), 
        EXTERNAL("external"), TECHNOLOGY("technology"), NUMBERDEFINITION("numberDefinition"),
        CELL("cell"), CELLTYPE("cellType"), GENERIC("GENERIC"), TIE("TIE"), RIPPER("RIPPER")
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
                  >> *(SPACE >> int_p [PrintInteger()])
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
                    >> *(SPACE >> int_p [PrintInteger()])
                    >> SPACE
                    >> RIGHT_PARAN
                    ;
                  
            programversion_section
                = LEFT_PARAN
                    >> SPACE
                    >> PROGVERSION [PrintTag()]
                    >> SPACE
                    >> any_string
                    >> SPACE
                    >> RIGHT_PARAN
                    ;
 
            program_section
                = LEFT_PARAN
                    >> SPACE
                    >> PROGRAM [PrintTag()]
                    >> SPACE
                    >> any_string
                    >> SPACE
                    >> programversion_section
                    >> SPACE
                    >> RIGHT_PARAN
                    ;

            dataorigin_setion
                = LEFT_PARAN
                    >> SPACE
                    >> DATAORIGIN [PrintTag()]
                    >> SPACE
                    >> any_string
                    >> SPACE
                    >> RIGHT_PARAN
                    ; 
    
            author_section
                = LEFT_PARAN
                    >> SPACE
                    >> AUTHOR [PrintTag()]
                    >> SPACE
                    >> any_string
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

            keywordlevel_section
                = LEFT_PARAN
                    >> SPACE
                    >> KEYWORDLEVEL [PrintTag()]
                    >> SPACE
                    >> int_p [PrintInteger()]
                    >> SPACE
                    >> RIGHT_PARAN
                    ;

            keyword_section
                = LEFT_PARAN
                    >> SPACE
                    >> KEYWORDMAP [PrintTag()]
                    >> SPACE
                    >> keywordlevel_section
                    >> SPACE
                    >> RIGHT_PARAN
                    ;
    
            status_section
                =  LEFT_PARAN
                    >> SPACE
                    >> STATUS [PrintTag()]
                    >> SPACE 
                    >> written_section
                    >> SPACE
                    >> RIGHT_PARAN
                    ;

            section_name 
                =   *(anychar_p - space_p)
                    ;

            celltypesec_name
                    = GENERIC|TIE|RIPPER;

            alnum_name
                = *alnum_p
                    ;

            numberdefinition_section
                = LEFT_PARAN
                    >> SPACE
                    >> NUMBERDEFINITION [PrintTag()]
                    >> SPACE
                    >> RIGHT_PARAN
                    ;

            technolgy_section
                = LEFT_PARAN
                    >> SPACE
                    >> TECHNOLOGY [PrintTag()]
                    >> SPACE
                    >> numberdefinition_section
                    >> SPACE
                    >> RIGHT_PARAN
                    ;
    
            celltype_section
                = LEFT_PARAN
                    >> SPACE
                    >> CELLTYPE [PrintTag()]
                    >> SPACE
                    >> celltypesec_name [PrintStr()]
                    >> SPACE
                    >> RIGHT_PARAN
                    ;

            cell_section
                = LEFT_PARAN
                    >> SPACE
                    >> CELL [PrintTag()]
                    >> SPACE
                    >> section_name [PrintStr()]
                    >> SPACE
                    >> celltype_section
                    >> SPACE
                    >> RIGHT_PARAN
                    ;
            
            listof_cells
                =   cell_section  
                    ;          

            external_section
                = LEFT_PARAN
                    >> SPACE
                    >> EXTERNAL [PrintTag()]
                    >> SPACE
                    >> section_name [PrintStr()]
                    >> SPACE 
                    >> ediflevel_section
                    >> SPACE
                    >> technolgy_section
                    >> *(SPACE >> cell_section)
                    >> SPACE
                    >> RIGHT_PARAN
                    ; 
            
            edif_section
                =   LEFT_PARAN
                    >> EDIF	[PrintTag()] 
					>> SPACE 
					>> section_name [PrintStr()]
                    >> SPACE 
					>> edifversion_section
                    >> SPACE
                    >> ediflevel_section
                    >> SPACE
                    >> keyword_section
                    >> SPACE
                    >> status_section
                    >> SPACE
                    >> external_section
                    >> SPACE 
					>> RIGHT_PARAN
                    ;

            top =  edif_section 
                   >> end_p ;
        }

        strlit<> EDIF, EDIFVERSION, EDIFLEVEL, STATUS, WRITTEN, TIMESTAMP, PROGRAM, PROGVERSION,
                  DATAORIGIN, AUTHOR, KEYWORDMAP, KEYWORDLEVEL, EXTERNAL, TECHNOLOGY, NUMBERDEFINITION,
                  CELL, CELLTYPE, GENERIC, TIE, RIPPER;

        rule<ScannerT>  top;

        rule<ScannerT> 
                space, section_name, edif_section, edifversion_section,
                ediflevel_section, status_section, written_section, timestamp_section,
                program_section, programversion_section, dataorigin_setion, author_section, 
                keyword_section, keywordlevel_section, external_section, technolgy_section,
                numberdefinition_section;

        rule<ScannerT>
                cell_section, celltype_section, listof_cells, celltypesec_name;
 
        rule<ScannerT>
               any_string, string_val, alnum_name; 
        
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
    string to_parse(start, end);
    std::cout << "String to parse:" << std::endl << to_parse << std::endl;
    
    parse_info<vector<char>::const_iterator> result = parse(start, end, g, skip);

    std::cout << std::endl << "[" << result.full << "]" << std::endl;
	
    if (result.full) {
        std::cout << "Parsing Done...." << std::endl;
        return true;
    }  else {
        std::cout << "Parsing Error:";
        for (int i = 0; i < 50; i++)
        {
            if (result.stop == end) {
                break;
            }
            std::cout << *result.stop++;
        }
        //std::cout << endl;
        return false;
    }
}


