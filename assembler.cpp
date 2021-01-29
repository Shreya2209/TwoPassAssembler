/*
Name : SHREYA SINHA
Roll No. 1801CS47

Date : 20/11/2020
=======================================================================
TITLE: Assembler Code
AUTHOR:  Shreya Sinha - 1801CS47
Declaration of Authorship - This cpp file, asm.cpp is part of the assignment of CS321 at the
department of Computer Science and Engg, IIT Patna .

*/

#include <bits/stdc++.h>
#include <string>
#include <fstream>
using namespace std;


class listing{
public:
	string loct;
	string machine;
	string ginput;
};

class line{
public:
	string label;
	string instr;
	string sec_val;
	int category;
};

vector<line> code;
vector<listing> listfil;
map<string,string>mp;
map<string, int> ins_type;
vector<string> inp, machineCode;
map<string, int>labels;
vector<pair<int,string>>error_display;
vector<int>PC;

void pass1();
void pass2();
void write();
int error_msg();
void runAssembler();


int main()
{
    cout<<"Enter name of test file to load input from:\n";
    runAssembler();
    return 0;
}


void map_instructions()
{
    mp["data"] = "";
    mp["ldc"] = "00";
    mp["adc"] = "01";
    mp["ldl"] = "02";
    mp["stl"] = "03";
    mp["ldnl"] ="04";
    mp["stnl"] ="05";
    mp["add"] = "06";
    mp["sub"] = "07";
    mp["shl"] = "08";
    mp["shr"] = "09";
    mp["adj"] = "0A";
    mp["a2sp"] = "0B";
    mp["sp2a"] = "0C";
    mp["call"] = "0D";
    mp["return"] = "0E";
    mp["brz"] = "0F";
    mp["brlz"] = "10";
    mp["br"] = "11";
    mp["HALT"] = "12";
    mp["SET"] = "13";
    return;
}
void map_inst_type()
{
    ins_type["data"] = 1;
    ins_type["ldc"] = 1;
    ins_type["adc"] = 1;
    ins_type["ldl"] = 2;
    ins_type["stl"] = 2;
    ins_type["ldnl"] = 2;
    ins_type["stnl"] = 2;
    ins_type["add"] = 0;
    ins_type["sub"] = 0;
    ins_type["shl"] = 0;
    ins_type["shr"] = 0;
    ins_type["adj"] = 1;
    ins_type["a2sp"] =0;
    ins_type["sp2a"] = 0;
    ins_type["call"] = 2;
    ins_type["return"] = 0;
    ins_type["brz"] = 2;
    ins_type["brlz"] = 2;
    ins_type["br"] = 2;
    ins_type["HALT"] = 0;
    ins_type["SET"] = 1;
    return;
}
void reverseStr(string& str)
{
    int n = str.length();
    for (int i = 0; i < n / 2; i++)
        swap(str[i], str[n - i - 1]);
}

int checkIfDecimal(string);
int checkIfHexadecimal(string s);
int checkIfOctal(string s);

int checkbase(string s, int n)
{
    if(n==10)
    return checkIfDecimal(s);
    if(n==16)
    return checkIfHexadecimal(s);
    if(n==8)
    return checkIfOctal(s);
	return 0;
}

int checkIfDecimal(string s){

	for(int i = 0; i < (int)s.size(); i++)
		if(!(s[i] >= '0' and s[i] <= '9'))
        return 0;

	return 1;
}

int checkIfHexadecimal(string s){

    int ok = 1 ;
    if((int)s.size() < 3)
    	return 0;
    ok &= (s[0] == '0') & ((s[1] == 'x' or s[1] == 'X'));

    for(int i = 2; i < (int) s.size(); i++)
    {
        bool state = (s[i] >= '0' and s[i] <= '9');
        state |= ((s[i] >= 'a' and s[i] <= 'f') or ((s[i] >= 'A' and s[i] <= 'F')));
        ok = (ok & state);
    }
    return ok;

}

int checkIfOctal(string s)
{
    int n = s.size();

    if(n < 2)
    	return 0;

    int ok = 1, i=1;

    while(i < n)
    {
        if(!(s[i] >= '0' and s[i] <= '7'))
        return 0;

        i++;
    }
    if(ok && (s[0] == '0'))
    return 1;

    return 0;
}

bool check_name(string s, int i)
{
    if(!((s[0]>='a' && s[0]<='z')||(s[0]>='A' && s[0]<='Z')||(s[0]>=33 && s[0]<=127)))return 0;

    for(int i=1;i<s.size();i++)
    {
       if(!((s[0]>='a' && s[0]<='z')||(s[0]>='A' && s[0]<='Z')||(s[0]>=33 && s[0]<=127)))return 0;
    }
    return 1;
}

string ConvertdecToHex(int n){
	if(n < 0){
		n = n + (int)pow(2, 24);
	}
    stringstream ss;

    ss << hex << n;

    return ss.str();
}
string ConvertdecToHexdata(unsigned int n){
    stringstream ss;

    ss << hex << n;

    return ss.str();
}
int type(string s){

	if(s.empty())
    return 0;
        if(s[0] == '+' || s[0] == '-' && s.size() > 1)
        {

		    s = s.substr(1, s.size() - 1);
	    }
	if(s.empty())
		return -1;
	else if(checkbase(s, 10))
     return 10;
	else if(checkbase(s,8))
     return 8;
	else if(checkbase(s,16))
     return 16;
	else if(check_name(s,0))
     return 1;

	return -1;
    }

string clear( int line, string text)
{
    int i=0;
    int flag=0;
    string temp="";


		reverseStr(text);
		while(text.back() == ' ' || text.back() == '\t')
        {
            text.pop_back();
        }
        reverseStr(text);
		while(text.back() == ' ' || text.back() == '\t')
        {
            text.pop_back();
        }

    i=0;

    while(i<(int)text.size() && flag!=1)
    {
        if(text[i]==';')
        {
            flag=1;
        }
        else if(text[i]==':')
        {
            temp +=":";
            if(i==(int)text.size()-1 || text[i+1]!=' ')
            temp+=" ";

        }
        else if(text[i] != ' ')
        {
			temp += text[i];
		}
        else
        {
            int j = i;
		    temp += " ";

		while(text[i] == text[j] && j < (int) text.size())
        {
			j++;
		}
		    i = j - 1;
        }
        i++;
    }
    while((temp.back() == ' ' || temp.back() == '\t' )&& temp.size()>0 )
		temp.pop_back();

    int p = 0;
    i=0;
	while(i<temp.size())
	{
        if(temp[i] == ' ')
        p++;
        i++;
    }
    // cout << temp << endl;
    if(p < 3)
    {
    	return temp;
    }
	else
    {
         string msg="Error at line "+ to_string(i+1)+": Syntax Error";
         error_display.push_back(make_pair(i+1,msg));
         return temp;
    }
	return temp;

}

void find_labels_form_tables()
{
    int i=0;
    for(auto it: inp)
    {
    	int oc;
    	i++;
		string temp;
		for(int j = 0; j < (int) it.size(); j++)
        {
			if(it[j] == ':')
            {
				oc = check_name(temp,0);
				if(oc == 0){

                    string msg="";
                    msg="Error at line "+to_string(i)+": Label Name INVALID" ;
        	          error_display.push_back(make_pair(i,msg));
					break;
				}
				if(labels.find(temp) != labels.end())
                {
                    string msg="";
                    msg="Error at line "+to_string(i)+": Redeclaration of label - "+temp;
                    error_display.push_back(make_pair(i,msg));
                    break;

				}
				labels[temp] = i;
				break;
			}
			temp = temp + it[j];
		}
	}
    code.resize((int) inp.size());
    PC.resize((int) inp.size());
	int counter = 0;
	i=0;
	int loop = -1;
	for(auto it: inp)
    {
    	loop++;
    	i = loop;
		vector<string> move;
		string word;
		move.resize(it.size() + 10);
		int ptr = 1,j=0;
		string z = it;
		for(int j = 0; j < z.size(); j++){
			if(it[j] == ':'){
				move[0] = z.substr(0, j + 1);
				reverse(z.begin(), z.end());
				while(j >= 0){
					z.pop_back();
					j--;
				}
				if(z.size())
					z.pop_back();
				reverse(z.begin(), z.end());
				break;
			}
		}
		for(int j = 0; j < (int)z.size(); j++)
        {
			if(it[j] == ' ')
			{
				move[ptr++] = word;
				word = "";
				continue;
			}
			word += z[j];
			if(j == z.size() - 1)
			{
				move[ptr++] = word;
			}
		}
		if(!move[0].empty())
			labels[move[0]] = counter;

        PC[i] = counter;

		if(ptr == 1)
        {
        	string te;
            code[i].label = move[0],code[i].instr = te;
	      code[i].sec_val = te;
	        code[i].category = 0;
			continue;
		}
		counter++;

		if(!mp.count(move[1]))
        {
            string msg="";
            msg="Error at line "+to_string(i+1)+": Mnemonic Invalid";
            error_display.push_back(make_pair(i+1,msg));
			continue;
		}
		int cmp;
		if(ins_type[move[1]] < 1)
		{
			cmp = ins_type[move[1]] + 2;
		}
		else
		{
			cmp = 3;
		}
        if(cmp != ptr)
        {
            string msg="";
            msg="Error at line "+to_string(i+1)+": OpCode Syntax Combination Invalid";
            error_display.push_back(make_pair(i+1,msg));
			continue;
		}
		code[i].category = type(move[2]);  code[i].label = move[0];
	  	code[i].instr = move[1];
	   code[i].sec_val = move[2];
	    // cout << code[i].label << ' ' << code[i].instr << " " << code[i].category << " " << PC[i] << endl;
		if(code[i].category == 1 && !labels.count(code[i].sec_val))
        {
            string msg="Error at line "+to_string(i+1)+": Label or data variable doesn't exist";
            error_display.push_back(make_pair(i+1,msg));
		}
		else if(code[i].category == -1)
        {
            string msg="Error at line "+to_string(i+1)+": Invalid number";
            error_display.push_back(make_pair(i+1,msg));
		}
		i++;
	}

}

void findErrors(){

	for(int i = 0; i < inp.size(); i++)
	{
		if(inp[i].empty())
			continue;
		int sp = 0;
		for(int j = 0; j < inp[i].size(); j++)
		{
			sp += (inp[i][j] == ' ');
		}
		if(sp > 2)
		{
            string msg = "Error at line :"+to_string(i+1)+" Invalid Stntax";
            error_display.push_back(make_pair(i+1,msg));
		}
	}
}

void show_errors()
{
  sort(error_display.begin(), error_display.end());
  for(int i=0;i<error_display.size();i++)
  {
    cout<<error_display[i].second;
  }
  return;
}


int error_msg() {

	ofstream outErrors("logFile.log");
	outErrors << "Log code in logFile.txt\n";
	if(error_display.empty())
    {
		cout << "Code Successfully assembled\n";
		outErrors << "Machine code - machineCode.o";
        cout<<"\n";
		outErrors << "Listing Code -listCode.lst";
        cout<<"\n";
		outErrors.close();
		return 0;
	}
    else
    {
	sort(error_display.begin(), error_display.end());
	cout << "ERRORS stored in logFile.log" << endl;
	for(auto to: error_display)
    {
		outErrors << to.second << endl;
	}
	outErrors.close();
	return 1;
    }
}

void display()
{
    cout << "Log code - logFile.log";
    cout << endl;
	cout << "Machine code - machineCode.o";
    cout << endl;
	cout << "Listing code - listCode.lst";
    cout << endl;
}
void runAssembler()
{
    // -----------------PASS 1 STARTS-----------------
    fstream inputfile;
    string file;
    string text;
    //cin>>file;

    inputfile.open("input.txt", ios::in);
    if(inputfile.fail()){
		 cout<<"ERROR : Unable to read File\n";
		exit(0);
	}
    else
    {
        int lines=0;
        while(getline(inputfile, text))
        {
            string s=clear(lines,text);
            inp.push_back(s);
            lines++;
        }
        map_inst_type();
        map_instructions();
        find_labels_form_tables();

    }
    // -----------------PASS 2 STARTS-----------------
    if(!error_msg())
    {
        listfil.resize((int)inp.size());
	for(int i = 0; i < (int) inp.size(); i++)
    {
    	cout << inp[i] << " " << code[i].instr << endl;
		if(!inp[i].empty())
        {
        	//cout << code[i].instr << endl;
        string s=ConvertdecToHex(PC[i]);
        reverse(s.begin(), s.end());
	    while((int) s.size() < 6)
		s += '0';
	    reverse(s.begin(), s.end());

		string location = s;
		if(code[i].instr == ""){

            listfil[i].loct = location;
	        listfil[i].machine = "        ";
	        machineCode.push_back("        ");
	        listfil[i].ginput = inp[i];
			continue;
		}
		if(code[i].category == 1)
        {
        	// cout << code[i].instr << " " << 1 << endl;
			int decForm;
			if(ins_type[code[i].instr] == 2)
            {
				int val = labels[code[i].sec_val];
				decForm = val - (PC[i] + 1);
			}
			else
            {
				decForm = labels[code[i].sec_val];
			}

            string s= ConvertdecToHex(decForm);
            reverse(s.begin(), s.end());
	        while((int) s.size() < 6)
		    s += '0';
	        reverse(s.begin(), s.end());

            string curMacCode=s;
            curMacCode +=mp[code[i].instr];

            listfil[i].loct = location;
	        listfil[i].machine = curMacCode;
	        machineCode.push_back(curMacCode);
	        listfil[i].ginput = inp[i];

		}
		else if(code[i].category == 0)
        {
            string c ="000000" + mp[code[i].instr];
            listfil[i].loct = location;
	        listfil[i].machine = c;
	        listfil[i].ginput = inp[i];
	        machineCode.push_back(c);

		}
		else
        {
        	// cout << code[i].instr << " " << 2 << endl;
        	string s;
        	if(code[i].instr == "data")
        	{
        		int decForm = stoi(code[i].sec_val, 0, code[i].category);
        		unsigned int num = decForm;
		        s = ConvertdecToHexdata(num);
		        reverse(s.begin(), s.end());
		        while((int) s.size() < 8)
			    s += '0';
		        reverse(s.begin(), s.end());
        	}
        	else
        	{
        		int decForm = stoi(code[i].sec_val, 0, code[i].category);
	            s= ConvertdecToHex(decForm);
	            reverse(s.begin(), s.end());
		        while((int) s.size() < 6)
			    s += '0';
		        reverse(s.begin(), s.end());
        	}
            string curMacCode=s;
            if(code[i].instr != "data")
            	curMacCode += mp[code[i].instr];
            // cout << inp[i] << " " << curMacCode << endl;
            listfil[i].loct = location;
	        listfil[i].machine = curMacCode;
	        listfil[i].ginput = inp[i];
	        machineCode.push_back(curMacCode);
		}
        }

	}
        write();
    }
}
void write(){ //writing back to file

	ofstream outList("listCode.lst");
	for(int i = 0; i < listfil.size(); i++){
		if(!inp[i].empty())
		outList << listfil[i].loct << " " << listfil[i].machine << " " << listfil[i].ginput <<"\n";
	}
	outList.close();
	ofstream outMachineCode;
	outMachineCode.open("machineCode.o",ios::binary | ios::out);
	for(int i = 0; i < machineCode.size(); i++){
		unsigned int x;
        string t= "        ";
		if(!(machineCode[i].empty() || machineCode[i] == t))
		{
    	std::stringstream ss;
    	ss << hex << machineCode[i] ;
    	ss >> x;
     	static_cast<int>(x);
 		outMachineCode.write((const char*)&x,sizeof(unsigned int));
        }
	}
	outMachineCode.close();

	display();
}


//THANKYOU - Shreya Sinha
