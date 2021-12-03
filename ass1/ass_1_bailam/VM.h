#ifndef VM_H
#define VM_H
#include "main.h"
enum{INT,FLOAT,BOOL,ADDRESS};
class data
{
  protected:
    int x;
    float y;
    int type;

  public:
    int address;
    void set_address(int line){this->address = line;}
    int get_type(){return this->type;}
    int get_int(){return x;}
    float get_float(){return y;}
    void set_type(int x){type = x;}
    void set_int(int i){this->x = i; this->type = INT;}
    void set_float(float z){this->y = z;this->type = FLOAT;}
    void take_data(string s,int);
    // Operator overloading
    // Equal
    data operator = (const data b)
    {
        this->type  = b.type;
        this->x = b.x;
        this->y = b.y;
        return *this;
    }
    // Plus
    data operator +(const data b)
    {  if (this->type != b.type)
       {if (this->type == INT && b.type == FLOAT)
           {float x_1 = this->x + b.y;
           set_float(x_1);}
      else  if(this->type == FLOAT && b.type == INT)
           {float x_1 = this->y + b.x;
            set_float(x_1);}
      else throw TypeMismatch(address);}

      else
      {if (type == INT) this->x += b.x;
      else if(type == FLOAT)this->y += b.y;
      else throw TypeMismatch(address);}
    return *this;}

    data operator -(const data b)
    {  if (this->type != b.type)
       {if (this->type == INT && b.type == FLOAT)
           {float x_1 = this->x - b.y;
           set_float(x_1);}
        else  if(this->type == FLOAT && b.type == INT)
           {float x_1 = this->y - b.x;
            set_float(x_1);}
        else throw TypeMismatch(address);}

      else
      {if (type == INT) this->x -= b.x;
      else if(type == FLOAT)this->y -= b.y;
      else throw TypeMismatch(address);}
    return *this;}
    // mul Res
    data operator *(const data b)
    {
        if (this->type != b.type)
       {if (this->type == INT && b.type == FLOAT)
           {float x_1 = this->x * b.y;
           set_float(x_1);}
      else  if(this->type == FLOAT && b.type == INT)
           {float x_1 = this->y * b.x;
            set_float(x_1);}
      else throw TypeMismatch(address);}

      else
      {if (type == INT) this->x *= b.x;
      else if(type == FLOAT)this->y *= b.y;
      else throw TypeMismatch(address);}
    return *this;}
    // div
    data operator / (const data b)
    {   if(b.x == 0 && b.type == INT) throw DivideByZero(address);
        if(b.y == 0 && b.type == FLOAT) throw DivideByZero(address);
        if (this->type != b.type)
       {if (this->type == INT && b.type == FLOAT)
           {float x_1 = this->x / b.y;
           set_float(x_1);}
      else  if(this->type == FLOAT && b.type == INT)
           {float x_1 = this->y / b.x;
            set_float(x_1);}
      else throw TypeMismatch(address);}

      else
      {if (type == INT) this->x /= b.x;
      else if(type == FLOAT)this->y /= b.y;
      else throw TypeMismatch(address);}
    return *this;}
    //Compare
    //equal Res
    data operator == (const data b)
    { if(this->type == ADDRESS || b.type == ADDRESS) throw TypeMismatch(address);
      if(this->type == b.type && this->type == BOOL)
        { if(this->x == b.x) this->x = 1;
          else this->x = 0;}
      else
        { if(this->type == INT)
          { if(b.type == FLOAT)
              {if(this->x == b.y) this->x = 1;
               else this->x = 0;}
            else if(b.type == INT)
              {if(this->x == b.x) this->x = 1;
               else this->x = 0;}
            else throw TypeMismatch(address);}
          else if(this->type == FLOAT)
          { if(b.type == FLOAT)
              {if(this->y == b.y) this->x = 1;
               else this->x = 0;}
            else if(b.type == INT)
              {if(this->y == b.x) this->x = 1;
               else this->x = 0;}
          else throw TypeMismatch(address);}
          else throw TypeMismatch(address);}
        this->type = BOOL;
        return *this;}
    // unequal Res
    data operator != (const data b)
    { if(this->type == ADDRESS || b.type == ADDRESS) throw TypeMismatch(address);
      if(this->type == b.type && this->type == BOOL)
        { if(this->x != b.x) this->x = 1;
          else this->x = 0;}
      else
        { if(this->type == INT)
          { if(b.type == FLOAT)
              {if(this->x != b.y) this->x = 1;
               else this->x = 0;}
            else if(b.type == INT)
              {if(this->x != b.x) this->x = 1;
               else this->x = 0;}
            else throw TypeMismatch(address);}
          else if(this->type == FLOAT)
          { if(b.type == FLOAT)
              {if(this->y != b.y) this->x = 1;
               else this->x = 0;}
            else if(b.type == INT)
              {if(this->y != b.x) this->x = 1;
               else this->x = 0;}
          else throw TypeMismatch(address);}
          else throw TypeMismatch(address);
        }
        this->type = BOOL;
        return *this;}

    // Less than
    data operator < (const data b)
    { if(this->type == ADDRESS || b.type == ADDRESS) throw TypeMismatch(address);
      if( b.type == BOOL || this->type == BOOL) throw TypeMismatch(address);
      else
        { if(this->type == INT)
          { if(b.type == FLOAT)
              {if(this->x < b.y) this->x = 1;
               else this->x = 0;}
            else if(b.type == INT)
              {if(this->x < b.x) this->x = 1;
               else this->x = 0;}
            else throw TypeMismatch(address);}
          else if(this->type == FLOAT)
          { if(b.type == FLOAT)
              {if(this->y < b.y) this->x = 1;
               else this->x = 0;}
            else if(b.type == INT)
              {if(this->y < b.x) this->x = 1;
               else this->x = 0;}
          else throw TypeMismatch(address);}
          else throw TypeMismatch(address);
        }
        this->type = BOOL;
        return *this;}
    // Less or equal res
    data operator <= (const data b)
    { if(this->type == ADDRESS || b.type == ADDRESS) throw TypeMismatch(address);
      if( b.type == BOOL || this->type == BOOL) throw TypeMismatch(address);
      else
        { if(this->type == INT)
          { if(b.type == FLOAT)
              {if(this->x <= b.y) this->x = 1;
               else this->x = 0;}
            else if(b.type == INT)
              {if(this->x <= b.x) this->x = 1;
               else this->x = 0;}
            else throw TypeMismatch(address);}
          else if(this->type == FLOAT)
          { if(b.type == FLOAT)
              {if(this->y <= b.y) this->x = 1;
               else this->x = 0;}
            else if(b.type == INT)
              {if(this->y <= b.x) this->x = 1;
               else this->x = 0;}
            else throw TypeMismatch(address);}
          else throw TypeMismatch(address);
        }
        this->type = BOOL;
        return *this;}
    // larger than res
    data operator > (const data b)
    { if(this->type == ADDRESS || b.type == ADDRESS) throw TypeMismatch(address);
       if( b.type == BOOL || this->type == BOOL) throw TypeMismatch(address);
      else
        { if(this->type == INT)
          { if(b.type == FLOAT)
              {if(this->x > b.y) this->x = 1;
               else this->x = 0;}
            else if(b.type == INT)
              {if(this->x > b.x) this->x = 1;
               else this->x = 0;}
            else throw TypeMismatch(address);}
          else if(this->type == FLOAT)
          { if(b.type == FLOAT)
              {if(this->y > b.y) this->x = 1;
               else this->x = 0;}
            else if(b.type == INT)
              {if(this->y > b.x) this->x = 1;
               else this->x = 0;}
            else throw TypeMismatch(address);}
        else throw TypeMismatch(address);}
        this->type = BOOL;
        return *this;}
    data operator >= (const data b)
    { if(this->type == ADDRESS || b.type == ADDRESS) throw TypeMismatch(address);
       if( b.type == BOOL || this->type == BOOL) throw TypeMismatch(address);
      else
        { if(this->type == INT)
          { if(b.type == FLOAT)
              {if(this->x >= b.y) this->x = 1;
               else this->x = 0;}
            else if(b.type == INT)
              {if(this->x >= b.x) this->x = 1;
               else this->x = 0;}
            else throw TypeMismatch(address);}
          else if(this->type == FLOAT)
          { if(b.type == FLOAT)
              {if(this->y >= b.y) this->x = 1;
               else this->x = 0;}
            else if(b.type == INT)
              {if(this->y >= b.x) this->x = 1;
               else this->x = 0;}
            else throw TypeMismatch(address);}
        else throw TypeMismatch(address);}
        this->type = BOOL;
        return *this;}
    // And gate res
    data operator && (const data b)
    {   if(this->type == b.type && this->type == BOOL)
        {   if(this->x == 1 && b.x == 1) this->x = 1;
            else this->x = 0;}
        else throw TypeMismatch(address);
        return *this;}
    // Or  gate res
    data operator || (const data b)
    {   if(this->type == b.type && this->type == BOOL)
        {   if(this->x == 0 && b.x == 0) this->x = 0;
            else this->x = 1;}
        else throw TypeMismatch(address);
     return *this;}
};
class VM
{
protected :
    string **cut_ins;
    string *mem_bank;
    data *sta_mem;
    data *register_CPU;
    vector<int>stack_add;
    static bool is_stop;

    friend class Operation_two_ins;
    friend class Operation_one_ins;
    friend class Operation_0_ins;
public:
  static bool is_input;
  static int size_instr;
  static int IP;
  // Constructor
  VM()
  {this->sta_mem = new data[65536];
   this->register_CPU = new data[16];
   this->mem_bank = new string[65536];
   this->cut_ins = new string *[65536];
  }
  ~VM()
  {   // Delete everything
      for(int i =0; i < size_instr;i++){delete[]cut_ins[i];}
      delete[]cut_ins;
      VM::IP = 0;
      VM::size_instr = 0;
      delete[]sta_mem;
      delete[]register_CPU;
      delete[]mem_bank;
      is_stop = false;
      is_input = false;
      this->stack_add.clear();}
  // 1 Operand
  void Output(int);
  void Jump(int);
  void Call(int);
  // 2 Operand
  void Load(int,int);
  void Store(int,int);
  void JumpIf(int,int);

  void check_2_big_error(string,int);
  void check_error(string,int); // check error ban đầu
  void check_error_1(string,int);
  int read_file(string filename);
  void run(string filename);
  int convert_regis(string,int);
};

class Operation
{
public:
    virtual void exe(VM&)= 0;
};
class Operation_two_ins: public Operation{
private:
    string t_0,t_1,t_2;
public :
   Operation_two_ins(string,string,string);
   virtual void exe(VM&) override;
};
class Operation_one_ins : public Operation
{
  private :
      string o_0,o_1;
  public :
      Operation_one_ins(string,string);
      virtual void exe(VM&) override;
};
class Operation_0_ins : public Operation
{   private :
       string z_0;
    public :
      Operation_0_ins(string);
      virtual void exe(VM&) override;
};
#endif
