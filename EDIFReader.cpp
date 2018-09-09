/**********************************************************************/
/* Author: Sumanish <sumanish.cse.jgec@gmail.com>                     */
/* Modified By: Sumanish <sumanish.cse.jgec@gmail.com>                */
/*                                                                    */  
/* This source code can be downloaded, use, modify, distribute        */
/* freely with this headed intact. Please don't delete this header.   */
/**********************************************************************/ 

#include "EDIFReader.hpp"
#include <boost/bind.hpp>
#include "EDIFParserFunctors.hpp"

//  Skip parser
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


//EDIF grammar
struct edif_grammar : public boost::spirit::grammar<edif_grammar>
{
    
    template <typename ScannerT>
    struct definition
    {

        definition(edif_grammar const& self ) : EDIF("edif"),
        EDIFVERSION("edifVersion"), EDIFLEVEL("edifLevel"), STATUS("status"), WRITTEN("written"),
        TIMESTAMP("timeStamp"), PROGRAM("program"), PROGVERSION1("Version"), PROGVERSION2("version"), DATAORIGIN("dataOrigin"),
        AUTHOR("author"), KEYWORDMAP("keywordMap"), KEYWORDLEVEL("keywordLevel"), 
        EXTERNAL("external"), TECHNOLOGY("technology"), NUMBERDEFINITION("numberDefinition"),
        CELL("cell"), CELLTYPE("cellType"), GENERIC("GENERIC"), TIE("TIE"), RIPPER("RIPPER"), VIEW("view"), 
        VIEWTYPE("viewType"), BEHAVIOR("BEHAVIOR"), DOCUMENT("DOCUMENT"), GRAPHIC("GRAPHIC"), LOGICMODEL("LOGICMODEL"), 
        MASKLAYOUT("MASKLAYOUT"), NETLIST("NETLIST"), PCBLAYOUT("PCBLAYOUT"), SCHEMATIC("SCHEMATIC"), 
        STRANGER("STRANGER"), SYMBOLIC("SYMBOLIC"), INTERFACE("interface"), PORT("port"), DIRECTION("direction"),
        INPUT("INPUT"), OUTPUT("OUTPUT"), INOUT("INOUT"), CONTENTS("contents"), INSTANCE("instance"), 
        VIEWREF("viewRef"), CELLREF("cellRef"), LIBRARYREF("libraryRef"),
        NET("net"), JOINED("joined"), MUSTJOIN("mustjoin"), CRITICALSIGNAL("criticalsignal"),
        PORTREF("portRef"), INSTANCEREF("instanceRef"), LIBRARY("library"), DESIGN("design") 
        {
            using namespace phoenix;
            LEFT_BRACE      =   ch_p('{') [PrintChar()];
            RIGHT_BRACE     =   ch_p('}') [PrintChar()];
            LEFT_PARAN      =   ch_p('(') [PrintChar()];
            RIGHT_PARAN     =   ch_p(')') [PrintChar()];
			SPACE		    = *space_p ; //[PrintChar()];
            any_string      = ch_p('"') 
                                >> string_val [PrintStrLit()]
                                >> ch_p('"')
                                ;

            string_val 
                = *(anychar_p - ch_p('"'));

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
                    >> (PROGVERSION1|PROGVERSION2) [PrintTag()]
                    >> SPACE
                    >> any_string [PrintStr()]
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
                    >> *(SPACE >> dataorigin_setion)
                    >> *(SPACE >> author_section)
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
                = *(anychar_p - space_p) ;

            string_without_right_paran
                = *(anychar_p - ch_p(')')) ;
            
            only_string
                = *(anychar_p - ch_p(')') - space_p) ;

            celltypesec_name
                = GENERIC|TIE|RIPPER;
 
            viewtype_name
                = BEHAVIOR|DOCUMENT|GRAPHIC|LOGICMODEL|MASKLAYOUT
                      |NETLIST|PCBLAYOUT|SCHEMATIC|STRANGER|SYMBOLIC;

            alnum_name
                = *alnum_p ;

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

            viewtype_section
                = LEFT_PARAN
                    >> SPACE
                    >> VIEWTYPE [PrintTag()]
                    >> SPACE
                    >> viewtype_name [PrintStr()]
                    >> SPACE
                    >> RIGHT_PARAN
                    ;
    
            direction_str
                = INPUT | OUTPUT | INOUT ;

            direction_section
                = LEFT_PARAN
                    >> SPACE
                    >> DIRECTION [PrintTag()]
                    >> SPACE
                    >> direction_str [PrintStr()]
                    >> SPACE
                    >> RIGHT_PARAN
                    ;

            port_section
                = LEFT_PARAN
                    >> SPACE
                    >> PORT [PrintTag()]
                    >> SPACE
                    >> section_name [PrintStr()]
                    >> SPACE
                    >> direction_section
                    >> SPACE
                    >> RIGHT_PARAN
                    ;
            
            interface_section
                = LEFT_PARAN
                    >> SPACE
                    >> INTERFACE [PrintTag()]
                    >> *(SPACE >> port_section)
                    >> SPACE
                    >> RIGHT_PARAN
                    ;

            libraryref_section
                = LEFT_PARAN
                    >> SPACE
                    >> LIBRARYREF [PrintTag()]
                    >> SPACE
                    >> string_without_right_paran [PrintStr()]
                    >> SPACE
                    >> RIGHT_PARAN
                    ;
                    
            cellref_section
                = LEFT_PARAN
                    >> SPACE
                    >> CELLREF [PrintTag()]
                    >> SPACE
                    >> section_name [PrintStr()]
                    >> SPACE
                    >> libraryref_section
                    >> SPACE
                    >> RIGHT_PARAN
                    ;

            viewref_section    
                = LEFT_PARAN
                    >> SPACE
                    >> VIEWREF [PrintTag()]
                    >> SPACE
                    >> section_name [PrintStr()]
                    >> SPACE
                    >> cellref_section
                    >> SPACE
                    >> RIGHT_PARAN
                    ;
                            
            instance_section
                = LEFT_PARAN
                    >> SPACE
                    >> INSTANCE [PrintTag()]
                    >> SPACE
                    >> section_name [PrintStr()]
                    >> SPACE
                    >> viewref_section
                    >> SPACE
                    >> RIGHT_PARAN
                    ;

            connection_type
                = JOINED | MUSTJOIN | CRITICALSIGNAL;

            instanceref_section
                = LEFT_PARAN
                    >> SPACE
                    >> INSTANCEREF [PrintTag()]
                    >> SPACE
                    >> only_string [PrintStr()]
                    >> SPACE
                    >> RIGHT_PARAN
                    ;
                
            connections
                = LEFT_PARAN
                    >> SPACE
                    >> PORTREF [PrintTag()]
                    >> SPACE
                    >> only_string [PrintStr()]
                    >> *(SPACE >> instanceref_section)
                    >> SPACE
                    >> RIGHT_PARAN
                    ;
    
            routing_section
                = LEFT_PARAN
                    >> SPACE
                    >> connection_type [PrintStr()]
                    >> *(SPACE >> connections)
                    >> SPACE
                    >> RIGHT_PARAN
                    ;

            net_section
                = LEFT_PARAN
                    >> SPACE
                    >> NET  [PrintTag()]
                    >> SPACE
                    >> section_name [PrintStr()]
                    >> SPACE
                    >> routing_section
                    >> SPACE
                    >> RIGHT_PARAN
                    ;
     
            contents_section
                = LEFT_PARAN
                    >> SPACE
                    >> CONTENTS [PrintTag()]
                    >> *(SPACE >> instance_section)
                    >> *(SPACE >> net_section)
                    >> SPACE
                    >> RIGHT_PARAN
                    ; 

            view_section
                = LEFT_PARAN
                    >> SPACE
                    >> VIEW [PrintTag()]
                    >> SPACE
                    >> section_name [PrintStr()]
                    >> SPACE
                    >> viewtype_section
                    >> SPACE
                    >> interface_section
                    >> *(SPACE >> contents_section)
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
                    >> view_section
                    >> SPACE
                    >> RIGHT_PARAN
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

            library_section
                = LEFT_PARAN
                    >> SPACE
                    >> LIBRARY [PrintTag()]
                    >> SPACE
                    >> only_string [PrintStr()]
                    >> SPACE
                    >> ediflevel_section
                    >> SPACE
                    >> technolgy_section
                    >> SPACE
                    >> cell_section
                    >> SPACE
                    >> RIGHT_PARAN
                    ;

            design_section
                = LEFT_PARAN
                    >> SPACE
                    >> DESIGN [PrintTag()]
                    >> SPACE
                    >> only_string [PrintStr()]
                    >> SPACE
                    >> cellref_section
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
                    >> *(SPACE >> external_section)
                    >> SPACE >> library_section
                    >> *(SPACE >> design_section)
                    >> SPACE 
					>> RIGHT_PARAN
                    ;

            top =  edif_section 
                   >> end_p ;
        }

        strlit<> EDIF, EDIFVERSION, EDIFLEVEL, STATUS, WRITTEN, TIMESTAMP, PROGRAM, PROGVERSION1, PROGVERSION2,
                  DATAORIGIN, AUTHOR, KEYWORDMAP, KEYWORDLEVEL, EXTERNAL, TECHNOLOGY, NUMBERDEFINITION,
                  CELL, CELLTYPE, GENERIC, TIE, RIPPER;
        
        strlit<> VIEW, VIEWTYPE, BEHAVIOR, DOCUMENT, GRAPHIC, LOGICMODEL, MASKLAYOUT,
                 NETLIST, PCBLAYOUT, SCHEMATIC, STRANGER, SYMBOLIC;
        
        strlit<> INTERFACE, PORT, DIRECTION, INPUT, OUTPUT, INOUT;

        strlit<> CONTENTS, INSTANCE, VIEWREF, CELLREF, LIBRARYREF,
                 NET, JOINED, MUSTJOIN, CRITICALSIGNAL, PORTREF, INSTANCEREF;

        strlit<> LIBRARY, DESIGN;

        rule<ScannerT>  top;

        rule<ScannerT> 
                space, section_name, edif_section, edifversion_section,
                ediflevel_section, status_section, written_section, timestamp_section,
                program_section, programversion_section, dataorigin_setion, author_section, 
                keyword_section, keywordlevel_section, external_section, technolgy_section,
                numberdefinition_section;

        rule<ScannerT>
                cell_section, celltype_section, celltypesec_name, view_section, viewtype_section,
                viewtype_name, interface_section, port_section, direction_section, direction_str,
                contents_section, instance_section, viewref_section, cellref_section;

        rule<ScannerT>
                libraryref_section, net_section, routing_section, connections,
                connection_type, module_port_ref, instance_con, module_port, instanceref_section,
                library_section, design_section;
 
        rule<ScannerT>
               any_string, string_val, alnum_name, string_without_right_paran,
               only_string ; 
        
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
    in.unsetf(ios::skipws); //Switch off white space skipping on the stream

    CharList vec;
    std::copy(istream_iterator<char>(in), istream_iterator<char>(), std::back_inserter(vec));

    ConstCharIter start = vec.begin();
    ConstCharIter end = vec.end();

    skip_grammar skip;
    string to_parse(start, end);
    //std::cout << "String to parse:" << std::endl << to_parse << std::endl;
    
    parse_info<vector<char>::const_iterator> result = parse(start, end, g, skip);

    std::cout << std::endl << "[" << result.full << "]" << std::endl;
	
    if (result.full) {
        std::cout << "Parsing Done...." << std::endl;
        return true;
    }  else {
        std::cout << "Parsing Error:";
        //for (int i = 0; i < 50; i++)
        while (1)
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
