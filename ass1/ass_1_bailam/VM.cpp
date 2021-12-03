#include "VM.h"

int VM::IP = 0;
bool VM::is_stop = false;
int VM::size_instr = 0;
bool VM::is_input = false;

int VM::read_file(string filename) {
	ifstream re_f;
	re_f.open(filename);
	int n = 0;
	if (re_f.fail()) return 0;
	while (!re_f.eof()) {
		char tmp[256];
		re_f.getline(tmp, 256, '\n');
		mem_bank[n] = tmp;
		this->check_2_big_error(mem_bank[n],n);
		n++;}
	re_f.close();
	return n;}
void data::take_data(string s,int n)
{   regex a("([0-9]+\\.+[0-9]+)"); // float
    regex b("\\d+$"); // int
    regex c("\\d+A$"); // address
    if (regex_match(s,c)) // Address
    { this->type = ADDRESS;
      s.pop_back();
      if(s.front() == '0' && s.size()!= 1)throw InvalidOperand(n);
      this->x = stoi(s);}
    else if (regex_match(s,a)) // Float
    {
     this->type = FLOAT;
     if(s[0] == '0' && s[1] != '.' && s.size()!= 1)throw InvalidOperand(n);
     this->y = stof(s);}
    else if(s == "true" || s == "false") // Bool
    {
        this->type = BOOL;
        if(s =="true") this->x = 1;
        else this->x = 0;}
    else if(regex_match(s,b)) // Int
    { if(s.front() == '0' && s.size()!= 1)throw InvalidOperand(n);
       this->x = stoi(s);
       this->type = INT;}
    else
    { if(VM::is_input)throw InvalidInput(n);
     else throw InvalidOperand(n);}
    if(VM::is_input) VM::is_input = false;
}
int VM::convert_regis(string i,int n)
{   if(i == "R1") return 1;
    else if(i == "R2")  return 2;
    else if(i == "R3")  return 3;
    else if(i == "R4")  return 4;
    else if(i == "R5")  return 5;
    else if(i == "R6")  return 6;
    else if(i == "R7")  return 7;
    else if(i == "R8")  return 8;
    else if(i == "R9")  return 9;
    else if(i == "R10") return 10;
    else if(i == "R11") return 11;
    else if(i == "R12") return 12;
    else if(i == "R13") return 13;
    else if(i == "R14") return 14;
    else if(i == "R15") return 15;
    else {throw InvalidOperand(n);}
}

void VM::check_error(string r,int n)
{
   regex a("\\s\\s");
   if(regex_search(r,a))throw InvalidInstruction(n);
}
void VM::check_error_1(string r,int n)
{   regex b ("\\s");
    if(regex_search(r,b))throw InvalidInstruction(n);
    if(r.front() == 'R') {convert_regis(r,n);}
    else this->register_CPU[0].take_data(r,n);
}
Operation_0_ins ::Operation_0_ins(string a1)
{
    this->z_0 = a1;
}
Operation_one_ins ::Operation_one_ins(string a1, string a2)
{
    this->o_0 = a1;
    this->o_1 = a2;
}
//2 ins
Operation_two_ins ::Operation_two_ins(string a1, string a2, string a3)
{
    this->t_0 = a1;
    this->t_1 = a2;
    this->t_2 = a3;
}

void Operation_0_ins::exe(VM& a)
{
  if(this->z_0 == "Halt"){VM::is_stop = true;}
  else if(this->z_0 == "Return")
       { VM::IP = a.stack_add.back();
         a.stack_add.pop_back();}
}
void VM::Output(int n)
{
    if(this->register_CPU[n].get_type() == INT)cout<<this->register_CPU[n].get_int();
    else if(this->register_CPU[n].get_type() == FLOAT)cout<<this->register_CPU[n].get_float();
    else if(this->register_CPU[n].get_type() == ADDRESS)cout<<this->register_CPU[n].get_int()<<"A";
    else if(this->register_CPU[n].get_type() == BOOL)
            {if(this->register_CPU[n].get_int() == 1)cout<<"true";
            else cout<<"false";}
}
void VM::Jump(int tmp)
{
 if(this->register_CPU[tmp].get_type() == ADDRESS)
    {int t = this->register_CPU[tmp].get_int()-1;
     if(t >= VM::size_instr-1)throw InvalidDestination(VM::IP);
     else VM::IP = t;}
  else throw TypeMismatch(VM::IP);
}
void VM::Call(int tmp)
{
  if(this->register_CPU[tmp].get_type() == ADDRESS)
     {int t = this->register_CPU[tmp].get_int()-1;
      if(t >= VM::size_instr-1) throw InvalidDestination(VM::IP);
      else   {if(this->stack_add.size()< 1000)
                {this->stack_add.push_back(VM::IP);
                 VM::IP = t;}
              else throw StackFull(VM::IP);}
     }
  else throw TypeMismatch(VM::IP);
}

void Operation_one_ins ::exe(VM& b)
{
    if (this->o_0 == "Input")
      { int n = b.convert_regis(this->o_1,VM::IP);
       string x;
       getline(cin,x);
       VM::is_input = true;
       b.register_CPU[n].take_data(x,VM::IP);}
    else if(this->o_0 == "Output")
        {   if(this->o_1.front() == 'R')
            {int n = b.convert_regis(this->o_1,VM::IP);
              b.Output(n);}
            else
            { b.register_CPU[0].take_data(this->o_1,VM::IP);
              b.Output(0);}
        }
    else if(this->o_0 == "Jump")
      {  if(this->o_1.front() == 'R')
         { int tmp = b.convert_regis(this->o_1,VM::IP);
            b.Jump(tmp);}
         else
         {  b.register_CPU[0].take_data(this->o_1,VM::IP);
            b.Jump(0);}
      }
    else if(this->o_0 == "Call")
        {  if(this->o_1.front() == 'R')
           {int tmp = b.convert_regis(this->o_1,VM::IP);
            b.Call(tmp);}
         else
           { b.register_CPU[0].take_data(this->o_1,VM::IP);
             b.Call(0);}
        }
    else if(this->o_0 == "Not")
        {  int n = b.convert_regis(this->o_1,VM::IP);
           b.register_CPU[n].set_address(VM::IP);
           if(b.register_CPU[n].get_type() == BOOL)
           {
               if(b.register_CPU[n].get_int() == 1) b.register_CPU[n].set_int(0);
                else b.register_CPU[n].set_int(1);
           }
           else throw TypeMismatch(VM::IP);
           b.register_CPU[n].set_type(BOOL);}
}
void VM::Load(int a_1,int a_2)
{
   if(this->register_CPU[a_2].get_type() != ADDRESS) throw TypeMismatch(VM::IP);
   else this->register_CPU[a_1] = this->sta_mem[this->register_CPU[a_2].get_int()];
}
void VM::Store(int a_1,int a_2)
{
  if(this->register_CPU[a_1].get_type() != ADDRESS) throw TypeMismatch(VM::IP);
   this->sta_mem[this->register_CPU[a_1].get_int()] = this->register_CPU[a_2];
}
void VM::JumpIf(int a_1,int a_2)
{
   if(this->register_CPU[a_1].get_type() == BOOL && this->register_CPU[a_2].get_type() == ADDRESS)
      { if(this->register_CPU[a_1].get_int() ==1)
              { int tmp = this->register_CPU[a_2].get_int() - 1;
                if(tmp >= VM::size_instr -1)throw InvalidDestination(VM::IP);
                else VM::IP = tmp;}
      }
   else throw TypeMismatch(VM::IP);
}
void Operation_two_ins ::exe(VM& c)
{   // Arithmetic
    if(this->t_0 == "Add")
          {int a_1 = c.convert_regis(this->t_1,VM::IP);
           c.register_CPU[a_1].set_address(VM::IP);
          int a_2 =0 ;
          if(this->t_2.front() == 'R') a_2 = c.convert_regis(this->t_2,VM::IP);
          else c.register_CPU[0].take_data(this->t_2,VM::IP);
          c.register_CPU[a_1] = c.register_CPU[a_1] + c.register_CPU[a_2];}
    else if(this->t_0 == "Minus")
          {int a_1 = c.convert_regis(this->t_1,VM::IP);
          c.register_CPU[a_1].set_address(VM::IP);
          int a_2 = 0;
          if(this->t_2.front() == 'R') a_2 = c.convert_regis(this->t_2,VM::IP);
          else c.register_CPU[0].take_data(this->t_2,VM::IP);
          c.register_CPU[a_1] = c.register_CPU[a_1] - c.register_CPU[a_2];}

     else if(this->t_0 == "Mul")
          {int a_1 = c.convert_regis(this->t_1,VM::IP);
          c.register_CPU[a_1].set_address(VM::IP);
          int a_2 = 0;
          if(this->t_2.front() == 'R') a_2 = c.convert_regis(this->t_2,VM::IP);
          else c.register_CPU[0].take_data(this->t_2,VM::IP);
          c.register_CPU[a_1] = c.register_CPU[a_1] * c.register_CPU[a_2];}
    else if(this->t_0 == "Div")
          {int a_1 = c.convert_regis(this->t_1,VM::IP);
          c.register_CPU[a_1].set_address(VM::IP);
          int a_2 = 0;
          if(this->t_2.front() == 'R') a_2 = c.convert_regis(this->t_2,VM::IP);
          else c.register_CPU[0].take_data(this->t_2,VM::IP);
          c.register_CPU[a_1] = c.register_CPU[a_1] / c.register_CPU[a_2];}
    // Read
    else if(this->t_0 == "Move")
         {
            if(this->t_2.front() == 'R')
            {
               int n_2 = c.convert_regis(this->t_2,VM::IP);
               int n_1 = c.convert_regis(this->t_1,VM::IP);
               c.register_CPU[n_1].set_int(c.register_CPU[n_2].get_int());
               c.register_CPU[n_1].set_float(c.register_CPU[n_2].get_float());
               c.register_CPU[n_1].set_type(c.register_CPU[n_2].get_type());
            }
            else
            {  int n =c.convert_regis(this->t_1,VM::IP);
               c.register_CPU[n].take_data(this->t_2,VM::IP);}
         }
    else if(this->t_0 == "Load")
       { int a_1 = c.convert_regis(this->t_1,VM::IP);
         c.register_CPU[a_1].set_address(VM::IP);
         int a_2 = 0;
         if(t_2.front() == 'R')a_2 = c.convert_regis(this->t_2,VM::IP);
         else c.register_CPU[0].take_data(this->t_2,VM::IP);
         c.Load(a_1,a_2);
       }
    else if(this->t_0 == "Store")
        { int a_1 = c.convert_regis(this->t_1,VM::IP);
          c.register_CPU[a_1].set_address(VM::IP);
          int a_2 = 0;
          if(t_2.front() == 'R')a_2 = c.convert_regis(this->t_2,VM::IP);
          else c.register_CPU[0].take_data(this->t_2,VM::IP);
          c.Store(a_1,a_2);
        }
     //Compare
    else if(this->t_0 =="CmpEQ")
        { int a_1 = c.convert_regis(this->t_1,VM::IP);
          c.register_CPU[a_1].set_address(VM::IP);
          int a_2 = 0;
          if(this->t_2.front() == 'R')a_2 = c.convert_regis(this->t_2,VM::IP);
          else c.register_CPU[0].take_data(this->t_2,VM::IP);
          c.register_CPU[a_1] == c.register_CPU[a_2];}
    else if(this->t_0 == "CmpNE")
        {int a_1 = c.convert_regis(this->t_1,VM::IP);
          c.register_CPU[a_1].set_address(VM::IP);
          int a_2 = 0;
          if(this->t_2.front() == 'R')a_2 = c.convert_regis(this->t_2,VM::IP);
          else c.register_CPU[0].take_data(this->t_2,VM::IP);
          c.register_CPU[a_1] != c.register_CPU[a_2];}
    else if(this->t_0 == "CmpLT")
        {int a_1 = c.convert_regis(this->t_1,VM::IP);
          c.register_CPU[a_1].set_address(VM::IP);
          int a_2 = 0;
          if(this->t_2.front() == 'R')a_2 = c.convert_regis(this->t_2,VM::IP);
          else c.register_CPU[0].take_data(this->t_2,VM::IP);
          c.register_CPU[a_1] < c.register_CPU[a_2];}
    else if(this->t_0 == "CmpLE")
        {int a_1 = c.convert_regis(this->t_1,VM::IP);
          c.register_CPU[a_1].set_address(VM::IP);
          int a_2 = 0;
          if(this->t_2.front() == 'R')a_2 = c.convert_regis(this->t_2,VM::IP);
          else c.register_CPU[0].take_data(this->t_2,VM::IP);
          c.register_CPU[a_1] <= c.register_CPU[a_2];}
    else if(this->t_0 == "CmpGT")
        {int a_1 = c.convert_regis(this->t_1,VM::IP);
          c.register_CPU[a_1].set_address(VM::IP);
          int a_2 = 0;
          if(this->t_2.front() == 'R')a_2 = c.convert_regis(this->t_2,VM::IP);
          else c.register_CPU[0].take_data(this->t_2,VM::IP);
          c.register_CPU[a_1] > c.register_CPU[a_2];}
    else if(this->t_0 == "CmpGE")
        {int a_1 = c.convert_regis(this->t_1,VM::IP);
          c.register_CPU[a_1].set_address(VM::IP);
          int a_2 = 0;
          if(this->t_2.front() == 'R')a_2 = c.convert_regis(this->t_2,VM::IP);
          else c.register_CPU[0].take_data(this->t_2,VM::IP);
          c.register_CPU[a_1] >= c.register_CPU[a_2];}
    // Boolean
    else if(this->t_0 == "And")
         {int a_1 = c.convert_regis(this->t_1,VM::IP);
          c.register_CPU[a_1].set_address(VM::IP);
          int a_2 = 0;
          if(this->t_2.front() == 'R')a_2 = c.convert_regis(this->t_2,VM::IP);
          else c.register_CPU[0].take_data(this->t_2,VM::IP);
          c.register_CPU[a_1] && c.register_CPU[a_2];}
    else if(this->t_0 == "Or")
         {int a_1 = c.convert_regis(this->t_1,VM::IP);
          c.register_CPU[a_1].set_address(VM::IP);
          int a_2 = 0;
          if(this->t_2.front() == 'R')a_2 = c.convert_regis(this->t_2,VM::IP);
          else c.register_CPU[0].take_data(this->t_2,VM::IP);
          c.register_CPU[a_1] || c.register_CPU[a_2];}
    else if(this->t_0 == "JumpIf")
          { int a_1 = c.convert_regis(this->t_1,VM::IP);
          c.register_CPU[a_1].set_address(VM::IP);
          int a_2 = 0;
          if(this->t_2.front() == 'R')a_2 = c.convert_regis(this->t_2,VM::IP);
          else c.register_CPU[0].take_data(this->t_2,VM::IP);
          c.JumpIf(a_1,a_2);}
}
// check file
void VM::check_2_big_error(string filename,int j)
{    check_error(mem_bank[j],j);
     string p = mem_bank[j];
     check_error(p,j);
     p = p.substr(0,p.find(" "));
     if (mem_bank[j] == "Return" || mem_bank[j] == "Halt")
       {cut_ins[j] = new string[1];
        cut_ins[j][0] = p;}
     else {
     string t = mem_bank[j];
     string p_3 = t.erase(0,t.find(" "));
     string p_2 = p_3.substr(p_3.find(" ")+1, p_3.back()- p_3.find(" ")-1);
     if (p == "Input" || p == "Output"||p == "Jump"||p == "Call"||(p == "Not" )) {
       check_error_1(p_2,j);
       cut_ins[j] = new string[2];
       cut_ins[j][0] = p;
       cut_ins[j][1] = p_2;}

     else if((p == "Add")||(p == "Minus")||(p == "Mul")||(p == "Div")||(p =="CmpEQ")||(p == "CmpNE")
     ||(p == "CmpLT")||(p == "CmpLE")||( p == "CmpGT")||(p == "CmpGE")
     || (p == "And") || (p == "Or")
     ||(p == "Move" )|| (p == "Load") || (p == "Store") || p == "JumpIf" )
    {
    if(p_2.find(",") > 300)throw InvalidInstruction(j);
    p_2 = p_2.substr(0,p_2.find(","));
    string temp_2 = mem_bank[j].substr(mem_bank[j].find(","), mem_bank[j].back()- mem_bank[j].find(","));
    temp_2 = temp_2.substr(temp_2.find(" ")+1,temp_2.back() - temp_2.find(" ")-1);
    check_error_1(p_2,j);
    check_error_1(temp_2,j);
    cut_ins[j] = new string[3];
    cut_ins[j][0] = p;
    cut_ins[j][1] = p_2;
    cut_ins[j][2] = temp_2;}
    else throw InvalidInstruction(j);}
}

void VM::run(string filename)
{   int k = read_file(filename);
    VM::size_instr = k;
    while(!this->is_stop){
    Operation *base;
     // operation with 0 operands
    if (cut_ins[IP][0] == "Return" || cut_ins[IP][0] == "Halt") base = new Operation_0_ins(cut_ins[IP][0]);
     // operation with 1 operands
    if (cut_ins[IP][0] == "Input" ||cut_ins[IP][0] == "Output"||cut_ins[IP][0] == "Jump"||cut_ins[IP][0] == "Call"
        ||cut_ins[IP][0] == "Not" )
      base = new Operation_one_ins(cut_ins[IP][0],cut_ins[IP][1]);
    // operation with 2 operands
    if((cut_ins[IP][0] == "Add")||(cut_ins[IP][0] == "Minus")||(cut_ins[IP][0] == "Mul")||(cut_ins[IP][0] == "Div")
    ||(cut_ins[IP][0] =="CmpEQ")||(cut_ins[IP][0] == "CmpNE")||(cut_ins[IP][0] == "CmpLT")
    ||(cut_ins[IP][0] == "CmpLE")||( cut_ins[IP][0] == "CmpGT")||(cut_ins[IP][0] == "CmpGE")
    || (cut_ins[IP][0] == "And") || (cut_ins[IP][0] == "Or")
    ||(cut_ins[IP][0]== "Move" )|| (cut_ins[IP][0] == "Load") || (cut_ins[IP][0] == "Store") || cut_ins[IP][0] == "JumpIf" )
      base = new Operation_two_ins(cut_ins[IP][0],cut_ins[IP][1],cut_ins[IP][2]);
  base->exe(*this);
  delete[]base;
  this->IP++;}
}
