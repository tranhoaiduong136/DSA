//change the address of 5 file above: 
//+result,errors, testcase,output,solution 
//Open report to see the result clearly
//Open errors to see the errors clearly
//The output file is just the temp file to check(no need to check that)

#include <stdio.h>
#include "main.h"
#include "Cache.h"
#include "Cache.cpp"

int h1(int k)
{
	return k + 1;
}
int h2(int k)
{
	return 2 * k + 1;
}
int h3(int k)
{
	return 3 * k;
}
int h4(int k)
{
	return 3 * k + 5;
}
Data *getData(string s)
{
	stringstream ss;
	ss << s;
	int idata;
	float fdata;
	if (ss >> idata)
		return new Int(idata);
	else if (ss >> fdata)
		return new Float(fdata);
	else if (s.compare("true") || s.compare("false"))
		return new Bool(s.compare("true"));
	else
	{
		s.resize(s.size() - 1);
		return new Address(stoi(s));
	}
	return NULL;
}
void simulate(string filename)
{
	ifstream ifs;
	ifs.open(filename, std::ifstream::in);
	string s;
	SearchEngine *sr;
	ReplacementPolicy *rp;
	Cache *c;
	while (getline(ifs, s))
	{
		stringstream ss(s);
		string code, tmp;
		ss >> code;
		int addr;

		switch (code[0])
		{
		case 'M': //MAXSIZE
			ss >> addr;
			MAXSIZE = addr;
			break;
		case 'S': // Search Engine
			ss >> tmp;
			if (tmp[0] == 'A')
				sr = new AVL();
			else
			{
				int (*hash1)(int);
				int (*hash2)(int);
				hash1 = tmp[1] == '1' ? h1 : tmp[1] == '2' ? h2
										 : tmp[1] == '3'   ? h3
														   : h4;
				hash2 = tmp[2] == '1' ? h1 : tmp[2] == '2' ? h2
										 : tmp[2] == '3'   ? h3
														   : h4;
				ss >> addr; //size
				sr = new DBHashing(hash1, hash2, addr);
			}
			break;
		case 'T': // ReplacementPolicy
			ss >> addr;
			if (addr == 1)
				rp = new LRU();
			else if (addr == 2)
				rp = new LFU();
			else if (addr == 3)
				rp = new FIFO();
			else
				rp = new MRU();
			c = new Cache(sr, rp);
			break;
		case 'R': //read
			ss >> addr;
			Data *res;
			res = c->read(addr);
			if (res == NULL)
			{
				ss >> tmp;
				c->put(addr, getData(tmp));
			}
			else
			{
				cout << res->getValue() << endl;
			}
			break;
		case 'U': //put
			ss >> addr;
			ss >> tmp;
			c->put(addr, getData(tmp));
			break;
		case 'W': //write
			ss >> addr;
			ss >> tmp;
			c->write(addr, getData(tmp));
			break;
		case 'P': // print
			cout << "Print replacement buffer\n";
			c->printRP();
			break;
		case 'E': //
			cout << "Print search buffer\n";
			c->printSE();
			break;
		}
	}
	delete c;
}
//Include more library
#include <fstream>
#include <iostream>
//#include <ostream>
#include <streambuf>
#include "chrono"
#include<string>
#include <regex>
#define WINDOWS 1

//clear screen
void console_clear_screen() {
  #ifdef WINDOWS
  system("cls");
  #endif
  #ifdef LINUX
  system("clear");
  #endif
}
// BUFFER WRITE THE CONSOLE OUTPUT TO FILE
class teebuf
    : public std::streambuf // 1 class in std
{
    streambuf* d_sbuf1;
    streambuf* d_sbuf2;
public:
    teebuf(std::streambuf* sbuf1, std::streambuf* sbuf2)
        : d_sbuf1(sbuf1), d_sbuf2(sbuf2){}
    int overflow(int c) 
	{
        if (c != std::char_traits<char>::eof()) 
		{
            this->d_sbuf1->sputc(c);
            this->d_sbuf2->sputc(c);
        }
        return char_traits<char>::not_eof(c);
    }
};
bool compare(string file_1,string file_2,ofstream &File3,int i)
{	
	// count line
	int count = 0;
	// file open
	bool output = true;
	//Compare 2 input file
	ifstream File1(file_1); // your ans
    ifstream File2(file_2); // my sol

	string line1,line2;
	
	// Cannot open the file
	// if cannot open throw error
	if(File1.fail()){ cerr<<"Error opening file !"<<endl;exit(1);}
	if(File2.fail()){ cerr<<"Error opening file !"<<endl;exit(1);}
	
	while((!File1.eof())&&(!File2.eof()))
	{	
		count++;
		getline(File1,line1);
		getline(File2,line2);

		if(line1 == "" && line2 =="")break;

		if(File1.eof())
		{	
			File3<<"Errors unmatched answer in line "<<count<<" in testcase "<<i<<endl;
			File3<<"*****************************"<<endl;
			output = false;
			break;	
		}
		if(line1 != line2)
		{ File3<<"Errors unmatched in line "<<count<<" in testcase "<<i<<endl;			
		  File3<<"Your file is: "<<line1<<endl;
		  File3<<"The ans is: "<<line2<<endl;
		  File3<<"*****************************"<<endl;
		  output = false;
		  break;
		}
		count++;	
	}
	File1.close();
	File2.close();
	return output;
}
void print(string file)
{
	ifstream ifs;
	ifs.open(file);
	string line;
	while(getline(ifs,line))
	{
		cout<<line<<endl;
	}
	ifs.close();
}

int main(int argc, char* argv[]) {
	//if (argc < 2) return 1;
//file report
string report ="C:\\Users\\Duong\\Desktop\\data_structure\\ass\\ass3\\report.txt";
ofstream ofs;
ofs.open(report);
// File errors
string errors = "C:\\Users\\Duong\\Desktop\\data_structure\\ass\\ass3\\errors.txt";
ofstream File3(errors);

//for(int i = 1;i <= 40;++i){
int i = 6;
ofs<<"Test case "<<i<<":\n";
// file testcase
string fileName = "C:\\Users\\Duong\\Desktop\\data_structure\\ass\\ass3\\initial\\tests\\test";
fileName += to_string(i);
fileName += ".txt";
// file solution
string output = "C:\\Users\\Duong\\Desktop\\data_structure\\ass\\ass3\\initial\\sols\\sol";
output += to_string(i);
output += ".txt";

//file temp output
string out1 = "C:\\Users\\Duong\\Desktop\\data_structure\\ass\\ass3\\output.txt";
ofstream out(out1);



teebuf   tb{out.rdbuf(), std::cout.rdbuf()};
streambuf* coutbuf = std::cout.rdbuf(&tb);
// The time:
auto start = chrono::steady_clock::now();
simulate(string(fileName));
auto end = chrono::steady_clock::now();
double elasped_time = double(chrono::duration_cast<chrono::nanoseconds>(end - start).count());
ofs << "Elasped time of the program: " << elasped_time / 1e9 << endl;
//Catch the output
cout.rdbuf(coutbuf);
// CLosed the buffer
out.close();

//Check with the file answer
if(compare(out1,output,File3,i) == true)ofs<<"TRUE"<<endl;
else ofs << "FALSE.PLEASE CHECK AGAIN"<<endl;

//Clear the screen
console_clear_screen();
//}
ofs.close();
File3.close();
//Cout to the console
console_clear_screen();
print(report);
return 0;
}



