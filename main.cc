//************************************************************************************
// CPSC 3730
// Programming Assignment: Triple DES Encryption
// main.cc
// Written By: Eric Den Haan
//************************************************************************************

#include <iostream>
#include "./des.h"

using namespace std;

int main()
{
   //Determine whether encryption or decryption will be run
   int mode;
   while(mode != 1 && mode != 2)
   {
      cout << "Enter 1 to Encrypt a file, 2 to Decrypt a file: ";
      cin >> mode;
   }

   //Ask the user for the input file location
   string inFile, outFile;
   cout << "Please enter the input file location and name: ";
   cin >> inFile;
   cout << endl;

   //Triple DES
   if(mode == 1)
   {
      outFile = inFile + "_encrypted.txt";
      DES D1(3, 1, 1, inFile);
      D1.fileRead();
      D1.encrypt();
      DES D2(4, 2, 2, D1.getOutput());
      D2.decrypt();
      DES D3(2, 3, 1, D2.getOutput(), outFile);
      D3.encrypt();
      cout << "File Encrypted. Output file in same folder as input. Here is the output:" << endl;
      D3.fileWrite();
      D3.printOutput();
   }
   else if(mode == 2)
   {
      outFile = inFile + "_decrypted.txt";
      DES D1(3, 3, 2, inFile);
      D1.fileRead();
      D1.decrypt();
      DES D2(4, 2, 1, D1.getOutput());
      D2.encrypt();
      DES D3(2, 1, 2, D2.getOutput(), outFile);
      D3.decrypt();
      cout << "File Decrypted. Output file in same folder as input. Here is the output:" << endl;
      D3.fileWrite();
      D3.printOutput();
   }

   return 0;
}
