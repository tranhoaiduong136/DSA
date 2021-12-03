#include <stdio.h>
#include "main.h"
#include "Cache.h"
#include "chrono"
#include <string>
#include "Cache.cpp"

Data* getData(string s) {
	stringstream ss(s);
	int idata;
    float fdata;
    if (ss >> idata)
		return new Int(idata);
    else if (ss >> fdata)
    	return new Float(fdata);
    else if (s.compare("true") || s.compare("false"))
    	return new Bool(s.compare("true"));
    else {
		s.resize(s.size()-1);
		return new Address(stoi(s));
	}

}
void simulate(string filename,Cache* c)
{
  ifstream ifs;
  ifs.open(filename, std::ifstream::in);
  string s;
  while (getline(ifs, s))
  {
    stringstream ss(s);
    string code,tmp;
    ss >> code;
    int addr;
    switch (code[0]) {
    	case 'R': //read
			    ss >> addr;
    			Data* res;
				res = c->read(addr);
    			if (res == NULL) {
    				ss >> tmp;
    				c->put(addr,getData(tmp));
    			} else {
					cout << res->getValue() << endl;
				}
    			break;
        case 'U': //put
                ss >> addr;
                ss >> tmp;
                c->put(addr,getData(tmp));
                break;
    	case 'W': //write
                ss >> addr;
    			ss >> tmp;
    			c->write(addr,getData(tmp));
    			break;
    	case 'P': // print
    			cout << "Print queue\n";
    			c->print();
    			break;
    	case 'E': // preorder
    			cout << "Print AVL in preorder\n";
    			c->preOrder();
    			break;
    	case 'I': // inorder
    			cout << "Print AVL in inorder\n";
    			c->inOrder();
    			break;
    }
  }
}

int main(int argc, char* argv[]) {
	//if (argc < 2)return 1;
	for(int i=0;i<24;i++){
    Cache* c = new Cache(MAXSIZE);
    //string fileName = "C:\\Users\\Duong\\Desktop\\data_structure\\ass\\ass2\\tests\\test";
  	//string fileName ="C:\\Users\\Duong\\Desktop\\data_structure\\ass\\ass2\\tests_insert\\insert\\test";
  	string fileName = "C:\\Users\\Duong\\Desktop\\data_structure\\ass\\ass2\\testcase_delete\\delete\\test";
  	fileName += to_string(i+1);
  	fileName += ".txt";
  	cout<<"---------------test "<<i+1<<"----------"<<endl;
    auto start = chrono::steady_clock::now();
    simulate(string(fileName),c);
    auto end = chrono::steady_clock::now();
    double elasped_time = double(chrono::duration_cast<chrono::nanoseconds>(end - start).count());
    cout << "Elasped time of the program: " << elasped_time / 1e9 << endl;
  	delete c;
  	cout<<"\n";
  	}
	return 0;
}
