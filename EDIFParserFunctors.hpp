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

using namespace std;

typedef std::vector<char>                   CharList;
typedef std::vector<char>::const_iterator   ConstCharIter;
typedef std::vector<char>::iterator         CharIter;


struct PrintInteger {
    void operator () (const int& arg) const 
    {
		cout << " " << arg ;
    }
    PrintInteger() {}
};  

struct  PrintStr {
    void operator () (ConstCharIter start, ConstCharIter end) const 
    {
    std::string s(start, end);
    cout << " " << s;
    }
    PrintStr() {}
};

struct  PrintStrLit {
    void operator () (ConstCharIter start, ConstCharIter end) const 
    {
    std::string s(start, end);
    cout << " " << "\"" << s << "\"";
    }
    PrintStrLit() {}
};

struct PrintTag {
    void operator () (ConstCharIter start, ConstCharIter end) const 
    {
    std::string s(start, end);
    cout << " " << s;
    }
    PrintTag() {}
};

struct PrintChar {
    void operator () (char c) const
    {
        cout << c;
    }
    PrintChar() {}

};

struct PrintNewLine {
    void operator () (ConstCharIter start, ConstCharIter end) const
    {
        cout << "\n";
    }
    PrintNewLine() {}

};
