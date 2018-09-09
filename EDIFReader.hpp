/**********************************************************************/
/* Author: Sumanish <sumanish.cse.jgec@gmail.com>                     */
/* Modified By: Sumanish <sumanish.cse.jgec@gmail.com>                */
/*                                                                    */  
/* This source code can be downloaded, use, modify, distribute        */
/* freely with this headed intact. Please don't delete this header.   */
/**********************************************************************/ 

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <boost/spirit.hpp>
#include <boost/spirit/home/classic/phoenix/binders.hpp>
#include "boost/lambda/bind.hpp"


using namespace boost::spirit;
using namespace std;

typedef std::vector<char>                   CharList;
typedef std::vector<char>::const_iterator   ConstCharIter;
typedef std::vector<char>::iterator         CharIter;

class EDIFReader {

public:
    /// default constructor
    EDIFReader() { }
    bool Read(string filename);

private:
    EDIFReader(const EDIFReader&);
    EDIFReader& operator=(const EDIFReader&);
};

