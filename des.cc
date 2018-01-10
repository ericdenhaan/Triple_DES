//************************************************************************************
// CPSC 3730
// Programming Assignment: Triple DES Encryption
// DES Implementation File
// Written By: Eric Den Haan
//************************************************************************************

#include "./des.h"

using namespace std;

//************************************************************************************
// Constructors
//************************************************************************************

DES::DES
	(const int& t, const int& i, const int& m, const string& in, const string& out)
{
	ID = i;
	mode = m;
	
	//For single DES
	if(t == 1)
	{
		inFile = in;
		outFile = out;
	}
	//Final step of triple DES
	else if(t == 2)
	{
		inputString = in;
		outFile = out;
	}
}

DES::DES(const int& t, const int& i, const int& m, const string& in)
{
	ID = i;
	mode = m;
	
	//Initial step of triple DES
	if(t == 3)
	{
		inFile = in;
	}
	//Second step of triple DES
	else if(t == 4)
	{
		inputString = in;
	}
}

//***********************************************************************************
// encrypt Function
//***********************************************************************************

void DES::encrypt()
{
	//Carry out the DES encryption algorithm
	initKey();
	input();
	for(unsigned int i = 0; i < inputBlocks.size(); i++)
	{
		currBlock = i;
		initialPermutation();
		for(int j = 0; j < 16; j++)
		{
			roundNumber = j;
			expansionPermutation();
			permutedChoice1();
			subKeyGen();
			eRI ^= keyI;
			sBoxes();
			permutationFunction();
			rI ^= lPrev;
			lI = rPrev;
		}
		inverseInitialPermutation();
	}
	output();
}

//***********************************************************************************
// decrypt Function
//***********************************************************************************

void DES::decrypt()
{
	//Carry out the DES decryption algorithm
	initKey();
	input();
	for(unsigned int i = 0; i < inputBlocks.size(); i++)
	{
		currBlock = i;
		initialPermutation();
		for(int i = 0; i < 16; i++)
		{
			roundNumber = i;
			expansionPermutation();
			permutedChoice1();
			subKeyGen();
			eRI ^= keyI;
			sBoxes();
			permutationFunction();
			rI ^= lPrev;
			lI = rPrev;
		}
		inverseInitialPermutation();
	}	
	output();
}

//************************************************************************************
// initKey Function
//************************************************************************************

void DES::initKey()
{
	//Ask the user to input a key and verify the values
	bool valid = false;
	string binString;
	while(valid != true)
	{
		cout << "Please enter Key #" << ID << ": ";
   		cin >> keyHex;
   		binString = "";
		if(keyHex.length() == 16)
		{
			for(unsigned int i = 0; i < keyHex.length(); i++)
			{
				switch(keyHex[i])
				{
					case '0' : 
						binString += "0000";
						break;
					case '1' : 
						binString += "0001";
						break;
					case '2' : 
						binString += "0010";
						break;
					case '3' : 
						binString += "0011";
						break;
					case '4' : 
						binString += "0100";
						break;
					case '5' : 
						binString += "0101";
						break;
					case '6' : 
						binString += "0110";
						break;
					case '7' : 
						binString += "0111";
						break;
					case '8' : 
						binString += "1000";
						break;
					case '9' : 
						binString += "1001";
						break;
					case 'A' : 
						binString += "1010";
						break;
					case 'B' : 
						binString += "1011";
						break;
					case 'C' : 
						binString += "1100";
						break;
					case 'D' : 
						binString += "1101";
						break;
					case 'E' : 
						binString += "1110";
						break;
					case 'F' : 
						binString += "1111";
						break;
					case 'a' : 
						binString += "1010";
						break;
					case 'b' : 	
						binString += "1011";
						break;
					case 'c' : 
						binString += "1100";
						break;
					case 'd' : 
						binString += "1101";
						break;
					case 'e' : 
						binString += "1110";
						break;
					case 'f' : 
						binString += "1111";
						break;
					default : 
						cout << "Invalid key - ";
						cout << "please enter a 16 character hex string" << endl;
						break;			
				}
			}
			if(binString.length() == 64)
			{
				valid = true;
			}
		}
		else
		{
			cout << "Invalid key - please enter a 16 character hex string" << endl;
		}
	}

	//Create a bitset of size 64 for the key
	keyB = bitset<64>(binString);
}

//************************************************************************************
// fileRead Function
//************************************************************************************

void DES::fileRead()
{
	//Read the file contents into a string
	ifstream f(inFile);
	string s((istreambuf_iterator<char>(f)), istreambuf_iterator<char>());
	inputString = s;
}

//************************************************************************************
// input Function
//************************************************************************************

void DES::input()
{
	//Pad the input string with the correct number of chars to make 64 bit blocks
	int pad = inputString.size() % 8;
	if(pad != 0)
	{
		for(int i = 0; i < 8-pad; i++)
		{
			inputString = inputString + " ";
		}
	}

	//Convert the chars in the string to bitsets - used for processing
	textB.resize(inputString.size());
	for(unsigned int i = 0; i < inputString.size(); i++)
	{
		textB[i] = bitset<8>(inputString[i]);
	}

	//Group textB into bitsets of size 64, populate inputBlocks
	string binString;
	for(unsigned int i = 0; i < textB.size(); i++)
	{
		binString += textB[i].to_string();		
	}
	string temp;
	for(unsigned int i = 0; i < binString.size(); i += 64)
	{
		for(int j = 0; j < 64; j++)
		{
			temp += binString[i+j];
		}
		inputBlocks.push_back(bitset<64>(temp));
		temp = "";
	}
}

//***********************************************************************************
// output Function
//***********************************************************************************

//Convert the bitsets back to chars
void DES::output()
{
	string binString;
	
	//Convert outputBlocks to a string of binary chars
	for(unsigned int i = 0; i < outputBlocks.size(); i++)
	{
		binString += outputBlocks[i].to_string();
	}

	//Convert the binary chars to ASCII
	for(unsigned int i = 0; i < binString.size() / 8; i++)
	{
		string temp = binString.substr(i * 8, 8);
		bitset<8> b(temp);
		outputString += b.to_ulong();
	}
}

//***********************************************************************************
// getOutput Function
//***********************************************************************************

//Return the outputString (to be used by another DES instance as inputString)
string DES::getOutput()
{
	return outputString;
}

//***********************************************************************************
// printOutput() Function
//***********************************************************************************

//Write to file with specified name
void DES::printOutput()
{
	cout << endl << outputString << endl;
}

//***********************************************************************************
// fileWrite Function
//***********************************************************************************

//Write to file with specified name
void DES::fileWrite()
{
	ofstream f;
	f.open(outFile);
	f << outputString;
	f.close();
}

//***********************************************************************************
// initialPermutation Function
//***********************************************************************************

void DES::initialPermutation()
{
	bitset<64> initial = inputBlocks[currBlock];
	bitset<64> permuted;

	//Note: indices for bitsets are reversed from standard array indices
	for(unsigned int i = 0; i < initialPermutationRef.size(); i++)
	{
		int pos = initialPermutationRef[i]-1;
		permuted[63-i] = initial[63-pos];
	}
	//Create l0 and r0
	for(int i = 32; i < 64; i++)
	{
		l0[i-32] = permuted[i];
	}
	for(int i = 0; i < 32; i++)
	{
		r0[i] = permuted[i];
	}

	lI = lPrev = l0;
	rI = rPrev = r0;
}

//***********************************************************************************
// expansionPermutation Function
//***********************************************************************************

void DES::expansionPermutation()
{
	bitset<48> permuted;

	//Follow similar logic as above
	for(unsigned int i = 0; i < expansionPermutationRef.size(); i++)
	{
		int pos = expansionPermutationRef[i]-1;
		permuted[47-i] = rPrev[31-pos];
	}
	eRI = permuted;
}

//***********************************************************************************
// subKeyGen Function
//***********************************************************************************

void DES::subKeyGen()
{
	//Circular left shifts (reversed for decryption)
	if(mode == 1)
	{
		rotateLeft(cI, shiftScheduleRef[roundNumber]);
		rotateLeft(dI, shiftScheduleRef[roundNumber]);
	}
	else if(mode == 2)
	{
		rotateLeft(cI, shiftScheduleRef[15-roundNumber]);
		rotateLeft(dI, shiftScheduleRef[15-roundNumber]);
	}

	cPrev = cI;
	dPrev = dI;

	//Create keyI
	bitset<56> shifted;
	for(int i = 28; i < 56; i++)
	{
		shifted[i] = cI[i-28];
	}
	for(int i = 0; i < 28; i++)
	{
		shifted[i] = dI[i];
	}
	for(unsigned int i = 0; i < permutedChoice2Ref.size(); i++)
	{
		int pos = permutedChoice2Ref[i]-1;
		keyI[47-i] = shifted[55-pos];
	}
}

//***********************************************************************************
// permutedChoice1 Function
//***********************************************************************************

void DES::permutedChoice1()
{
	bitset<56> permuted;

	//Follow similar logic as above to permute the key
	for(unsigned int i = 0; i < permutedChoice1Ref.size(); i++)
	{
		int pos = permutedChoice1Ref[i]-1;
		permuted[55-i] = keyB[63-pos];
	}
	blockKey = permuted;

	for(int i = 28; i < 56; i++)
	{
		c0[i-28] = blockKey[i];
	}
	for(int i = 0; i < 28; i++)
	{
		d0[i] = blockKey[i];
	}

	cI = cPrev = c0;
	dI = dPrev = d0;
}

//***********************************************************************************
// sBoxes Function
//***********************************************************************************

void DES::sBoxes()
{
	//Reference the S-Boxes and use them for further permutation
	vector<bitset<6>> temp;

	for(int i = 0; i < 48; i += 6)
	{
		bitset<6> subSet;
		for(int j = 0; j < 6; j++)
		{
			subSet[5-j] = eRI[47-(i+j)];
		}
		temp.push_back(subSet);
	}
	for(unsigned int i = 0; i < temp.size(); i++)
	{
		bitset<6> ref = temp[i];
		bitset<2> row;
		bitset<4> col;
		row[1] = ref[5];
		row[0] = ref[0];
		col[3] = ref[4];
		col[2] = ref[3];
		col[1] = ref[2];
		col[0] = ref[1];
		int rowInt = row.to_ulong();
		int colInt = col.to_ulong();
		vector<int> table = sBoxRef[i];
		bitset<4> component = table[(rowInt * 16) + colInt];
		for(int j = 0; j < 4; j++)
		{
			rI[31-(i*4)-j] = component[3-j];
		}
	}
}

//***********************************************************************************
// permutationFunction Function
//***********************************************************************************

void DES::permutationFunction()
{
	bitset<32> initial = rI;
	bitset<32> permuted;

	//Following similar logic as above
	for(unsigned int i = 0; i < permutationFunctionRef.size(); i++)
	{
		int pos = permutationFunctionRef[i]-1;
		permuted[31-i] = initial[31-pos];
	}
	for(int i = 0; i < 32; i++)
	{
		rI[i] = permuted[i];
	}
}

//***********************************************************************************
// inverseInitialPermutation Function
//***********************************************************************************

void DES::inverseInitialPermutation()
{
	bitset<32> temp;
	temp = lI;
	lI = rI;
	rI = temp;

	//Following similar logic as above
	bitset<64> initial;
	bitset<64> permuted;
	for(int i = 32; i < 64; i++)
	{
		initial[i] = lI[i-32] ;
	}
	for(int i = 0; i < 32; i++)
	{
		initial[i] = rI[i];
	}
	for(unsigned int i = 0; i < inverseInitialPermutationRef.size(); i++)
	{
		int pos = inverseInitialPermutationRef[i]-1;
		permuted[63-i] = initial[63-pos];
	}

	outputBlocks.push_back(permuted);
}

//***********************************************************************************
// rotate Function
//***********************************************************************************

//Rotate a given bitset left or right by a specified number of bits
void DES::rotateLeft
	(bitset<28>& subKey, const int quantity)
{
	subKey = subKey << quantity | subKey >> (28-quantity);
}