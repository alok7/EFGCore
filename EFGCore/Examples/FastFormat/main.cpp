#include <iostream>
#include <Core/CommonUtils/Utils.h>

using namespace EFG::Core::Utils;

int main()
{

   const char* strDouble = "7.23"; 
   double valueDouble = toDouble(strDouble); 
   printf("%f\n", valueDouble); 
   
   const char* strInt = "723"; 
   int valueInt = toInteger(strInt); 
   printf("%d\n", valueInt); 
   
   double DoubleValue = 7.23; 
   Format<double> format; // default precision 9 
   format.convert(DoubleValue);
   printf("%s\n", format.toString()); 
  
   int IntValue = 723; 
   Format<int> format_; 
   format_.convert(IntValue); 
   printf("%s\n", format_.toString()); 

   return 0;
}
