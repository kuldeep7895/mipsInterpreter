#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <climits>
#include <unordered_map>
using namespace std;
vector<vector<string>> tokens;
vector<vector<string>> tokenVals;
int reg[32];
int lineNum = 1;
int countInstructions = 0;


vector<vector<string>> allTokens = {{},{"ADD","REG","COMMA","REG","COMMA","REG"},{"SUB","REG","COMMA","REG","COMMA","REG"},{"MUL","REG","COMMA","REG","COMMA","REG"},{"BEQ","REG","COMMA","REG","COMMA","NUM"},{"BNE","REG","COMMA","REG","COMMA","NUM"},{"SLT","REG","COMMA","REG","COMMA","REG"},{"J","NUM"},{"LW","REG","COMMA","NUM","LPAREN","REG","RPAREN"},{"LW","REG","COMMA","NUM"},{"LW","REG","COMMA","LPAREN","REG","RPAREN"},{"SW","REG","COMMA","NUM","LPAREN","REG","RPAREN"},{"SW","REG","COMMA","NUM"},{"SW","REG","COMMA","LPAREN","REG","RPAREN"},{"ADDI","REG","COMMA","REG","COMMA","NUM"},{"LW","REG","COMMA","REG","LPAREN","REG","RPAREN"},{"SW","REG","COMMA","REG","LPAREN","REG","RPAREN"}};

string registerNames[32] = {
	"$zero","$at","$v0","$v1","$a0","$a1","$a2","$a3","$t0","$t1","$t2","$t3","$t4","$t5","$t6","$t7","$s0","$s1","$s2","$s3","$s4","$s5","$s8","$s7","$t8","$t9","$k0","$k1","$gp","$sp","$fp","$ra"
};

bool check_number(string str) {
   int i = 0;
   if(str.length()==0){
   	return false;
   }
   if (str[0]=='-'){
   	i = 1;
   	if(str.length()==1){
   		return false;
   	}
   }
   for (; i < str.length(); i++)
   if (isdigit(str[i]) == false)
      return false;
      return true;
}

int regNum(string registerName){
	if(registerName == "$zero") {return 0;}
	if(registerName == "$at") {return 1;}
	if(registerName == "$v0") {return 2;}
	if(registerName == "$v1") {return 3;}
	if(registerName == "$a0") {return 4;}
	if(registerName == "$a1") {return 5;}
	if(registerName == "$a2") {return 6;}
	if(registerName == "$a3") {return 7;}
	if(registerName == "$t0") {return 8;}
	if(registerName == "$t1") {return 9;}
	if(registerName == "$t2") {return 10;}
	if(registerName == "$t3") {return 11;}
	if(registerName == "$t4") {return 12;}
	if(registerName == "$t5") {return 13;}
	if(registerName == "$t6") {return 14;}
	if(registerName == "$t7") {return 15;}
	if(registerName == "$s0") {return 16;}
	if(registerName == "$s1") {return 17;}
	if(registerName == "$s2") {return 18;}
	if(registerName == "$s3") {return 19;}
	if(registerName == "$s4") {return 20;}
	if(registerName == "$s5") {return 21;}
	if(registerName == "$s6") {return 22;}
	if(registerName == "$s7") {return 23;}
	if(registerName == "$t8") {return 24;}
	if(registerName == "$t9") {return 25;}
	if(registerName == "$k0") {return 26;}
	if(registerName == "$k1") {return 27;}
	if(registerName == "$gp") {return 28;}
	if(registerName == "$sp") {return 29;}
	if(registerName == "$fp") {return 30;}
	if(registerName == "$ra") {return 31;}
	return -1;
}	


string convertToToken(string curString){
	
	regex tReg("\\$t[0-7]");
	regex sReg("\\$s[0-7]");
	regex aReg("\\$a[0-3]");
	
	if(curString=="add"){
		return "ADD";
	}
	else if(curString=="sub"){
		return "SUB";
	}
	else if(curString=="mul"){
		return "MUL";
	}
	else if(curString=="beq"){
		return "BEQ";
	}
	else if(curString=="bne"){
		return "BNE";
	}
	else if(curString=="slt"){
		return "SLT";
	}
	else if(curString=="j"){
		return "J";
	}
	else if(curString=="lw"){
		return "LW";
	}
	else if(curString=="sw"){
		return "SW";
	}
	else if(curString=="addi"){
		return "ADDI";
	}
	else if((regex_match(curString,tReg) )||(regex_match(curString,sReg) )||(regex_match(curString,aReg))||(curString=="$zero")||(curString=="$at")||(curString=="$v0")||(curString=="$v1")||(curString=="$k0")||(curString=="$k1")||(curString=="$gp")||(curString=="$sp")||(curString=="$fp")||(curString=="$ra")){
		return "REG";
	}
	else if(check_number(curString)){
		return "NUM";
	}
	else{
		return "-1";
	}
}

bool tokenise(string line){
	
	string curString = "";
	int n = line.length();
	int i = 0;
	vector<string> curLinetokens;
	vector<string> curLinetokenVals;
	
	while((line[i]==' '||line[i]=='\t')){
	
		i++;
	}
	while(i<n&&line[i]!='\n'){
		//cout<<curString<<endl;
		//cout<<line[i]<<endl;
		if(line[i]==' '||line[i]=='\t'){
			if(curString!=""){
				string token = convertToToken(curString);
				//cout<<token<<endl;
				if(token!="-1"){
					curLinetokens.push_back(token);
					curLinetokenVals.push_back(curString);
				}else{
					cout<<"Invalid token on line num "<<lineNum<<" after "<<curString<<endl;
					//cout<<"here"<<curString<<endl;
					return false;
				}
				while((line[i]==' '||line[i]=='\t')){
					i++;
				}
				//cout<<curString<<endl;
				curString = "";
			}else{
				while((line[i]==' '||line[i]=='\t')){
					i++;
				}
			}
			
		}
		else{
			if(line[i]==','||line[i]=='('||line[i]==')'){
			
				if(curString!=""){
					string token = convertToToken(curString);	
					if(token!="-1"){
						curLinetokens.push_back(token);
						curLinetokenVals.push_back(curString);
					}else{
						cout<<"Invalid token on line num "<<lineNum<<" after "<<curString<<endl;
						//cout<<"here"<<curString<<endl;
						return false;
					}
				}
				
				if(line[i]==','){
					curLinetokens.push_back("COMMA");
					curLinetokenVals.push_back(",");
				}
				if(line[i]=='('){
					curLinetokens.push_back("LPAREN");
					curLinetokenVals.push_back("(");
				}
				if(line[i]==')'){
					curLinetokens.push_back("RPAREN");
					curLinetokenVals.push_back(")");
				}
				curString = "";
			}else{
				curString+=line[i];
			}
			i++;
		}
	}
	if(curString!="\n"&&curString!=""){
		
		string token = convertToToken(curString);
		//cout<<token<<endl;
		if(token!="-1"){
			curLinetokens.push_back(token);
			curLinetokenVals.push_back(curString);
		}else{
			return false;
		}
		//cout<<curString<<endl;
	}else{
		if(curLinetokens.size()==0){
			tokens.push_back(curLinetokens);
			tokenVals.push_back(curLinetokenVals);
			return true;
		}
	}
	
	//cout<<"hi"<<endl;
	tokens.push_back(curLinetokens);
	tokenVals.push_back(curLinetokenVals);
	return true;
}

bool Parse(){
	
	
	for(int i=0; i<tokens.size(); i++){
		if(tokens[i].size()!=0){
				countInstructions++;
			}
			
		int j = 0;
		for(;j<allTokens.size();j++){
			
//			for(int k=0; k<allTokens[j].size();k++){
//  				 cout<<allTokens[j][k] << ' ';
//  			}
//  			cout<<endl;

			
  			
			if(allTokens[j]==tokens[i]){
				//cout<<"gk"<<j<<endl;
				break;
			}
			
		}
		//cout<<allTokens.size()<<"    "<<j<<endl;
		if(j==allTokens.size()){
			//cout<<"jnlkjlkjl"<<i<<endl;
			cout<<"ERROR ON LINE NUM "<<(i+1)<<endl;
			return false;
		}
	}
	
	return true;
}

int main(){

	int ROW_ACCESS_DELAY = 10;
	int COL_ACCESS_DELAY = 2;
	int ROW_BUFFER = 0;
	
	int countClockCycle = 0;
	for (int i = 0 ; i<32 ;i++){
		reg[i] = INT_MAX;
	}
	reg[0] = 0;

	string line;
	ifstream myfile ("example.asm");
	
	if (myfile.is_open()){
		while ( getline (myfile,line) ){
			//cout << line << '\n';
			string a = line;
			//cout<<a<<endl;
			bool tokenRes = tokenise(a);
			if(!tokenRes){
				return -1;
			}
			lineNum++;
    		}
		myfile.close();
 	}

  	else cout << "Unable to open file"; 
  	
  	
//  	  	for(int i=0; i<tokens.size(); i++){
//  		//cout<<"siz e"<<tokens[i].size()<<endl;
//  		for(int j=0; j<tokens[i].size(); j++){
//  			 cout<<tokens[i][j] << ' ';
//  		}
//  		cout <<endl;
//  	}
  	
  	bool parseRes = Parse();
  	if(!parseRes){
  		return -1;
  	}
  	
  	
  	
  	int memory[262144-countInstructions];
  	reg[29] = (262144)*4;
  	
  	
  	for (int i = 0 ; i< sizeof(memory)/ sizeof(memory[0]);i++){
		memory[i] = INT_MAX;
	}
	
	unordered_map<int, int> instrCount;
	int countWords = countInstructions;
	

	
	vector<int> memUpdateLoc;
	vector<int> regUpdateLoc;
	vector<int> curMemUpdateLoc;
	int i = 0;
	int prevClockCycle = 0;
	while(i<tokens.size()){
		
		
//		for(int j=0; j<tokenVals[i].size(); j++){
//			
//  			 cout<<tokenVals[i][j] << ' ';
//  		
//  		}	
  				
		if(tokens[i].size()!=0){
		
			
			
			
			instrCount[i]++;
			if(countClockCycle!=prevClockCycle){
				if(countClockCycle-prevClockCycle==1){
					cout<<"cycle "<<countClockCycle<<" : ";
					for (int i = 0 ; i<regUpdateLoc.size();i++){
						std::stringstream stream;
						stream << std::hex << reg[regUpdateLoc[i]];
						std::string result( stream.str() );
						cout<<registerNames[regUpdateLoc[i]]<<": "<<result<<" ";
					}
					
					for(int loc : curMemUpdateLoc){
						std::stringstream stream;
						stream << std::hex << memory[((loc)/4)-countInstructions];
						std::string result( stream.str() );
							
						cout <<loc <<" to "<<loc+3<<": "<<result<<"  ";		
					}
				}
				else{
					
					cout<<"clock "<<prevClockCycle+2<<"-"<<countClockCycle<<" : ";
					for (int i = 0 ; i<regUpdateLoc.size();i++){
						std::stringstream stream;
						stream << std::hex << reg[regUpdateLoc[i]];
						std::string result( stream.str() );
						cout<<registerNames[regUpdateLoc[i]]<<": "<<result<<" ";
					}
					
					for(int loc : curMemUpdateLoc){
						std::stringstream stream;
						stream << std::hex << memory[((loc)/4)-countInstructions];
						std::string result( stream.str() );
							
						cout <<loc <<" to "<<loc+3<<": "<<result<<"  ";		
					}
				
				}
				prevClockCycle = countClockCycle;
				cout<<endl;
			}
			
			
			regUpdateLoc.clear();
			curMemUpdateLoc.clear();
			// ADD 
			
			if(tokens[i]==allTokens[1]){
				int reg1 = regNum(tokenVals[i][1]);
				int reg2 = regNum(tokenVals[i][3]);
				int reg3 = regNum(tokenVals[i][5]);
				//cout<<reg1<<"  "<<reg2<<"   "<<reg3<<"   "<<endl;
				if(reg[reg2]==INT_MAX||reg[reg3]==INT_MAX){
					cout<<"No value in registered stored cnn;t retrieve"<<endl;
					break;
				}
				else{
					regUpdateLoc.push_back(reg1);
					
					reg[reg1] = reg[reg2]+reg[reg3];
				}
			}
			// SUB 	
			else if(tokens[i]==allTokens[2]){
				int reg1 = regNum(tokenVals[i][1]);
				int reg2 = regNum(tokenVals[i][3]);
				int reg3 = regNum(tokenVals[i][5]);
				//cout<<reg1<<"  "<<reg2<<"   "<<reg3<<"   "<<endl;
				if(reg[reg2]==INT_MAX||reg[reg3]==INT_MAX){
					cout<<"No value in registered stored cnn;t retrieve"<<endl;
					break;
				}
				else{
					regUpdateLoc.push_back(reg1);
					reg[reg1] = reg[reg2]-reg[reg3];
				}
			}
			// MUL
			else if(tokens[i]==allTokens[3]){
				int reg1 = regNum(tokenVals[i][1]);
				int reg2 = regNum(tokenVals[i][3]);
				int reg3 = regNum(tokenVals[i][5]);
				//cout<<reg1<<"  "<<reg2<<"   "<<reg3<<"   "<<endl;
				if(reg[reg2]==INT_MAX||reg[reg3]==INT_MAX){
					cout<<"No value in registered stored cnn;t retrieve"<<endl;
					break;
				}
				else{
					regUpdateLoc.push_back(reg1);
					reg[reg1] = reg[reg2]*reg[reg3];
				}
			}
			// BEQ
			else if(tokens[i]==allTokens[4]){
				int reg1 = regNum(tokenVals[i][1]);
				int reg2 = regNum(tokenVals[i][3]);
				//cout<<"hejn"<<tokenVals[i][5]<<endl;
				int num = stoi(tokenVals[i][5]);
				//cout<<reg1<<"  "<<reg2<<"   "<<endl;
				if(reg[reg2]==INT_MAX||reg[reg1]==INT_MAX){
					cout<<"No value in registered stored cnn;t retrieve"<<endl;
					break;
				}
				else{
					if(reg[reg1]==reg[reg2]){
						if(num>tokenVals.size()||num<1){
							cout<<"Invalid line num"<<endl;
							break;
						}
						i = num-1;
						continue;
					}
				}
			}   
			// BNE 
			else if(tokens[i]==allTokens[5]){
				int reg1 = regNum(tokenVals[i][1]);
				int reg2 = regNum(tokenVals[i][3]);
				
				int num = stoi(tokenVals[i][5]);
				//cout<<reg1<<"  "<<reg2<<"   "<<num<<endl;
				if(reg[reg2]==INT_MAX||reg[reg1]==INT_MAX){
					cout<<"No value in registered stored cnn;t retrieve"<<endl;
					break;
				}
				else{
					if(reg[reg1]!=reg[reg2]){
						if(num>tokenVals.size()||num<1){
							cout<<"Invalid line num"<<endl;
							break;
						}
						i = num-1;
						continue;
					}
				}
				
				
			}
			// SLT
			else if(tokens[i]==allTokens[6]){
				int reg1 = regNum(tokenVals[i][1]);
				int reg2 = regNum(tokenVals[i][3]);
				int reg3 = regNum(tokenVals[i][5]);
				//cout<<reg1<<"  "<<reg2<<"   "<<reg3<<"   "<<endl;
				if(reg[reg2]==INT_MAX||reg[reg3]==INT_MAX){
					cout<<"No value in registered stored cnn;t retrieve"<<endl;
					break;
				}
				else{
					if(reg[reg2]<reg[reg3]){
						reg[reg1] = 1;
					}else{
						reg[reg1] = 0;
					}
					regUpdateLoc.push_back(reg1);
				
				}
			}	
			
			// J NUM 
			else if(tokens[i]==allTokens[7]){
					//cout<<tokenVals[i][1]<<endl;
					
					int num = stoi(tokenVals[i][1]);
					if(num>tokenVals.size()||num<1){
							cout<<"Invalid line num"<<endl;
							break;
						}
					i = num-1;
					continue;
					
			}
			
			// LW REG NUM ( REG )
			else if(tokens[i]==allTokens[8]){
				
					
					int reg1 = regNum(tokenVals[i][1]);
					int num = stoi(tokenVals[i][3]);
				
					//cout<<tokenVals[i][5]<<endl;
					int reg2 = regNum(tokenVals[i][5]);
					if(reg[reg2]==INT_MAX){
						cout<<"No value in registered stored cnn;t retrieve"<<endl;
						break;
					}
					else{
						if((num+reg[reg2])%4!=0){
							cout<<"We consider memory as chunks of words and the addres is not a multiple of 4";
							break;
						}
						int loc = ((num+reg[reg2])/4)-countInstructions;
						
						if(memory[loc]==INT_MAX){
							cout<<"Memory location is empty"<<endl;
							break;
						}
						
						if(loc<0||loc>=262144-countInstructions){
							cout<<"Out of memory"<<endl;
							break;
						}
						int row = ((num+reg[reg2])/1024)+1;
						cout<<"clock "<<countClockCycle+1<<" DRAM request issued"<<endl;
						if(ROW_BUFFER==0){
							ROW_BUFFER = row;
							countClockCycle+=ROW_ACCESS_DELAY+COL_ACCESS_DELAY;
						}else{
							if(ROW_BUFFER==row){
								countClockCycle+=COL_ACCESS_DELAY;
							}else{
								ROW_BUFFER = row;
								countClockCycle+=(ROW_ACCESS_DELAY*2)+COL_ACCESS_DELAY;
							}
						}
						
						reg[reg1] = memory[loc];
						regUpdateLoc.push_back(reg1);
					}
			}
			
			
			// LW REG NUM 
			else if(tokens[i]==allTokens[9]){
				
					int reg1 = regNum(tokenVals[i][1]);
					int num = stoi(tokenVals[i][3]);
					if((num)%4!=0){
						cout<<"We consider memory as chunks of words and the addres is not a multiple of 4";
						break;
					}
					int loc = ((num)/4)-countInstructions;
					if(memory[loc]==INT_MAX){
						cout<<"Memory location is empty"<<endl;
						break;
					}
					
					if(loc<0||loc>=262144-countInstructions){
							cout<<"Out of memory"<<endl;
							break;
					}
					
					int row = ((num)/1024)+1;
					cout<<"clock "<<countClockCycle+1<<" DRAM request issued"<<endl;
					if(ROW_BUFFER==0){
							ROW_BUFFER = row;
							countClockCycle+=ROW_ACCESS_DELAY+COL_ACCESS_DELAY;
						}else{
							if(ROW_BUFFER==row){
								countClockCycle+=COL_ACCESS_DELAY;
							}else{
								ROW_BUFFER = row;
								countClockCycle+=(ROW_ACCESS_DELAY*2)+COL_ACCESS_DELAY;
						}
					}
					
					reg[reg1] = memory[loc];
					regUpdateLoc.push_back(reg1);
					
			}
			
			// LW REG (REG)
			else if(tokens[i]==allTokens[10]){
				
					int reg1 = regNum(tokenVals[i][1]);
					int reg2 = regNum(tokenVals[i][4]);
					if(reg[reg2]==INT_MAX){
						cout<<"No value in registered stored cnn;t retrieve"<<endl;
						break;
					}
					else{
					
						
						if((reg[reg2])%4!=0){
							cout<<"We consider memory as chunks of words and the addres is not a multiple of 4";
							break;
						}
						int loc = ((reg[reg2])/4) - countInstructions;
						
						if(memory[loc]==INT_MAX){
							cout<<"Memory location is empty"<<endl;
							break;
						}
						
						
						if(loc<0||loc>=262144-countInstructions){
							cout<<"Out of memory"<<endl;
							break;
						}
						
						
						int row = ((reg[reg2])/1024)+1;
						cout<<"clock "<<countClockCycle+1<<" DRAM request issued"<<endl;
						if(ROW_BUFFER==0){
								ROW_BUFFER = row;
								countClockCycle+=ROW_ACCESS_DELAY+COL_ACCESS_DELAY;
							}else{
								if(ROW_BUFFER==row){
									countClockCycle+=COL_ACCESS_DELAY;
								}else{
									ROW_BUFFER = row;
									countClockCycle+=(ROW_ACCESS_DELAY*2)+COL_ACCESS_DELAY;
							}
						}
						
						reg[reg1] = memory[loc];
						regUpdateLoc.push_back(reg1);
					}
			}
			// SW REG NUM ( REG )
			else if(tokens[i]==allTokens[11]){
				
					int reg1 = regNum(tokenVals[i][1]);
					int num = stoi(tokenVals[i][3]);
					int reg2 = regNum(tokenVals[i][5]) ;
					if(reg[reg2]==INT_MAX||reg[reg1]==INT_MAX){
						cout<<"No value in registered stored cnn;t retrieve"<<endl;
						break;
					}
					else{
						if((num+reg[reg2])%4!=0){
							
							cout<<"We consider memory as chunks of words and the addres is not a multiple of 4";
							break;
						}
						
						int loc = ((num+reg[reg2])/4)-countInstructions;
						
						//cout<<loc<<"   "<<num<<"   "<<reg2<<"   "<<countInstructions<<endl;
						if(loc<0||loc>=262144-countInstructions){
							cout<<"Out of memory"<<endl;
							break;
						}
						
						if(reg[reg1]==INT_MAX){
							cout<<"Memory location is empty"<<endl;
							break;
						}
						if(memory[loc]==INT_MAX){
							countWords++;
							memUpdateLoc.push_back(num+reg[reg2]);
							curMemUpdateLoc.push_back(num+reg[reg2]);
						}
						
						
						
						int row = ((num+reg[reg2])/1024)+1;
						cout<<"clock "<<countClockCycle+1<<" DRAM request issued"<<endl;
						if(ROW_BUFFER==0){
								ROW_BUFFER = row;
								countClockCycle+=ROW_ACCESS_DELAY+COL_ACCESS_DELAY;
							}else{
								if(ROW_BUFFER==row){
									countClockCycle+=COL_ACCESS_DELAY;
								}else{
									ROW_BUFFER = row;
									countClockCycle+=(ROW_ACCESS_DELAY*2)+COL_ACCESS_DELAY;
							}
						}
						
						
						memory[loc] = reg[(reg1)];
						
						
					}
			}
			// SW REG NUM
			else if(tokens[i]==allTokens[12]){
				
					int reg1 = regNum(tokenVals[i][1]);
					int num = stoi(tokenVals[i][3]);
					
					if(num%4!=0){
						cout<<"We consider memory as chunks of words and the addres is not a multiple of 4";
						break;
					}
					
					int loc = (num/4) - countInstructions;
					
					if(reg[reg1]==INT_MAX){
						cout<<"Memory location is empty"<<endl;
						break;
					}
					
					
						
						if(loc<0||loc>=262144-countInstructions){
							cout<<"Out of memory"<<endl;
							break;
						}
					if(memory[loc]==INT_MAX){
						countWords++;
						memUpdateLoc.push_back(num);
						curMemUpdateLoc.push_back(num);
					}
					
					int row = ((num)/1024)+1;
					cout<<"clock "<<countClockCycle+1<<" DRAM request issued"<<endl;
					if(ROW_BUFFER==0){
								ROW_BUFFER = row;
								countClockCycle+=ROW_ACCESS_DELAY+COL_ACCESS_DELAY;
							}else{
								if(ROW_BUFFER==row){
									countClockCycle+=COL_ACCESS_DELAY;
								}else{
									ROW_BUFFER = row;
								countClockCycle+=(ROW_ACCESS_DELAY*2)+COL_ACCESS_DELAY;
						}
					}
					
					
					memory[loc] = reg[(reg1)] ;
				
					
			}
			// sw reg (reg)
			else if(tokens[i]==allTokens[13]){
				
					int reg1 = regNum(tokenVals[i][1]);
			
					int reg2 = regNum(tokenVals[i][4]);
					if(reg[reg2]==INT_MAX||reg[reg1]==INT_MAX){
						cout<<"No value in registered stored cnn;t retrieve"<<endl;
						break;
					}
					else{
						if((reg[reg2])%4!=0){
							cout<<"We consider memory as chunks of words and the addres is not a multiple of 4";
							break;
						}
						
						int loc = (reg[reg2]/4)-countInstructions;

						cout<<loc<<endl;
						if(loc<0||loc>=262144-countInstructions){
							cout<<"Out of memory"<<endl;
							break;
						}
												
						
						if(memory[loc]==INT_MAX){
							countWords++;
							memUpdateLoc.push_back(reg[reg2]);
							curMemUpdateLoc.push_back(reg[reg2]);
						}
						
						
						int row = ((reg[reg2])/1024)+1;
						cout<<"clock "<<countClockCycle+1<<" DRAM request issued"<<endl;
						if(ROW_BUFFER==0){
								ROW_BUFFER = row;
								countClockCycle+=ROW_ACCESS_DELAY+COL_ACCESS_DELAY;
							}else{
								if(ROW_BUFFER==row){
									countClockCycle+=COL_ACCESS_DELAY;
								}else{
									ROW_BUFFER = row;
									countClockCycle+=(ROW_ACCESS_DELAY*2)+COL_ACCESS_DELAY;
							}
						}
						
						memory[loc] = reg[(reg1)];
						
					}
			}
			// ADDI REG1 REG2 NUM
			else if(tokens[i]==allTokens[14]){
				int reg1 = regNum(tokenVals[i][1]);
				int reg2 = regNum(tokenVals[i][3]);
				//cout<<tokenVals[i][5]<<endl;
				int num = stoi(tokenVals[i][5]);
				//cout<<reg1<<"  "<<reg2<<"   "<<endl;
				if(reg[reg2]==INT_MAX){
					cout<<"No value in registered stored cnn;t retrieve"<<endl;
					break;
				}
				else{
					reg[reg1] = reg[reg2]+num;
					regUpdateLoc.push_back(reg1);
				}
			}
			
			// LW REG REG(REG)
			
			else if(tokens[i]==allTokens[15]){
				
					int reg1 = regNum(tokenVals[i][1]);
					int reg2 = regNum(tokenVals[i][3]);
					int reg3 = regNum(tokenVals[i][5]) ;
					if(reg[reg2]==INT_MAX||reg[reg3]==INT_MAX){
						cout<<"No value in registered stored cnn;t retrieve"<<endl;
						break;
					}
					else{
					
						if((reg[reg2]+reg[reg3])%4!=0){
							cout<<"We consider memory as chunks of words and the addres is not a multiple of 4";
							break;
						}
						
						int loc = ((reg[reg3]+reg[reg2])/4) - countInstructions;
						
						if(memory[loc]==INT_MAX){
							cout<<"Memory location is empty"<<endl;
							break;
						}
						
						if(loc<0||loc>=262144-countInstructions){
							cout<<"Out of memory"<<endl;
							break;
						}
						
						
						int row = ((reg[reg3]+reg[reg2])/1024)+1;
						cout<<"clock "<<countClockCycle+1<<" DRAM request issued"<<endl;
						if(ROW_BUFFER==0){
								ROW_BUFFER = row;
								countClockCycle+=ROW_ACCESS_DELAY+COL_ACCESS_DELAY;
							}else{
								if(ROW_BUFFER==row){
									countClockCycle+=COL_ACCESS_DELAY;
								}else{
									ROW_BUFFER = row;
									countClockCycle+=(ROW_ACCESS_DELAY*2)+COL_ACCESS_DELAY;
							}
						}
						
						
						reg[reg1] = memory[loc];
						regUpdateLoc.push_back(reg1);
					}
			}
			// SW  REG REG (REG)
			else if(tokens[i]==allTokens[16]){
				
					int reg1 = regNum(tokenVals[i][1]);
					int reg2 = regNum(tokenVals[i][3]);
					int reg3 = regNum(tokenVals[i][5]) ;
					if(reg[reg2]==INT_MAX||reg[reg3]==INT_MAX){
						cout<<"No value in registered stored cnn;t retrieve"<<endl;
						break;
					}
					else{
						if((reg[reg2]+reg[reg3])%4!=0){
							cout<<"We consider memory as chunks of words and the addres is not a multiple of 4";
							break;
						}
						
						int loc = (reg[reg3]+reg[reg2])/4 - countInstructions; 
						
						
						
						if(loc<0||loc>=262144-countInstructions){
							cout<<"Out of memory"<<endl;
							break;
						}
						
						if(reg[reg1]==INT_MAX){
							cout<<"Memory location is empty"<<endl;
							break;
						}
						if(memory[loc]==INT_MAX){
							countWords++;
							memUpdateLoc.push_back(reg[reg3]+reg[reg2]);
							curMemUpdateLoc.push_back(reg[reg3]+reg[reg2]);
						}
						
						
						int row = ((reg[reg3]+reg[reg2])/1024)+1;
						cout<<"clock "<<countClockCycle+1<<" DRAM request issued"<<endl;
						if(ROW_BUFFER==0){
								ROW_BUFFER = row;
								countClockCycle+=ROW_ACCESS_DELAY+COL_ACCESS_DELAY;
							}else{
								if(ROW_BUFFER==row){
									countClockCycle+=COL_ACCESS_DELAY;
								}else{
									ROW_BUFFER = row;
									countClockCycle+=(ROW_ACCESS_DELAY*2)+COL_ACCESS_DELAY;
							}
						}
						
						
						memory[loc] = reg[reg1];
						
					}
			}
			countClockCycle++;
		}
		//cout<<i<<endl;
		i++;
		
	}

	
//	cout<<"Result after Last instruction succesfully executed"<<endl;
//	for (int i = 0 ; i<32 ;i++){
//	
//		std::stringstream stream;
//		stream << std::hex << reg[i];
//		std::string result( stream.str() );
//		cout<<registerNames[i]<<":   "<<result<<"       ";	
//	}
//			
//	cout<<endl<<endl;
	
	
	
				if(countClockCycle!=prevClockCycle){
				if(countClockCycle-prevClockCycle==1){
					cout<<"cycle "<<countClockCycle<<" : ";
					for (int i = 0 ; i<regUpdateLoc.size();i++){
						std::stringstream stream;
						stream << std::hex << reg[regUpdateLoc[i]];
						std::string result( stream.str() );
						cout<<registerNames[regUpdateLoc[i]]<<": "<<result<<" ";
					}
					
					for(int loc : curMemUpdateLoc){
						std::stringstream stream;
						stream << std::hex << memory[((loc)/4)-countInstructions];
						std::string result( stream.str() );
							
						cout <<loc <<" to "<<loc+3<<": "<<result<<"  ";		
					}
				}
				else{
					
					cout<<"clock "<<prevClockCycle+2<<"-"<<countClockCycle<<" : ";
					for (int i = 0 ; i<regUpdateLoc.size();i++){
						std::stringstream stream;
						stream << std::hex << reg[regUpdateLoc[i]];
						std::string result( stream.str() );
						cout<<registerNames[regUpdateLoc[i]]<<": "<<result<<" ";
					}
					
					for(int loc : curMemUpdateLoc){
						std::stringstream stream;
						stream << std::hex << memory[((loc)/4)-countInstructions];
						std::string result( stream.str() );
							
						cout <<loc <<" to "<<loc+3<<": "<<result<<"  ";		
					}
				
				}
				prevClockCycle = countClockCycle;
				cout<<endl;
			}
			
	
	
	cout<<"Words stored "<< countWords<<endl;
	cout<<"Clock cycle count = "<< countClockCycle<<endl;
	cout<<"Final data values that are updated during execution:"<<endl;
	for(int loc : memUpdateLoc){
		std::stringstream stream;
		stream << std::hex << memory[((loc)/4)-countInstructions];
		std::string result( stream.str() );
			
		cout <<loc <<" to "<<loc+3<<": "<<result<< endl;		
	} 
  
	
	unordered_map<int, int>:: iterator p;
	for (p = instrCount.begin(); p != instrCount.end(); p++){
        	cout << "Instruction at line number : " << (p->first +1)<< " was executed " << p->second << " times \n";
        }
	
//  	cout<<tokens.size()<<endl;
//  	for(int i=0; i<tokens.size(); i++){
//  		//cout<<"siz e"<<tokens[i].size()<<endl;
//  		for(int j=0; j<tokens[i].size(); j++){
//  			 cout<<tokens[i][j] << ' ';
//  		}
//  		cout <<endl;
//  	}
//  	
//  	for(int i=0; i<tokenVals.size(); i++){
//  		//cout<<"siz e"<<tokens[i].size()<<endl;
//  		for(int j=0; j<tokenVals[i].size(); j++){
//  			 cout<<tokenVals[i][j] << ' ';
//  		}
//  		cout <<endl;
//  	}

  	return 0;
}
