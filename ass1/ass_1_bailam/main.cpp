#include "VM.h"
//#include "VM.cpp"
#include "chrono"
using namespace std;


int size_file(string filename)
{
    ifstream ifs(filename);
    ifs.seekg(0,ios::end);
    int n = ifs.tellg();
    ifs.close();
    return n;
}
void test(string filename) {
    VM *vm = new VM();
    int n = size_file("C:\\Users\\Duong\\Desktop\\data_structure\\ass\\ass1\\ass_1_bailam\\bailamlai\\bin\\Debug\\bailamlai.exe");
    cout<<"Size of file :" <<n<<"bytes"<<endl;
    try {
        vm->run(filename);
    }
    catch (exception& e) {
        cout << e.what();
    }
    delete vm;
}

int main(int argc, char **argv)
{
    for(int i=0;i<46;i++){
    
    string a;
    a+= "C:\\Users\\Duong\\Desktop\\data_structure\\ass\\ass1\\ass_1_bailam\\testcase\\prog";
    a+= to_string(i+1);
    a+=".txt";
    cout<<"test"<<i+1<<":";
    auto start = chrono::steady_clock::now();
    //-----------------------------------------------------------------
    test(a);
    //-----------------------------------------------------------------
    cout << endl;
    auto end = chrono::steady_clock::now();
    double elasped_time = double(chrono::duration_cast<chrono::nanoseconds>(end - start).count());
    cout << "Elasped time of the program: " << elasped_time / 1e9 << endl;
    //-------------------------------------------------------------------
    cout << "\n";
    }
    //fileInput.close();
    return 0;
}
