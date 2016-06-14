#include <iostream>
#include <stack>
#include <iterator>
#include <string.h>
#include <sstream>
#include "tree.h"
#include "token.h"
#include <cstdlib>
#include <vector>
#include <math.h>
#include <stdio.h>
#include "environment.h"

using namespace std;

stack<tree*> st;

string keys[] = { "let", "fn", "in",  "where", "aug", "or", "not","true", "false", "nil" , "dummy","within",
					"and", "rec", "gr", "ge", "ls","le", "eq", "ne"};

class parser{

	public:
	token nt;
	char readnew[10000];
	int index;
	int sizeOfFile;
	int astFlag;

	parser(char read1[], int i, int size, int af){
		//readnew = read;
		copy(read1, read1+10000,readnew);
		index = i;
		sizeOfFile=size;
		astFlag = af;
	}

	void read( string val , string type ) {
		//cout << "passed token " << t.getVal() << " with type " << t.getType() << endl;

		if( val != nt.getVal() || type != nt.getType()){
			cout << "Expected : " << val << " - " << type <<" but found : " << nt.getVal() << " - " << nt.getType() << endl;
			exit(0);
		}

		if(type=="ID" || type=="INT" || type=="STR"){
//			cout << " build tree for " << t.getType() << " and value " << t.getVal() << endl;
			build_tree( val , type , 0);
		}
		//cout << " preparing next token  " << endl;
		nt = getToken(readnew);
		//cout << nt.getType() << " is the next token -------------------" << nt.getVal() << endl;
		while( nt.getType()=="DELETE"){
			//cout << " inside the delete " << nt.getVal() << endl;
			nt = getToken(readnew);
		}

		//cout << "  next token is " << nt.getVal() << " and type " << nt.getType() << endl;

	}

	void build_tree(string val,string type, int child){
		//cout << " building tree with " << val << " and " << type << " and child " << child << endl;
		if(child==0){
			//st.push()
			tree *temp = createNode(val,type);
			//cout << temp->val;
			st.push(temp);

			//cout << st.top()->val << " only 1 element " << endl;
		}else if (child>0){
		//	cout << " need to build tree with " << child << " nodes " << endl;
			stack<tree*> temp;
			int no_of_pops=child;
			while( !st.empty() && no_of_pops>0 ){
				temp.push(st.top());
				st.pop();
				no_of_pops--;
			}
			tree *tempLeft = temp.top();
		//	cout << " temp left Node from stack " <<tempLeft->getVal() << endl;
			temp.pop();
			child--;
			if(!temp.empty() && child >0){
				tree *rightNode = temp.top();
				tempLeft->right = rightNode;
		//		cout << " right of temp left " << rightNode->getVal() << endl;
				temp.pop();
				child--;
			while(!temp.empty() && child>0 ){
				tree *addRight = temp.top();
		//		cout << " getting right child from stack till no of child exist" << addRight->getVal() << endl;
				temp.pop();
				rightNode->right = addRight;
				rightNode = rightNode->right;
				child--;
			}
		}
		if(!st.empty()){
		//	cout << " next top element is " << st.top()->getVal() << " and " << st.top()->getType() << endl;
		}
		tree *toPush = createNode(val,type);
		toPush->left=tempLeft;
		st.push(toPush);
		//cout << " put data on stack " << st.top()->getVal() << "   "<<st.top()->getType() << endl;
	}
}

	tree *createNode(string value, string typ){
		tree *temp = new tree();
		temp->setVal(value);
		temp->setType(typ);
		temp->left = NULL;
		temp->right = NULL;
		return temp;
	}

	tree *createNode(tree* x){
		tree *temp = new tree();
		temp->setVal(x->getVal());
		temp->setType(x->getType());
		temp->left = x->left;
		temp->right = NULL;
		return temp;
	}

	bool isReservedKey(string str){
		//int size = keys.size();
		int i=0;
		for(i=0;i<20;i++){
			if(str == keys[i]) return true;
		}
		return false;
	}

	bool isOperator( char ch){
		if( ch == '+' || ch == '-' || ch == '*' ||  ch == '<' ||  ch == '>' ||  ch == '&' ||  ch == '.' ||
	 		ch == '@' ||  ch == '/' ||  ch == ':' ||  ch == '=' ||  ch == '~' || ch == '|' ||  ch == '$' ||
	 		ch == '!' ||  ch == '#' || ch == '%' || ch == '^' || ch == '_' || ch == '[' || ch == ']' ||
		  	ch == '{' || ch == '}' || ch == '"' || ch == '`' || ch == '?' ){
		  		return true;
		  }
		  return false;
	}

	bool isAlpha( char ch){
		if( (ch >= 65 && ch <= 90) || (ch >= 97 && ch <= 122) ){
			return true;
		}
		return false;
	}

	bool isDigit(char ch){
		if( ch >= 48 && ch <= 57){
			return true;
		}
		return false;
	}


	token getToken(char read[]){
		//cout << " index " << index << endl;
		token t;
		int i = index; //resetting i
//		string tokenType;
		string id ="";
		string num ="";
		string isop ="";
		string isString ="";
		string isPun = "";
		string isComment = "";
		string isSpace = "";

		if(read[i] == '\0' || i==sizeOfFile ){
		//	cout << " No more input to process";
			t.setType("EOF");
			t.setVal("EOF");
			return t;
		}
 		while(i < sizeOfFile || i < 10000 || read[i] != '\0') {
 	//		cout << " in identifier " << endl;
			if ( isDigit(read[i]) ){ 	// read digit
				while( isDigit(read[i]) ){
					num = num+read[i];
					i++;
				}
				index = i;
				t.setVal(num);
				t.setType("INT");
//				cout << " token " << t.getVal() << " type " << t.getType() << endl;
				return t;
			}
			else if( isAlpha(read[i]) ){ // read identifier
				while( isAlpha(read[i]) || isDigit(read[i]) || read[i]== '_' ){
					id = id+read[i];
					i++;
				}

				if( isReservedKey(id)){
					index = i;
					t.setVal(id);
					t.setType("KEYWORD");
//					cout << " token " << t.getVal() << " type " << t.getType() << endl;
 					return t;
				}else{
					index = i;
					t.setVal(id);
					t.setType("ID");
//					cout << " token " << t.getVal() << " type " << t.getType() << endl;
					return t;
				}

			}
			else if( read[i]=='/' && read[i+1] == '/'){ //read comment here
				while( read[i] == '\''  || read[i] == '\\' ||read[i] == '(' || read[i] == ')' || read[i] == ';' || read[i] == ',' || read[i] == ' '
					|| read[i] == '\t' || isAlpha(read[i]) || isDigit(read[i]) ||	isOperator(read[i]) ) {

					 if( read[i] == '\n'){
			 			i++;
		 				break;
			 		}else{
			 			isComment = isComment + read[i];
						i++;
					 }
				}

				index = i;
				t.setVal(isComment);
				t.setType("DELETE"); //
			//cout << " token " << t.getVal() << " type " << t.getType() << endl;
				return t;

			}
			else if ( isOperator(read[i]) ) {
				while (isOperator(read[i]) ){
					isop = isop + read[i];
					i++;
				}

				index = i;
				t.setVal(isop);
				t.setType("OPERATOR");
//			cout << " token " << t.getVal() << " type " << t.getType() << endl;
				return t;
			}
			else if( read[i] == '\''){ //read string
				isString = isString + read[i];
				i++;
				while( read[i] == '\'' || read[i] == '\\'  || read[i] == '(' || read[i] == ')' || read[i] == ';' || read[i] == ',' || read[i] == ' '
					|| isAlpha(read[i]) || isDigit(read[i]) || read[i]== '_' ||	isOperator(read[i]) )
				{
					if(read[i] == '\'' ) {
						isString = isString + read[i];
						i++;
						break;
					}
					else if ( read[i] == '\\'){
						isString = isString + read[i];
						i++;
						if(read[i]== 't' || read[i]=='n' || read[i] == '\\' || read[i] == '"'){
							isString = isString + read[i];
							i++;
						}else{
							cout <<  " hello from the no string !!!!!!!!!!!" << read[i] << endl;
							i++;
						}
					}
					else {
						 isString = isString + read[i];
					 	i++;
					}

				}
				index = i;
				t.setVal(isString);
				t.setType("STR");
//			cout << " token " << t.getVal() << " type " << t.getType() << endl;
				return t;

			}
			else if ( read[i] == ')' || read[i] == '(' || read[i] == ';' || read[i] == ',' ){
			//while( read[i] == ')' || read[i] == '(' || read[i] == ';' || read[i] == ',' ){
				isPun = isPun+read[i];
				i++;
			//}

				index = i;
				t.setVal(isPun);
				t.setType("PUNCTION");
//			cout << " token " << t.getVal() << " type " << t.getType() << endl;
				return t;
			}
			else if( isspace(read[i]) ) {
				while( isspace(read[i])){
					isSpace = isSpace + read[i];
					i++;
				}
				index = i;
				t.setVal(isSpace);
				t.setType("DELETE");
//			cout << " token " << t.getVal() << " type " << t.getType() << endl;
				return t;
			}
			else{
//			cout << " this char is " << read[i] << " wht the f " << endl;
				string temp = temp + read[i];
				t.setVal(temp);
				t.setType("UNKNOWN");
				i++;
				index = i;
		//		cout << " token " << t.getVal() << " type " << t.getType() << endl;
				return t;
			//continue;
			}
		}
	}

	/*void print_my_tree(int dots, tree root){
			int n=0;
			while(n<dots){
				cout << ".";
				n++;
			}
			//cout << val << " and " << type << endl;
			if( root.getType() == "ID" || root.getType() == "STR" || root.getType() == "INT"){
				root.setVal("<" + root.getType() + ":"+ root.getVal()+ ">");
				//cout << "<" << type << ":" << val << ">";
			}

			cout << root.getVal() << endl;

			if(root.left != NULL){
				print_my_tree(dots+1,root.left);
			}
			if(root.left->right != NULL){
				print_my_tree(dots,root.left->right);
			}
		}*/

	void start_parsing(){

	//	while(index < sizeOfFile-1){
		nt = getToken(readnew);
		while(nt.getType()=="DELETE"){
			nt = getToken(readnew);
		}

		//cout << " first token " << nt.getVal() << " type " << nt.getType() << endl;
		//cout << " started reading file " << nt.getVal() << " type " << nt.getType() << endl;

	//}
	proc_E();
//	nt = getToken(readnew);
		while(nt.getType()=="DELETE"){
			nt = getToken(readnew);
		}
	//cout << st.top()->val << " stack " << st.top()->type << endl;
//	cout << endl;
		if(index >= sizeOfFile-1){
			tree *t = st.top();
			//cout << " tree before " << endl;

			if( astFlag==1){
				t->print_my_tree(0);
			}
			makeST(t);
			/*cout << " tree after printed " << endl;
			t->print_my_tree(0);*/

			/*vector <tree*> delta;
			//int deltaVal =0;
			//makeDelta(t, delta);
			int index = 0;
			vector< vector<tree*> > setOfDelta;*/

			/*delta.push_back( createNode("gamma", "KEYWORD"));
			delta.push_back( createNode("gamma", "KEYWORD"));
			delta.push_back( createNode("*", "OPERATOR"));
			delta.push_back( createNode("gamma", "KEYWORD"));
			delta.push_back(createNode("1", "INT"));
			delta.push_back(createNode("x", "ID"));
			delta.push_back( createNode("lambda", "KEYWORD"));
			delta.push_back(createNode("4", "INT"));
			delta.push_back(createNode("2", "INT"));
			setOfDelta.push_back(delta);
			delta.clear();
			delta.push_back( createNode("gamma", "KEYWORD"));
			delta.push_back( createNode("gamma", "KEYWORD"));
			delta.push_back(createNode("-", "OPERATOR"));
			delta.push_back(createNode("x", "ID"));
			delta.push_back(createNode("1", "INT"));
			setOfDelta.push_back(delta);
			csem(setOfDelta);*/

			/*delta.push_back( createNode("gamma", "KEYWORD"));
			delta.push_back( createNode("gamma", "KEYWORD"));
			delta.push_back( createNode("1", "INT"));
			delta.push_back( createNode("x", "ID"));
			delta.push_back( createNode("lambda", "KEYWORD"));
			delta.push_back( createNode("5", "INT"));
			delta.push_back( createNode("6", "INT"));
			setOfDelta.push_back(delta);
			delta.clear();
			delta.push_back( createNode("2", "INT"));
			delta.push_back( createNode("w", "ID"));
			delta.push_back( createNode("lambda", "KEYWORD"));
			setOfDelta.push_back(delta);
			delta.clear();
			//delta.push_back( createNode("gamma", "KEYWORD"));
			//delta.push_back( createNode("gamma", "KEYWORD"));
			delta.push_back( createNode("+", "OPERATOR"));
			delta.push_back( createNode("x", "ID"));
			delta.push_back( createNode("w", "ID"));
			setOfDelta.push_back(delta);
			csem(setOfDelta);*/

			/*delta.push_back( createNode("gamma", "KEYWORD"));
			delta.push_back( createNode("1", "KEYWORD"));
			delta.push_back( createNode("x", "ID"));
			delta.push_back( createNode("lambda", "KEYWORD"));
			delta.push_back( createNode("gamma", "KEYWORD"));
			delta.push_back( createNode("2", "KEYWORD"));
			delta.push_back( createNode("z", "ID"));
			delta.push_back( createNode("lambda", "KEYWORD"));
			delta.push_back( createNode("7", "INT"));
			setOfDelta.push_back(delta);
			delta.clear();
			//delta.push_back( createNode("gamma", "KEYWORD"));
			//delta.push_back( createNode("gamma", "KEYWORD"));
			delta.push_back( createNode("+", "OPERATOR"));
			delta.push_back( createNode("1", "INT"));
			delta.push_back( createNode("gamma", "KEYWORD"));
			delta.push_back( createNode("3", "KEYWORD"));
			delta.push_back( createNode("w", "ID"));
			delta.push_back( createNode("lambda", "KEYWORD"));
			delta.push_back( createNode("x", "ID"));
			setOfDelta.push_back(delta);
			delta.clear();
			//delta.push_back( createNode("gamma", "KEYWORD"));
			//delta.push_back( createNode("gamma", "KEYWORD"));
			delta.push_back( createNode("*", "OPERATOR"));
			delta.push_back( createNode("2", "INT"));
			delta.push_back( createNode("z", "ID"));
			setOfDelta.push_back(delta);
			delta.clear();
			//delta.push_back( createNode("gamma", "KEYWORD"));
			delta.push_back( createNode("neg", "KEYWORD"));
			delta.push_back( createNode("w", "ID"));
			setOfDelta.push_back(delta);
			csem(setOfDelta);*/

			/*delta.push_back( createNode("gamma", "KEYWORD"));
			delta.push_back( createNode("1", "KEYWORD"));
			delta.push_back( createNode("n", "ID"));
			delta.push_back( createNode("lambda", "KEYWORD"));
			delta.push_back( createNode("neg", "OPERATOR"));
			delta.push_back( createNode("3", "INT"));
			setOfDelta.push_back(delta);
			delta.clear();
			delta.push_back( createNode("2", "KEYWORD"));
			delta.push_back( createNode("3", "KEYWORD"));
			delta.push_back( createNode("beta", "beta"));
			delta.push_back( createNode("<", "OPERATOR"));
			delta.push_back( createNode("n", "ID"));
			delta.push_back( createNode("0", "INT"));
			setOfDelta.push_back(delta);
			delta.clear();
			delta.push_back( createNode("neg", "KEYWORD"));
			delta.push_back( createNode("n", "ID"));
			setOfDelta.push_back(delta);
			delta.clear();
			delta.push_back( createNode("n", "ID"));
			setOfDelta.push_back(delta);
			csem(setOfDelta);*/

			/*delta.push_back( createNode("gamma", "KEYWORD"));
			delta.push_back( createNode("1", "KEYWORD"));
			tree *coma = createNode(",", "KEYWORD");
			delta.push_back(coma);
			tree *lambda = createNode("lambda", "KEYWORD");
			lambda->left = coma;
			coma->left = createNode("x","ID");
			coma->left->right = createNode("y","ID");
			delta.push_back(lambda);
			delta.push_back( createNode("2", "CHILDCOUNT"));
			delta.push_back( createNode("tau", "tau"));
			delta.push_back( createNode("5", "INT"));
			delta.push_back( createNode("6", "INT"));
			setOfDelta.push_back(delta);
			delta.clear();
			delta.push_back( createNode("+", "OPERATOR"));
			delta.push_back( createNode("x", "ID"));
			delta.push_back( createNode("y", "ID"));
			setOfDelta.push_back(delta);
			csem(setOfDelta);*/

			/*delta.push_back( createNode("gamma", "KEYWORD"));
			delta.push_back( createNode("1", "KEYWORD"));
			delta.push_back( createNode("f", "KEYWORD"));
			delta.push_back( createNode("lambda", "KEYWORD"));
			delta.push_back( createNode("gamma", "KEYWORD"));
			delta.push_back(createNode("YSTAR","KEYWORD"));
			delta.push_back( createNode("2", "KEYWORD"));
			delta.push_back(createNode("f", "KEYWORD"));
			delta.push_back(createNode("lambda", "KEYWORD"));
			setOfDelta.push_back(delta);
			delta.clear();
			delta.push_back( createNode("gamma", "KEYWORD"));
			delta.push_back( createNode("f", "ID"));
			delta.push_back( createNode("3", "INT"));
			setOfDelta.push_back(delta);
			delta.clear();
			delta.push_back( createNode("3", "KEYWORD"));
			delta.push_back(createNode("n", "KEYWORD"));
			delta.push_back(createNode("lambda", "KEYWORD"));
			setOfDelta.push_back(delta);
			delta.clear();
			delta.push_back( createNode("4", "KEYWORD"));
			delta.push_back( createNode("5", "KEYWORD"));
			delta.push_back( createNode("beta", "beta"));
			delta.push_back( createNode("eq", "OPERATOR"));
			delta.push_back( createNode("n", "ID"));
			delta.push_back( createNode("1", "INT"));
			setOfDelta.push_back(delta);
			delta.clear();
			delta.push_back( createNode("1", "INT"));
			setOfDelta.push_back(delta);
			delta.clear();
			delta.push_back( createNode("*", "OPERATOR"));
			delta.push_back( createNode("n", "ID"));
			delta.push_back( createNode("gamma", "KEYWORD"));
			delta.push_back( createNode("f", "ID"));
			delta.push_back( createNode("-", "OPERATOR"));
			delta.push_back( createNode("n", "ID"));
			delta.push_back( createNode("1", "INT"));
			setOfDelta.push_back(delta);

			csem(setOfDelta);*/

			/*for(int i=0;i<setOfDelta.size();i++){
				vector <tree*> temp = setOfDelta.at(i);
				for(int j=0;j<temp.size();j++){
					cout << temp.at(j)->getVal() << " ";
				}
				cout << endl;

			}

			csem(setOfDelta);
*/
			vector <tree*>  delta;

			//vector< vector <tree*> > setOfDelta;
			tree* setOfDeltaArray[200][200];
			//int index =0;
			myDelta(t,setOfDeltaArray);
//			cout << " CAME OUT WITH " << position << endl;
			//setOfDeltaArray[position] = delta;
			/*cout << " my pre oder walk " << endl;
			cout << setOfDeltaArray <<  endl;*/

			//my control structures have been created

			/*for(int i=0;i<50;i++){
				for(int j=0;j<100;j++){
					if(setOfDeltaArray[i][j] != NULL ) cout << setOfDeltaArray[i][j]->getVal() << " ";
				}
				cout << endl;
			}

			cout << " ################" << endl;*/

			int size = 0;
			int inner = 0;
			while(setOfDeltaArray[size][0] != NULL){
				size++;
			}
			//cout << " size of delta is " << size <<  endl;
			//cout << endl;
			//cout << endl;

			//cout << " final delta eqn " << size;

			vector< vector<tree*> > setOfDelta;
			for(int i=0;i<size;i++){
				vector <tree*> temp;
				for(int j=0;j<200;j++){
					if(setOfDeltaArray[i][j] != NULL)temp.push_back(setOfDeltaArray[i][j]);
				}
				setOfDelta.push_back(temp);
			}

			/*cout << " ^^^^^^^^^^^^^^^^^^" << endl;

			for(int i=0;i<setOfDelta.size();i++){
				vector <tree*> delta = setOfDelta.at(i);
				for(int j=0;j<delta.size();j++){
					cout << delta.at(j)->getVal() << " ";
				}
				cout << endl;
			}

			cout << " can be send to machine " << endl;*/

			csem(setOfDelta);

			/*delta.push_back( createNode("gamma", "KEYWORD"));
			delta.push_back( createNode("Print", "ID"));
			delta.push_back( createNode("gamma", "KEYWORD"));
			delta.push_back( createNode("1", "KEYWORD"));
			delta.push_back( createNode("f", "KEYWORD"));
			delta.push_back( createNode("lambda", "KEYWORD"));
			delta.push_back( createNode("2", "KEYWORD"));
			delta.push_back(createNode("x", "KEYWORD"));
			delta.push_back(createNode("lambda", "KEYWORD"));
			setOfDelta.push_back(delta);
			delta.clear();
			delta.push_back( createNode("f", "KEYWORD"));
			setOfDelta.push_back(delta);
			delta.clear();
			delta.push_back( createNode("3", "KEYWORD"));
			delta.push_back(createNode("y", "KEYWORD"));
			delta.push_back(createNode("lambda", "KEYWORD"));
			setOfDelta.push_back(delta);
			delta.clear();
			delta.push_back( createNode("gamma", "KEYWORD"));
			delta.push_back( createNode("4", "KEYWORD"));
			delta.push_back( createNode("g", "KEYWORD"));
			delta.push_back( createNode("lambda", "KEYWORD"));
			//delta.push_back( createNode("gamma", "KEYWORD"));
			//delta.push_back(createNode("YSTAR","KEYWORD"));
			delta.push_back( createNode("5", "KEYWORD"));
			delta.push_back(createNode("x", "KEYWORD"));
			delta.push_back(createNode("lambda", "KEYWORD"));
			setOfDelta.push_back(delta);
			delta.clear();
			delta.push_back( createNode("g", "KEYWORD"));
			setOfDelta.push_back(delta);
			delta.clear();
			delta.push_back( createNode("6", "KEYWORD"));
			delta.push_back(createNode("y", "KEYWORD"));
			delta.push_back(createNode("lambda", "KEYWORD"));
			setOfDelta.push_back(delta);
			delta.clear();
			delta.push_back( createNode("gamma", "KEYWORD"));
			delta.push_back( createNode("7", "KEYWORD"));
			delta.push_back( createNode("h", "KEYWORD"));
			delta.push_back( createNode("lambda", "KEYWORD"));
			//delta.push_back( createNode("gamma", "KEYWORD"));
			//delta.push_back(createNode("YSTAR","KEYWORD"));
			delta.push_back( createNode("8", "KEYWORD"));
			delta.push_back(createNode("x", "KEYWORD"));
			delta.push_back(createNode("lambda", "KEYWORD"));
			setOfDelta.push_back(delta);
			delta.clear();
			delta.push_back( createNode("h", "KEYWORD"));
			setOfDelta.push_back(delta);
			delta.clear();
			delta.push_back( createNode("9", "KEYWORD"));
			delta.push_back(createNode("y", "KEYWORD"));
			delta.push_back(createNode("lambda", "KEYWORD"));
			setOfDelta.push_back(delta);
			delta.clear();
			delta.push_back( createNode("gamma", "KEYWORD"));
			delta.push_back( createNode("x", "ID"));
			delta.push_back( createNode("y", "ID"));
			delta.push_back( createNode("x", "ID"));
			setOfDelta.push_back(delta);
			csem(setOfDelta);*/





		}


	}

	bool isBiOper(string op){
		if(op=="+" || op == "-" || op == "*" || op == "/" || op == "**" || op == "gr" || op == "ge" || op == "<" || op == "<=" || op == ">"
		|| op == ">=" || op == "ls" || op == "le" || op == "eq" || op == "ne" || op=="&" || op == "or" || op == "><" ){
			return true;
		}

		return false;
	}

	bool is_number(const std::string& s)
	{
	    std::string::const_iterator it = s.begin();
	    while (it != s.end() && std::isdigit(*it)) ++it;
	    return !s.empty() && it == s.end();
	}

	void csem( vector < vector<tree*> > &controlStructure){
		//cout << " in csem" << endl;
		//cout << controlStructure.size() << endl;
		stack <tree*> control;
		stack <tree*> machine;
		stack <environment*> stackOfEnvironment;
		stack <environment*> getCurrEnvironment;

		int currEnvIndex = 0; //initial invironment
		environment *currEnv = new environment(); //env0;
		//cout << " initil env " << currEnv->name << endl;
		currEnvIndex++;
		machine.push(createNode(currEnv->name,"ENV"));
		control.push(createNode(currEnv->name,"ENV"));
		stackOfEnvironment.push(currEnv);
		getCurrEnvironment.push(currEnv);

		//cout << " pushed the init to stack" << endl;

		vector <tree*> tempDelta;
		tempDelta = controlStructure.at(0);
		for(int i=0;i<tempDelta.size();i++){
			control.push(tempDelta.at(i));
			//cout << " on control " <<control.top()->getVal() << endl;
		}
		//cout << " delta 0 is on stack " << endl;

		while(!control.empty()){
			tree *nextToken;
			nextToken = control.top();
			//cout << " left token " << nextToken->getType() << " with value " << nextToken->getVal() << endl;
			//cout << " right token " << machine.top()->getType() << " with value " << machine.top()->getVal() << endl;
			control.pop();

			/*if(machine.top()->getVal()=="tau"){
				cout << " right side is tuple " << endl;
				stack <tree*> res;
				printTuple(machine.top(),res);
				cout << "printed tuple " << endl;
			}*/

			if(nextToken->getVal()=="nil"){
				//cout << " setting type to tau" << endl;
				nextToken->setType("tau");
			}

			if(nextToken->getType()=="INT" || nextToken->getType()=="STR" || nextToken->getVal()=="lambda" || nextToken->getVal()=="YSTAR"
			|| nextToken->getVal()=="Print"  || nextToken->getVal()=="Isinteger" || nextToken->getVal()=="Istruthvalue"
			|| nextToken->getVal()=="Isstring" || nextToken->getVal()=="Istuple" || nextToken->getVal()=="Isfunction"
			|| nextToken->getVal() == "Isdummy" || nextToken->getVal() == "Stem" || nextToken->getVal() == "Stern"
			|| ( nextToken->getVal() == "Conc" /*&& behind->getVal()=="gamma" && behind2->getVal()=="gamma"*/) || nextToken->getType() == "BOOL" || nextToken->getType() == "NIL"
			|| nextToken->getType() == "DUMMY" || nextToken->getVal()=="Order" || nextToken->getVal()=="nil" || nextToken->getVal()=="ItoS"/*|| isBiOper(nextToken->getVal()) || nextToken->getVal() == "neg" || nextToken->getVal() == "not"*/){
				if(nextToken->getVal()=="lambda"){
					tree *boundVar = control.top();
					//cout << " bound var " << boundVar->getVal() << endl;
					control.pop();
					tree *nextDeltaIndex = control.top();
					//cout << " next delta index " << nextDeltaIndex->getVal() << endl;
					control.pop();
					tree *env = createNode(currEnv->name,"ENV");
					//cout << " binding this lambda with curr env " << env->getVal() << endl;
					machine.push(nextDeltaIndex); //index of next delta structure to open
					machine.push(boundVar); //lambda is bound to this variable
					machine.push(env); //then environment it was created in
					machine.push(nextToken); //1 : lambda
				}/*else if(nextToken->getType()=="STR" && machine.top()->getVal()=="Conc"){
					cout << " str with conc" << endl;
					machine.pop();//popping conc from machine
						tree *first = machine.top();
					if(first->getType()=="STR"){
						machine.pop();
						string res = "'" + first->getVal().substr(1,first->getVal().length()-2) + nextToken->getVal().substr(1,nextToken->getVal().length()-2) + "'";
						cout << " resulting string is " << res;
						tree *nextPush = createNode(res,"STR");
						control.push(nextPush);
					}else{
						cout << "expected string found " << first->getType() << endl;
						cout <<" @@@@@@@@@@@@ something wrong " << endl;
					}

				}*/else{
					//cout << " only pushing the value " << nextToken->getVal() << " with type " << nextToken->getType() << endl;
					machine.push(nextToken);
				}
			}else if(nextToken->getVal()=="gamma"){
				tree *machineTop = machine.top();
				if( machineTop->getVal()=="lambda"){
					//cout << " control with " << nextToken->getVal() << " machine with " << machine.top()->getVal() << endl;
					machine.pop(); //popped lambda

					tree *prevEnv = machine.top();
					//cout << " popping the evn from mchine with which lmda was created " << prevEnv->getVal() << endl;
					machine.pop(); //popped the evn in which it was created

					tree *boundVar = machine.top();
					//cout << " gettting the bound var from mchine " << boundVar->getVal() << endl;
					machine.pop(); //bound variable of lambda

					tree *nextDeltaIndex = machine.top();
					//cout << " next to put next delta at index " << nextDeltaIndex->getVal() << endl;
					machine.pop(); //next delta to be opened

					/*cout << "%%%%%%%%%%%%% NEXT ON MACHINE IS " << machine.top()->getVal() << endl;
					stack <tree*> tt = machine;
					while(!tt.empty()){
						//cout << tt.top()->getVal() << endl;
						tt.pop();
					}
					cout << "PRINTED ALL" << endl;*/

					environment *newEnv = new environment(); //env0;

					std::stringstream ss;
					ss	<< currEnvIndex;
					string str = ss.str(); //creating the string of current evn number

					newEnv->name = "env"+str;

					if(currEnvIndex>2000) return;

					stack <environment*> tempEnv = stackOfEnvironment;
					//cout << " my env stack size " << tempEnv.size() << endl;
					//cout << " prev evn should be " << prevEnv->getVal() << endl;
					//cout << " curr evn is " << currEnv->name << endl;
					while( tempEnv.top()->name != prevEnv->getVal() ){
						//cout << " finding prev " << tempEnv.top()->name << endl;
						tempEnv.pop();
					}

					//cout << " should be prev on stack " << tempEnv.top()->name << " with size " << tempEnv.size() << endl;
					newEnv->prev = tempEnv.top();

					//cout << " created new env " << newEnv->name << " with prev " << newEnv->prev->name << endl;

					if(boundVar->getVal() == "," && machine.top()->getVal()== "tau" /*&& machine.top()->getType()=="tau"*/){
						//machine.pop()
						vector <tree*> boundVariables;
						tree *leftOfComa = boundVar->left;
						while(leftOfComa != NULL){
							boundVariables.push_back(createNode(leftOfComa));
							leftOfComa = leftOfComa->right;

						}

						vector <tree*> boundValues;
						tree *tau = machine.top(); //popping the tau;
						machine.pop();

						tree *tauLeft = tau->left;
						while(tauLeft != NULL){
							//cout << tauLeft->getVal() << " fdgdfg " << endl;
							boundValues.push_back(tauLeft);
							tauLeft = tauLeft->right;
						}

						/*cout << " printing boundVariables " << endl;
						for(int i=0;i <boundVariables.size();i++){
							cout << " bound variable from comma is " <<  boundVariables.at(i)->getVal() << endl;
						}

						cout << " printing boundValues " << endl;
						for(int i=0;i<boundValues.size();i++){
							cout << " bound values are " << boundValues.at(i)->getVal() << endl;
							if(boundValues.at(i)->getVal()=="tau"){
								stack <tree*> res;
								printTuple(boundValues.at(i),res);
							}
						}

						cout << endl;*/


						for(int i=0;i<boundValues.size();i++){
							//tree *bindVarVal = machine.top(); //do not uncomment
							//cout << " inside bound values " << endl << endl;
							//cout << " bind the " << boundVariables.at(i)->getVal() << " with the value " << boundValues.at(i)->getVal()<< endl;
							if(boundValues.at(i)->getVal()=="tau"){
								stack <tree*> res;
								printTuple(boundValues.at(i),res);
							}
							//cout << endl;
							//machine.pop(); //do not uncomment
							vector <tree*> listOfNodeVal;
							listOfNodeVal.push_back(boundValues.at(i));
							newEnv->boundVar.insert(std::pair<tree*,vector<tree*> >(boundVariables.at(i),listOfNodeVal));
							//cout << " helle " << endl;
							//cout << endl << endl;
						}
					}else if(machine.top()->getVal()=="eta"){
						//cout << " bind var is " << boundVar->getVal() << endl;
						vector <tree*> listOfNodeVal;
						stack <tree*> temp;
						int j=0;
						while(j<4){
							//cout << " putting " << machine.top()->getVal() << " on bound val stack " << endl;
							temp.push(machine.top());
							machine.pop();
							j++;
						}

						while(!temp.empty()){
							tree *fromStack;
							fromStack = temp.top();
							temp.pop();
							listOfNodeVal.push_back(fromStack);
						}
						/*cout << " binding " << boundVar->getVal() << " with ";
						for(int i=0; i< listOfNodeVal.size();i++){
							cout << listOfNodeVal.at(i)->getVal() << " ";
						}
						cout << endl;*/

						newEnv->boundVar.insert(std::pair<tree*,vector<tree*> >(boundVar,listOfNodeVal));

					}else if(machine.top()->getVal()=="lambda"){
						//cout << " bind var is " << boundVar->getVal() << endl;
						vector <tree*> listOfNodeVal;
						stack <tree*> temp;
						int j=0;
						while(j<4){
							//cout << " putting " << machine.top()->getVal() << " on bound val stack " << endl;
							temp.push(machine.top());
							machine.pop();
							j++;
						}

						while(!temp.empty()){
							tree *fromStack;
							fromStack = temp.top();
							temp.pop();
							listOfNodeVal.push_back(fromStack);
						}
						/*cout << " binding " << boundVar->getVal() << " with ";
						for(int i=0; i< listOfNodeVal.size();i++){
							cout << listOfNodeVal.at(i)->getVal() << " ";
						}
						cout << endl;*/
						newEnv->boundVar.insert(std::pair<tree*,vector<tree*> >(boundVar,listOfNodeVal));

					}else if(machine.top()->getVal()=="Conc"){
						//cout << " bind var is " << boundVar->getVal() << endl;
						vector <tree*> listOfNodeVal;
						stack <tree*> temp;
						int j=0;
						while(j<2){
							//cout << " putting " << machine.top()->getVal() << " on bound val stack " << endl;
							temp.push(machine.top());
							machine.pop();
							j++;
						}

						while(!temp.empty()){
							tree *fromStack;
							fromStack = temp.top();
							temp.pop();
							listOfNodeVal.push_back(fromStack);
						}
						/*cout << " binding " << boundVar->getVal() << " with ";
						for(int i=0; i< listOfNodeVal.size();i++){
							cout << listOfNodeVal.at(i)->getVal() << " ";
						}
						cout << endl;*/
						newEnv->boundVar.insert(std::pair<tree*,vector<tree*> >(boundVar,listOfNodeVal));

					}else{
						//cout << " only 1 variable to bind " << endl;
						tree *bindVarVal = machine.top();
						//cout << " bind the " << boundVar->getVal() << " with the value " << bindVarVal->getVal()<< endl;
						machine.pop();
						vector <tree*> listOfNodeVal;
						listOfNodeVal.push_back(bindVarVal);
						newEnv->boundVar.insert(std::pair<tree*,vector<tree*> >(boundVar,listOfNodeVal));
					}
					//cout << " my print statement " << endl;
					currEnv = newEnv;
					//cout << " new curr created is " << currEnv->name << endl;
					control.push(createNode(currEnv->name, "ENV"));
					machine.push(createNode(currEnv->name, "ENV"));
					stackOfEnvironment.push(currEnv);
					getCurrEnvironment.push(currEnv);

					istringstream is3(nextDeltaIndex->getVal());
					int deltaIndex;
					is3 >> deltaIndex;

					vector <tree*> nextDelta = controlStructure.at(deltaIndex);
					for(int i=0;i< nextDelta.size();i++){
						//cout << " putting on control " << nextDelta.at(i)->getVal() << endl;
						control.push(nextDelta.at(i));
					}
					currEnvIndex++;
				}else if(machineTop->getVal()=="tau"){
					//cout << " in gamma with " << machineTop->getVal() << endl;
					tree *tau = machine.top();//saved the tau with kids on left to right;
					machine.pop();
					tree *selectChildNum = machine.top();
					machine.pop();
					//cout << " select child no " << selectChildNum->getVal() << endl;
					istringstream is4(selectChildNum->getVal());
					int childIndex;
					is4 >> childIndex;
					//cout << " in now int " << childIndex << endl;
					tree *tauLeft = tau->left;
					tree *selectedChild;
					while(childIndex>1){
						childIndex--;
						//cout << " leaving child " << tauLeft->getVal() << " with count " << childIndex << endl;
						tauLeft = tauLeft->right;
					}
					//cout << " selected child is " << tauLeft->getVal() << endl;
					selectedChild = createNode(tauLeft);
					if(selectedChild->getVal()=="lamdaTuple"){
						tree *getNode = selectedChild->left;
						//cout << " selected lamda child" << getNode->getVal() << " " << getNode->getType() << endl;
						while(getNode != NULL){
							//cout << " selected lamda pushing " << getNode->getVal() << " on machine "<< endl;
							machine.push(createNode(getNode));
							getNode = getNode->right;
						}
					}else{
						//cout << " selected child is " << selectedChild->getVal() << endl;
						machine.push(selectedChild);
					}
				}else if( machineTop->getVal()=="YSTAR"){
					//cout << nextToken->getVal() << " with " << machineTop->getVal() << endl;
					machine.pop(); //pop the YSTAR
					if(machine.top()->getVal()=="lambda"){
						//machine.top()->setVal("eta");
						///look here
						//cout << " with " << nextToken->getVal() << " and " << machine.top()->getVal() << endl;
						tree *etaNode = createNode(machine.top()->getVal(),machine.top()->getType()); //getting eta
						etaNode->setVal("eta");
						machine.pop();
						tree *boundEvn1 = machine.top(); //getting bound evn
						machine.pop();
						tree *boundVar1 = machine.top(); //getting bound var
						machine.pop();
						tree *deltaIndex1 = machine.top(); //getting delta index
						machine.pop();

						machine.push(deltaIndex1); //pushing eta node 1
						machine.push(boundVar1); // 2
						machine.push(boundEvn1); // 3
						machine.push(etaNode); //eta completed 4
						//cout << " eta is put on machine " << machine.top()->getVal() << endl;
						//look here

					}else{
						cout <<" PROBLEM WITH RECURSSION ******** " << machine.top()->getVal() << endl;
						return;
					}
				}else if( machineTop->getVal()=="eta"){
					//cout << " with " << nextToken->getVal() << " and " << machine.top()->getVal() << endl;
					//2 f env0 eta
					tree *eta = machine.top(); //getting eta
					machine.pop();
					tree *boundEvn1 = machine.top(); //getting bound evn
					machine.pop();
					tree *boundVar1 = machine.top(); //getting bound var
					machine.pop();
					tree *deltaIndex1 = machine.top(); //getting delta index
					machine.pop();

					machine.push(deltaIndex1); //pushing eta node 1
					machine.push(boundVar1); // 2
					machine.push(boundEvn1); // 3
					machine.push(eta); //eta completed 4
					//cout << " eta is put back on machine " << machine.top()->getVal() << endl;

					//eta->setVal("lambda"); //preparing lambda
					machine.push(deltaIndex1);// 1
					//cout << " putting " << machine.top()->getVal() << endl;
					machine.push(boundVar1); //2
					//cout << " putting " << machine.top()->getVal() << endl;
					machine.push(boundEvn1); //3
					//cout << " putting " << machine.top()->getVal() << endl;
					machine.push(createNode("lambda","KEYWORD"));//4
					//cout << " putting " << machine.top()->getVal() << endl;
					control.push(createNode("gamma","KEYWORD"));
					control.push(createNode("gamma","KEYWORD"));

					//cout <<  " ended with "<< control.top()->getVal() << " on top of control " << machine.top()->getVal() << " on machine " << endl;
				}else if(machineTop->getVal()=="Print"){
					//cout << nextToken->getVal() << " with " << machineTop->getVal() << endl;
					machine.pop();
					tree *nextToPrint = machine.top();
					if(nextToPrint->getVal()=="lambda"){
						//cout << " in lambda closure " << endl;
						machine.pop();
						tree *env = machine.top();
						machine.pop();
						tree *boundVar = machine.top();
						machine.pop();
						tree *num = machine.top();
						machine.pop();
						cout << "[lambda closure: " << boundVar->getVal() << ": "<< num->getVal() << "]";
						return;
					}else if(nextToPrint->getVal()=="tau"){
						//cout << "print with tau " << endl;
						tree *getTau = machine.top();
						stack<tree*> res;
						printTuple(getTau,res);
						stack<tree*> getRev;
						while(!res.empty()){
							//cout << res.top()->getVal() << endl;
							getRev.push(res.top());
							res.pop();
						}
						//cout << endl;
						cout << "(";
						while(getRev.size()>1){
							if(getRev.top()->getType()=="STR" )cout << addSpaces(getRev.top()->getVal()) << ", ";
							else cout << getRev.top()->getVal()<< ", ";
							getRev.pop();
						}
						//cout << getRev.size() << " no of element left " << endl;
						if(getRev.top()->getType()=="STR" )cout << addSpaces(getRev.top()->getVal()) << ")";
						else cout << getRev.top()->getVal()<< ")";
						getRev.pop();
						//return;

					}else{
						/*while(!machine.empty()){
							cout << " in else " << machine.top()->getVal() << " with type " << machine.top()->getType() << endl;
							machine.pop();
						}*/
						//cout << " next to print is " << nextToPrint->getVal() << endl;
						//if(machine.top()->getType()=="STR" || machine.top()->getVal()=="' '") machine.push(createNode("dummy","dummy"));
						if(machine.top()->getType()=="STR" /*|| machine.top()->getVal()!="' '"*/) cout << addSpaces(machine.top()->getVal());
						else cout << machine.top()->getVal();
						//machine.pop();
						//return;
					}
				}else if(machineTop->getVal()=="Isinteger"){
					//cout << " inside " << nextToken->getVal() << " with " << machineTop->getVal() << endl;
					machine.pop();
					tree *isNextInt = machine.top();
					machine.pop();
					//cout << " 1.  next is " << isNextInt->getVal() << " and type " << isNextInt->getType() << endl;
					if(isNextInt->getType()=="INT"){
						//cout << " pushing true" << endl;
						tree *resNode = createNode("true","boolean");
						machine.push(resNode);
					}else{
						//cout << "pushing false" << endl;
						tree *resNode = createNode("false","boolean");
						machine.push(resNode);
					}
				}else if(machineTop->getVal()=="Istruthvalue"){
					//cout << " inside " << nextToken->getVal() << " with " << machineTop->getVal() << endl;
					machine.pop();
					tree *isNextBool = machine.top();
					machine.pop(); //poppoedxfg
					//cout << " 2. next is " << isNextBool->getVal() << " and type " << isNextBool->getType() << endl;
					if(isNextBool->getVal()=="true" || isNextBool->getVal()=="false"){
						//cout << " pushing true" << endl;
						tree *resNode = createNode("true","BOOL");
						machine.push(resNode);
					}else{
						//cout << "pushing false" << endl;
						tree *resNode = createNode("false","BOOL");
						machine.push(resNode);
					}
				}else if(machineTop->getVal()=="Isstring"){
					//cout << " inside " << nextToken->getVal() << " with " << machineTop->getVal() << endl;
					machine.pop();
					tree *isNextString = machine.top();
					//cout << " 3. next is " << isNextString->getVal() << " and type " << isNextString->getType() << endl;
					machine.pop();
					if(isNextString->getType()=="STR"){
						//cout << " pushing true" << endl;
						tree *resNode = createNode("true","BOOL");
						machine.push(resNode);
					}else{
						//cout << "pushing false" << endl;
						tree *resNode = createNode("false","BOOL");
						machine.push(resNode);
					}
				}else if(machineTop->getVal()=="Istuple"){
					//cout << " inside " << nextToken->getVal() << " with " << machineTop->getVal() << endl;
					machine.pop();
					tree *isNextTau = machine.top();
					machine.pop();
					//cout << " 4. next is " << isNextTau->getVal() << " and type " << isNextTau->getType() << endl;
					if(isNextTau->getType()=="tau"){
						//cout << " pushing true" << endl;
						tree *resNode = createNode("true","BOOL");
						machine.push(resNode);
					}else{
						//cout << "pushing false" << endl;
						tree *resNode = createNode("false","BOOL");
						machine.push(resNode);
					}
				}else if(machineTop->getVal()=="Isfunction"){
					//cout << " inside " << nextToken->getVal() << " with " << machineTop->getVal() << endl;
					machine.pop();
					tree *isNextFn = machine.top();
					//cout << " 5. next is " << isNextFn->getVal() << " and type " << isNextFn->getType() << endl;
					if(isNextFn->getVal()=="lambda"){
						//cout << " pushing true" << endl;
						tree *resNode = createNode("true","BOOL");
						machine.push(resNode);
					}else{
						//cout << "pushing false" << endl;
						tree *resNode = createNode("false","BOOL");
						machine.push(resNode);
					}
				}else if(machineTop->getVal()=="Isdummy"){
					//cout << " inside " << nextToken->getVal() << " with " << machineTop->getVal() << endl;
					machine.pop();
					tree *isNextDmy = machine.top();
					//cout << " 6. next is " << isNextDmy->getVal() << " and type " << isNextDmy->getType() << endl;
					if(isNextDmy->getVal()=="dummy"){
						//cout << " pushing true" << endl;
						tree *resNode = createNode("true","BOOL");
						machine.push(resNode);
					}else{
						//cout << "pushing false" << endl;
						tree *resNode = createNode("false","BOOL");
						machine.push(resNode);
					}
				}else if(machineTop->getVal()=="Stern"){
					//cout << " inside " << nextToken->getVal() << " with " << machineTop->getVal() << endl;
					machine.pop();
					tree *isNextString = machine.top();

					//cout << " 7. next is " << isNextString->getVal() << " and type " << isNextString->getType() << endl;

					if(isNextString->getVal()==""){
						//isNextString->setVal("''");
						cout << " empty string" << endl;
								return;
					}

					if(isNextString->getType()=="STR"){
						//int ind = isNextString->getVal().length()-3;
						string strRes = "'"+ isNextString->getVal().substr(2,isNextString->getVal().length()-3) + "'";
						//cout << " after Stern " << strRes << endl;
						machine.pop();
						machine.push(createNode(strRes,"STR"));
					}else{
						//cout << " i don't know what to do !!!!! with " <<  isNextString->getType() << " val " << isNextString->getVal() << endl;
					}
				}else if(machineTop->getVal()=="Stem"){
					//cout << " inside " << nextToken->getVal() << " with " << machineTop->getVal() << endl;
					//cout << machine.top()->getVal() << endl;
					machine.pop();
					tree *isNextString = machine.top();

					//cout << " 8. next is " << isNextString->getVal() << " and type " << isNextString->getType() << endl;

					if(isNextString->getVal()==""){
						//isNextString->setVal("''");
						cout << " empty string" << endl;
								return;
					}

					if(isNextString->getType()=="STR"){
						string strRes = "'" + isNextString->getVal().substr(1,1) +  "'" ;
						//cout << " after Stem " << strRes << endl;
						machine.pop();
						machine.push(createNode(strRes,"STR"));
					}else{
						//cout << " i don't know what to do !!!!! with " <<  isNextString->getType() << " val " << isNextString->getVal() << endl;
					}
				}else if(machineTop->getVal()=="Order"){
					//cout << " in gamma with Order" << endl;
					machine.pop();

					tree *getTau = machine.top();
					tree *saveTau = getTau;
					//cout << " 10. next is tua " << getTau->getVal() << endl;
					int numOfKids = 0;
					//cout <<getTau->left->right->getVal() << endl;
					if(getTau->left != NULL) getTau = getTau->left;
					//cout << " hello " << endl;
					while(getTau != NULL){
						numOfKids++;
						//cout <<  getTau->getVal() << ", ";
						getTau = getTau->right;
					}
					getTau = machine.top();
					machine.pop();

					if(( getTau->getVal()=="nil")){
						//cout << " ONLY 1 KID which is nil" << endl;
						numOfKids = 0;
					}

					//cout << " oder is " << numOfKids;
					stringstream ss11;
					ss11 << numOfKids;
					string str34 = ss11.str();
					tree *orderNode = createNode(str34,"INT");
					machine.push(orderNode);
					//cout << " helllo poddsfs" << endl;

				}else if(machineTop->getVal()=="Conc"){
					//cout << " inside " << nextToken->getVal() << " with " << machineTop->getVal() << endl;
					tree *concNode = machine.top();
					machine.pop();
					tree *firstString = machine.top();
					//cout << " string one " << firstString->getVal().substr(1,firstString->getVal().length()-2)<< "with type " << firstString->getType() << endl;
					machine.pop();
					tree *secondString = machine.top();
					//cout << " second string " << secondString->getVal().substr(1,secondString->getVal().length()-2) << " with type " << secondString->getType() << endl;
					if(secondString->getType()=="STR" || (secondString->getType()=="STR" && secondString->left != NULL && secondString->left->getVal()=="true")){
						machine.pop();
						string res = "'" + firstString->getVal().substr(1,firstString->getVal().length()-2) + secondString->getVal().substr(1,secondString->getVal().length()-2)+ "'";
						//cout << " result " << res << endl;
						tree *resNode = createNode(res,"STR");
						machine.push(resNode);
						//pop gamma from control
						control.pop();
					}else{
						//cout << " **************** ONLY ONE STRING ************************" << endl;
						concNode->left = firstString;
						//machine.push(firstString);
						machine.push(concNode);
						//cout <<  machine.top()->getVal() << " ON TOP OF MACHINE" << endl;
						//cout << " applying Conc to only 1 string added a left kid flag and keeping conc " << endl;
						firstString->left = createNode("true","flag");
					}
				}else if(machineTop->getVal()=="ItoS"){
					//cout << " inside " << nextToken->getVal() << " with " << machineTop->getVal() << endl;
					machine.pop(); //popping ItoS
					tree *convertToString = machine.top();
					//cout << " convert " << convertToString->getVal() << " with " << convertToString->getType() << " to str " << endl;
					machine.pop();
					machine.push(createNode("'"+convertToString->getVal()+",","STR"));
				}

				}else if(nextToken->getVal().substr(0,3)=="env"){
					//cout << " killing env " << nextToken->getVal() << endl;
					stack <tree*> removeFromMachineToPutBack;
					/*while(machine.top()->getType() != "ENV"){
						//tree *temp = machine.top();
						cout << " next token in mchine " << machine.top()->getVal() << " with type " << machine.top()->getType() << endl;
						removeFromMachineToPutBack.push(machine.top());
						machine.pop();
					}*/
					if(machine.top()->getVal()== "lambda"){
						removeFromMachineToPutBack.push(machine.top());
						machine.pop();
						removeFromMachineToPutBack.push(machine.top());
						machine.pop();
						removeFromMachineToPutBack.push(machine.top());
						machine.pop();
						removeFromMachineToPutBack.push(machine.top());
						machine.pop();
					}else{
						//cout << " in else putting " << endl;
						removeFromMachineToPutBack.push(machine.top());
						machine.pop();
						//cout << removeFromMachineToPutBack.top()->getVal() << " to the temp stack " << endl;
					}
					//cout << " KILL IT" << endl;
					//tree *topToken = machine.top();
					//cout << " on machine top " << machine.top()->getVal() << endl;
					//machine.pop();
					tree *remEnv = machine.top();
					//cout << " expecting env " << machine.top()->getVal() << endl;
					//machine.pop();
					if(nextToken->getVal() == remEnv->getVal() ){
						//control.pop();
						//cout << "killing off " << nextToken->getVal() << " and " << remEnv->getVal() << endl;
						machine.pop();
						//cout << "killed off " << nextToken->getVal() << " and " << remEnv->getVal() << endl;

						//cout << " killing from stack of env " << stackOfEnvironment.top()->name << endl;
						//stackOfEnvironment.pop();

						stack <tree*> printMachine = machine;
						/*cout << " left on machine $$$$$$$$$$" << endl;
						while( !printMachine.empty()){
							cout << printMachine.top()->getVal() << endl;;
							printMachine.pop();
						}*/
						//cout << " all this is left on machine $$$$$" << endl;

						getCurrEnvironment.pop();
						if(!getCurrEnvironment.empty()){
							currEnv = getCurrEnvironment.top();
							//cout << getCurrEnvironment.top()->name << endl;
							//cout << " new curr env is " << currEnv->name << endl;
						}else{
							currEnv = NULL;
							//cout << " current env is NULL ************" << endl;
						}

					}else{
						cout << "error evn do not match";
						return;
					}

					//cout << " to put back " << removeFromMachineToPutBack.size() << endl;

					while(!removeFromMachineToPutBack.empty()){
						//cout << " putting back " <<  removeFromMachineToPutBack.top()->getVal() <<endl;
						machine.push(removeFromMachineToPutBack.top());
						removeFromMachineToPutBack.pop();
					}
					 //pushing the token back on machine
				}else if(nextToken->getType()=="ID" && nextToken->getVal() !="Print"  && nextToken->getVal() !="Isinteger"
						&& nextToken->getVal()!="Istruthvalue" && nextToken->getVal() !="Isstring" && nextToken->getVal() !="Istuple"
						&& nextToken->getVal()!="Isfunction" && nextToken->getVal() != "Isdummy" && nextToken->getVal() != "Stem"
						&& nextToken->getVal() != "Stern" && nextToken->getVal() != "Conc"){
					//cout << " in else with next " << nextToken << endl;
					environment *temp = currEnv;
					 int flag =0;
					while(temp != NULL){

						 //cout <<" key is " << nextToken->getVal() << endl;

					 map<tree*,vector<tree*> >::iterator itr = temp->boundVar.begin();
					 while(itr != temp->boundVar.end()){
					 		 //cout << " inside evn name " << temp->name << " prev " << temp->prev->name << endl;
					 		 if( nextToken->getVal()== itr->first->getVal()){
					 			//cout << " value of " << itr->first->getVal() << " is of size "<< itr->second.size() << " FOUND%%%%%%%%" << endl;
					 			 vector<tree*> temp = itr->second;
					 			 if(temp.size()==1 && temp.at(0)->getVal()=="Conc" && temp.at(0)->left != NULL){
					 				 //cout << " BOUND VAL IS CONC ************* %%%%%%%%% ************"<< endl;
					 				 control.push(createNode("gamma","KEYWORD"));
					 				 machine.push(temp.at(0)->left);
					 				 machine.push(temp.at(0));
					 			 }else{
					 				 int i = 0;
					 				 while(i< temp.size() ){
					 					 //cout << " bound val is " << temp.at(i)->getVal() << endl;
					 					 if(temp.at(i)->getVal()=="lamdaTuple"){
					 						 tree *myLambda = temp.at(i)->left;
					 						 while(myLambda != NULL){
					 							 //cout << " my bind var is " << myLambda->getVal() << endl;
					 							 machine.push(createNode(myLambda));
					 							 myLambda = myLambda->right;
					 						 }
					 					 }else{
					 						 if(temp.at(i)->getVal()=="tau"){
					 							 //cout << " bound tuple " << endl;
					 							 stack <tree*> res;
					 							 printTuple(temp.at(i),res);
					 						 }
					 						machine.push(temp.at(i));
					 					 }
					 					 i++;
					 				 }
					 			 }
					 			//cout << " FIND Value %%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
					 			 flag =1;
					 			 break;
					 		 }
					 		 itr++;
					 	 }
					 	 if(flag==1) break;
					 	 temp = temp->prev;
					 }
					if(flag==0){
						//cout << " DID NOT FIND %%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
						//cout << " no variable to bound so pushing to machine might be part of tuple " << endl;
						//machine.push(nextToken); //do not uncomment
						return;
					}
					 //cout << " new op " << op << endl;
				}else if( isBiOper(nextToken->getVal())|| nextToken->getVal() == "neg" || nextToken->getVal() == "not"){
					//cout << " is is binary op " << nextToken->getVal() << endl;
					string op = nextToken->getVal();
					//cout << " binary op " << op << endl;
					//cout << machine.top()->getVal() << " on machine" << endl;
					//machine.pop();
					if( isBiOper(nextToken->getVal()) ){
						tree *node1 = machine.top();
						machine.pop();
						tree *node2 = machine.top();
						machine.pop();
						if(node1->getType()=="INT" && node2->getType()=="INT"){
							int num1;
							int num2;

						//	cout << " INTEGERS " << endl;
							istringstream is1(node1->getVal());
							is1 >> num1;
						//	cout << " num1 is " << num1 << endl;
							istringstream is2(node2->getVal());
							is2 >> num2;
						//	cout << " num2 is " << num2 << endl;

							int res = 0;
							double resPow;
							if(op=="+"){
								res = num1+num2;
								stringstream ss;
								ss << res;
								string str = ss.str();
								tree *res = createNode(str,"INT");
							//	cout << " res is " << res->getVal() << endl;
								machine.push(res);
							}else if(op=="-"){
								//cout << " in minus" << endl;
								res = num1-num2;
								stringstream ss;
								ss << res;
								string str = ss.str();
								tree *res = createNode(str,"INT");
								//cout << " res is " << res->getVal() << endl;
								machine.push(res);
							}else if(op=="*"){
								res = num1*num2;
								stringstream ss;
								ss << res;
								string str = ss.str();
								tree *res = createNode(str,"INT");
								machine.push(res);
							}else if(op=="/"){
								if(num2 == 0) cout << " divide by zero " << endl;
								res = num1/num2;
								stringstream ss;
								ss << res;
								string str = ss.str();
								tree *res = createNode(str,"INT");
								machine.push(res);
							}else if(op=="**"){
								resPow = pow((double)num1,(double)num2);
								stringstream ss;
								ss << res;
								string str = ss.str();
								tree *res = createNode(str,"INT");
								machine.push(res);
							}else if(op=="gr" || op == ">"){
								//resPow = pow((double)num1,(double)num2);
								string resStr = num1>num2?"true":"false";
								tree *res = createNode(resStr,"bool");
								//cout << " res is " + resStr << endl;
								machine.push(res);
							}else if(op=="ge" || op == ">="){
								//resPow = pow((double)num1,(double)num2);
								string resStr = num1>=num2?"true":"false";
								tree *res = createNode(resStr,"bool");
								machine.push(res);
							}else if(op=="ls" || op == "<"){
								//resPow = pow((double)num1,(double)num2);
								//cout << " in less than " << endl;
								string resStr = num1<num2?"true":"false";
								tree *res = createNode(resStr,"bool");
								machine.push(res);
							}else if(op=="le" || op == "<="){
								//resPow = pow((double)num1,(double)num2);
								string resStr = num1<=num2?"true":"false";
								tree *res = createNode(resStr,"bool");
								machine.push(res);
							}else if(op=="eq" || op == "="){
								//resPow = pow((double)num1,(double)num2);
								string resStr = num1==num2?"true":"false";
								tree *res = createNode(resStr,"bool");
								machine.push(res);
							}else if(op=="ne" || op == "><"){
								//resPow = pow((double)num1,(double)num2);
								string resStr = num1 != num2?"true":"false";
								tree *res = createNode(resStr,"bool");
								machine.push(res);
							}else{
								//cout <<" op is not recognised " << op << endl;
								//control.pop();
								//cout << " added res to machine " << machine.top() << endl;
							}
						//pop extra gamma
						//control.pop();
						///cout << "popped extra gamma " << endl;
						}else if(node1->getType()=="STR" && node2->getType()=="STR"){
							//cout << " INTEGERS " << endl;
							if(op=="ne" || op == "<>"){
								//resPow = pow((double)num1,(double)num2);
								string resStr = node1->getVal() != node2->getVal() ?"true":"false";
								tree *res = createNode(resStr,"BOOL");
								//cout << "pusing " << resStr << " to machine " << endl;
								machine.push(res);
							}else if(op=="eq" || op == "=="){
								//resPow = pow((double)num1,(double)num2);
								string resStr = node1->getVal() == node2->getVal() ?"true":"false";
								//cout << "pusing " << resStr << " to machine " << endl;
								tree *res = createNode(resStr,"BOOL");
								machine.push(res);
							}else{
								//cout <<" op is not recognised " << op << endl;
								//control.pop();
								//cout << " added res to machine " << machine.top() << endl;
							}
						}else if( (node1->getVal()=="true" || node1->getVal()=="false") && ( node2->getVal()=="false" || node2->getVal()=="true") ){
							if(op=="ne" || op == "<>"){
								//resPow = pow((double)num1,(double)num2);
								string resStr = node1->getVal() != node2->getVal() ?"true":"false";
								tree *res = createNode(resStr,"BOOL");
								//cout << "pusing " << resStr << " to machine " << endl;
								machine.push(res);
							}else if(op=="eq" || op == "=="){
								//resPow = pow((double)num1,(double)num2);
								string resStr = node1->getVal() == node2->getVal() ?"true":"false";
								tree *res = createNode(resStr,"BOOL");
								//cout << "pusing " << resStr << " to machine " << endl;
								machine.push(res);
							}else if(op=="or"){
								//resPow = pow((double)num1,(double)num2);
								//cout << node1->getVal() << "  " << node2->getVal() << " on machine" << endl;
								string resStr;
								if(node1->getVal()=="true" || node2->getVal()== "true"){
									resStr = "true";
									tree *res = createNode(resStr,"BOOL");
									//cout << "pusing " << resStr << " to machine " << endl;
									machine.push(res);
								}else{
									resStr = "false";
									tree *res = createNode(resStr,"BOOL");
									//cout << "pusing " << resStr << " to machine " << endl;
									machine.push(res);
								}
							}else if(op=="&"){
								//resPow = pow((double)num1,(double)num2);
								//cout << node1->getVal() << " " << node2->getVal() << endl;
								string resStr;
								if(node1->getVal()=="true" && node2->getVal()== "true"){
									resStr = "true";
									tree *res = createNode(resStr,"BOOL");
									machine.push(res);
								}else{
									resStr = "false";
									tree *res = createNode(resStr,"BOOL");
									machine.push(res);
								}
							}else{
								//cout <<" op is not recognised " << op << endl;
								//control.pop();
								//cout << " added res to machine " << machine.top() << endl;
							}
						}

					}else if(op == "neg" || op == "not"){
						//cout << " found unary op " << op << endl;
						if(op == "neg"){
							//cout << " neg " << endl;
							tree *node1 = machine.top();
							//cout << node1->getVal();
							machine.pop();
							istringstream is1(node1->getVal());
							int num1;
							is1 >> num1;
							//cout << " num1 is " << num1 << endl;
							int res = -num1;
							stringstream ss;
							ss << res;
							string str = ss.str();
							//cout << " res is " << res << endl;
							tree *resStr = createNode(str,"INT");
							machine.push(resStr);
						}else if(op=="not" && ( machine.top()->getVal()== "true" || machine.top()->getVal()== "false") ){
							//cout << " next token is " << machine.top()->getVal() << " with type " << machine.top()->getType() << endl;
							if(machine.top()->getVal()=="true"){
								machine.pop();
								machine.push(createNode("false","BOOL"));
							}else{
								machine.pop();
								machine.push(createNode("true","BOOL"));
							}
						}
					}else{
						cout << " WTF op " << nextToken->getVal() << endl;
					}
				}else if(nextToken->getVal()=="beta"){
					tree *truthVal = machine.top();
					machine.pop();
					//cout << " found " << nextToken->getVal() << " with "<< truthVal->getVal() << " on machine" << endl;
					tree *elseIndex = control.top();
					control.pop();
					tree *thenIndex = control.top();
					control.pop();

					int index;
					if(truthVal->getVal()=="true"){
						istringstream is1(thenIndex->getVal());
						is1 >> index;
						//cout << " next index is then condition " << index << endl;
					}else{
						istringstream is1(elseIndex->getVal());
						is1 >> index;
						//cout << " next index is else condition " << index << endl;
					}
					vector <tree*> nextDelta = controlStructure.at(index);
					for(int i=0;i< nextDelta.size();i++){
						//cout << " putting on control " << nextDelta.at(i)->getVal() << endl;
						control.push(nextDelta.at(i));
					}
					//int elseID;
				}else if(nextToken->getVal()=="tau" /*&& nextToken->getType()=="tau"*/){
					//cout << " type " << nextToken->getType() << endl;
					tree *myTuple = createNode("tau","tau");

					tree *noOfChild = control.top(); //behind tua is the no of kids of tau
					control.pop();

					int numOfKids;

					istringstream is1(noOfChild->getVal());
					is1 >> numOfKids;

					//cout << " remove next " << numOfKids << " from machine " << endl;
					if(machine.top()->getVal()=="lambda"){
						//cout << " my kid is lamda " << machine.top()->getVal() << endl;
						tree *lamda = createNode(machine.top()->getVal(), machine.top()->getType()); //popped lambda
						//cout << " popped lambda " << lamda->getVal() << endl;
						machine.pop();

						tree *prevEnv = createNode(machine.top()->getVal(), machine.top()->getType());
						//cout << " popping the evn from mchine with which lmda was created " << prevEnv->getVal() << endl;
						machine.pop(); //popped the evn in which it was created

						tree *boundVar = createNode(machine.top()->getVal(), machine.top()->getType());
						//cout << " gettting the bound var from mchine " << boundVar->getVal() << endl;
						machine.pop(); //bound variable of lambda

						tree *nextDeltaIndex = createNode(machine.top()->getVal(), machine.top()->getType());
						//cout << " next to put next delta at index " << nextDeltaIndex->getVal() << endl;
						machine.pop(); //next delta to be opened

						tree *myLambda = createNode("lamdaTuple","lamdaTuple");
						myLambda->left = nextDeltaIndex;
						nextDeltaIndex->right = boundVar;
						boundVar->right = prevEnv;
						prevEnv->right = lamda;
						myTuple->left = myLambda;

					}else{
						//cout << " not lamda node " << machine.top()->getVal() << endl;
						myTuple->left = createNode(machine.top()); //removing right link
						//cout << " first kid of tau is " << machine.top()->getVal() << endl;
						machine.pop();

					}


					tree *sibling = myTuple->left;

					 for(int i=1;i<numOfKids;i++){
						 //cout << " num of kids left " << numOfKids-i << endl;
						 //cout << " attaching myself to my sibling " << sibling->getVal() << " " << sibling->getType() << endl;
						 //cout << " machine top for tuple " << machine.top()->getVal() << endl;
							 tree *temp = machine.top();
							 if(temp->getVal()=="lambda"){
								 //cout << " kid is lambda " << endl;
								 tree *lamda = createNode(machine.top()->getVal(), machine.top()->getType()); //popped lambda
								 //cout << " popped lamda " << lamda->getVal() << endl;
								 machine.pop();

								 tree *prevEnv = createNode(machine.top()->getVal(), machine.top()->getType());
								 //cout << " popping the evn from mchine with which lmda was created " << prevEnv->getVal() << endl;
								 machine.pop(); //popped the evn in which it was created

								 tree *boundVar = createNode(machine.top()->getVal(), machine.top()->getType());
								 //cout << " gettting the bound var from mchine " << boundVar->getVal() << endl;
								 machine.pop(); //bound variable of lambda

								 tree *nextDeltaIndex = createNode(machine.top()->getVal(), machine.top()->getType());
								 //cout << " next to put next delta at index " << nextDeltaIndex->getVal() << endl;
								 machine.pop(); //next delta to be opened

								 tree *myLambda = createNode("lamdaTuple","lamdaTuple");

								 myLambda->left = nextDeltaIndex;
								 nextDeltaIndex->right = boundVar;
								 boundVar->right = prevEnv;
								 prevEnv->right = lamda;
								 sibling->right = myLambda;
								 sibling = sibling->right;

							 }else{
								 machine.pop();
								 sibling->right = temp;
								 //cout << " kid of tau " << sibling->right->getVal() << endl;
								 sibling = sibling->right;
							 }

							/* if(temp->left != NULL){
								 cout << " my left is not null " << temp->left->getVal() << endl;
								 if(temp->left->right != NULL) cout << " my sibling is not null " << temp->left->right->getVal() << endl;
							 }*/

					 }
					/*cout << " machine is having " << machine.top()->getVal() << endl;
					cout << " lets print the tuple " << endl;
					stack <tree*> tempSt;
					printTuple(myTuple,tempSt);
					cout <<" lo ho gyi print " << endl;
					//machine.push(temp);*/
					machine.push(myTuple); //after the kids putting the tau on machine
				}else if(nextToken->getVal()=="aug"){
						//cout << " HLLOOOO FROM the other side " << endl;
						tree *tokenOne = createNode(machine.top());
						machine.pop();
						tree *tokenTwo = createNode(machine.top());
						machine.pop();
						//cout << tokenOne->getType() << " and " << tokenTwo->getType() << endl;
						if(tokenOne->getVal()=="nil" && tokenTwo->getVal()=="nil"){
								//cout << " BOTH NIL" << endl;
								tree *myNilTuple = createNode("tau","tau");
								myNilTuple->left = tokenOne;
								machine.push(myNilTuple);

						}else if( tokenOne->getVal()=="nil"){
							//cout << " token 1 is nil " << endl;
							tree *myNilTuple = createNode("tau","tau");
							myNilTuple->left = tokenTwo;
							machine.push(myNilTuple);

						}else if(tokenTwo->getVal() =="nil"){
							//cout << " token two is nil " << endl;
							tree *myNilTuple = createNode("tau","tau");
							myNilTuple->left = tokenOne;
							machine.push(myNilTuple);

						}else if(tokenOne->getType() !="tau"){
							//cout << " tokenOne is not tau " << endl;
							tree *myTuple = tokenTwo->left;
							while(myTuple->right != NULL){
								myTuple = myTuple->right;
							}
							myTuple->right = createNode(tokenOne);
							machine.push(tokenTwo);

						}else if(tokenTwo->getType() !="tau"){
							//cout << " tokenTwo is not tau " << endl;
							tree *myTuple = tokenOne->left;
							while(myTuple->right != NULL) {
								myTuple = myTuple->right;
							}
							myTuple->right = createNode(tokenTwo);
							machine.push(tokenOne);
						}else{
							//cout << " both tuple " << endl;
							tree *myNilTuple = createNode("tau","tau");
							myNilTuple->left = tokenOne;
							myNilTuple->left->right = tokenTwo;
							machine.push(myNilTuple);
						}
				}

				/*cout << " PRINTING MY MACHINE !!!!!" << endl;
				stack <tree*> printMyMachine = machine;
				while(!printMyMachine.empty()){
					cout << printMyMachine.top()->getVal() << endl;
					printMyMachine.pop();
				}
				cout << " PRINTED " << endl;*/
		}

		cout << endl;
		//cout << " control is empty now expression is evaluated" << endl;
		//cout << " output is " << machine.top()->getVal() << endl;
		/*if(machine.top()->getVal()=="tau"){
			stack <tree*> stt;
			printTuple(machine.top(),stt);
			cout << " endddddd " << endl;

			tree *tauLeft = machine.top();
			cout << tauLeft->left->getVal() << " " << tauLeft->left->right->getVal()<< " " << tauLeft->left->right->getVal()<< " " << tauLeft->left->right->right->getVal() << endl;
			tauLeft = tauLeft->left;
			cout << tauLeft->left->getVal() << " " << tauLeft->left->right->getVal()<< " " << tauLeft->left->right->getVal() << endl;
			tauLeft = tauLeft->left->left->left;
			cout << tauLeft->left->getVal() << " " << tauLeft->left->right->getVal()<< " " << tauLeft->left->right->getVal() << endl;
			cout << " dsfds" << endl;

			//tauLeft = tauLeft->left->left->left->left->left->left->left->left;
			int i=0;
			tauLeft = tauLeft->left;
			while(tauLeft != NULL ){
				cout << i << "  "  << tauLeft->getVal() << " ";
				tauLeft = tauLeft->right;
				i++;
			}

			cout << endl;
			cout << machine.top()->left->right->getVal();
		} */
		//controlStructure.erase(controlStructure.begin());
		//controlStructure.pop_back();
	}

	void printTuple(tree *tauNode, stack<tree*> &res){

		//cout <<  " aaja " << endl;
		if(tauNode == NULL) return;
		if(tauNode->getVal()=="lamdaTuple") return;
		//cout << "(";
			//while(tauNode != NULL){
			if(tauNode->getVal() != "tau" && tauNode->getVal() != "nil" ){
				//cout << " hello " << endl;
				//if(tauNode->getType()=="STR") cout <<  tauNode->getVal().substr(1,tauNode->getVal().length()-2) << ", ";
				//else cout << tauNode->getVal() << ", ";
				//cout << " bye " << endl;
				res.push(tauNode);
			}
			//cout << tauNode->getVal() << endl;
			//}
			//	cout << ")";
		printTuple(tauNode->left,res);
		printTuple(tauNode->right,res);
	}

	string addSpaces(string temp){
		for(int i=1;i<temp.length();i++){
			if(temp[i-1]==92 && (temp[i] == 'n') ){
				temp.replace(i-1,2,"\\\n");
			}if(temp[i-1]==92 && (temp[i] == 't') ){
				temp.replace(i-1,2,"\\\t");
			}
		}

		for(int i=0;i<temp.length();i++){
			if(temp[i]==92 || temp[i]== 39 ){
				temp.replace(i,1,"");
			}
		}
		//cout << temp ;
		return temp;
	}


	void myDelta(tree* x,tree* (*setOfDelta)[200]){
		static int index = 1;
		static int j =0;
		static int i =0;
		static int betaCount = 1;
		if(x==NULL) return;

		//cout << " next token " <<x->getVal() << endl;

		if(x->getVal()=="lambda"){
			std::stringstream ss;

			//if(i<=index)++index;
			//else index = i+1;
			int t1 = i;
			int k = 0;
			setOfDelta[i][j] = createNode("","");
			i =0;

			while(setOfDelta[i][0] != NULL){
				i++;
				k++;
			}
			i = t1;
			ss	<< k;
			index++;
			//ss	<< ++index;
			string str = ss.str();
			tree *temp = createNode(str, "deltaNumber");

			//cout << " pushing lambda val " << i << " " << j << endl;
			setOfDelta[i][j++] = temp;

			//cout <<  " pushing left of lambda " << x->left->getVal() << i << " " << j << endl;
			setOfDelta[i][j++] = x->left;

			//cout <<  " pushing lambda " << x->getVal() << i << " " << j << endl;
			setOfDelta[i][j++] = x;

			int myStoredIndex = i;
			int tempj = j+3;

			while( setOfDelta[i][0] != NULL )i++;
			j=0;

			myDelta(x->left->right,setOfDelta);

			i = myStoredIndex;
			j = tempj;

		}else if(x->getVal()=="->"){
			int myStoredIndex = i;
			int tempj = j;
			int nextDelta = index;
			int k =i;


			//cout << " next Delta " << nextDelta << endl;
			std::stringstream ss2;
			ss2	<< nextDelta;
			string str2 = ss2.str();
			tree *temp1 = createNode(str2, "deltaNumber");
			//cout <<  " pushing number " << x->getVal() << i << " " << j << endl;
			setOfDelta[i][j++] = temp1 ;

			int nextToNextDelta = index;
			std::stringstream ss3;
			ss3	<< nextToNextDelta;
			string str3 = ss3.str();
			//cout <<  " pushing number " << x->getVal() << i << " " << j << endl;
			tree *temp2 = createNode(str3, "deltaNumber");
			setOfDelta[i][j++] = temp2;

			//delta.push_back(x->left);
			tree *beta = createNode("beta", "beta");
			//cout <<  " pushing number " << x->getVal() << i << " " << j << endl;
			setOfDelta[i][j++] = beta;

			//cout << " print here " << setOfDelta[i][j-2]->getVal() <<  "  " <<  setOfDelta[i][j-1]->getVal() << endl;
			while(setOfDelta[k][0] != NULL){
				k++;
			}
			int firstIndex = k;
			int lamdaCount = index;

			myDelta(x->left,setOfDelta);
			int diffLc = index-lamdaCount;

			//cout<<"pushing1"<<endl;
					while(setOfDelta[i][0] != NULL )i++;
			j=0;

			myDelta(x->left->right,setOfDelta);

			//cout<<"pushing2"<<endl;


			while(setOfDelta[i][0] != NULL )i++;
			j=0;

			//cout << " kid 1 "<<x->left->getVal() << endl;
			//cout << " kid 2 "<<x->left->right->getVal() << endl;
			//cout << " kid 3 "<<x->left->right->right->getVal() << endl;

			/*if(x->left->right->right != NULL) */
			myDelta(x->left->right->right,setOfDelta);

			//cout<<"pushing5"<<endl;
			//setOfDelta.push_back(delta);

			//cout << " no of lambda is " << myStoredIndex << endl;
			//cout << " j is " <<  j << endl;


			stringstream ss23;
			if(diffLc==0 || i < lamdaCount){
				//cout << " lambda count " +  lamdaCount << endl;
				//cout<< " index " << index << endl;
				ss23<<firstIndex;
			}
			else{
				//cout << " no lambda = index " << endl;
				//cout << " index is " << endl;
				ss23	<< i-1;
			}

			string str5 = ss23.str();

			setOfDelta[myStoredIndex][tempj]->setVal(str5);
			stringstream ss24;
			ss24	<< i;
			string str6 = ss24.str();

			setOfDelta[myStoredIndex][tempj+1]->setVal(str6);

			i = myStoredIndex;
			j=0;

			while(setOfDelta[i][j] != NULL ){
				j++;
			}
			//j=0;
			betaCount+=2;

		}else if(x->getVal()=="tau"){
			tree *tauLeft = x->left;
			int numOfChildren = 0;
			while(tauLeft != NULL){
				numOfChildren++;
				tauLeft = tauLeft->right;
			}
			//cout << " no of kids of tau here " << endl;
			std::stringstream ss;
			ss	<< numOfChildren;
			string str = ss.str();
			tree *countNode = createNode(str,"CHILDCOUNT");
			//cout <<  " pushing child count  " << countNode->getVal() << i << " " << j << endl;

			setOfDelta[i][j++] = countNode; //putting the number of kids of tua
			//x->setType("tau");
			tree *tauNode = createNode("tau","tau");
			//cout <<  " pushing tau node " << tauNode->getVal() << i << " " << j << endl;

			setOfDelta[i][j++] = tauNode;//putting the tau node and not pushing x
			//x = x->left;
			//myDelta(x->left,delta,setOfDelta,index); //calling the delta on kid of tau
			myDelta(x->left,setOfDelta);
			x = x->left;
			while(x != NULL){
				myDelta(x->right,setOfDelta);
				x = x->right;
			}
		}else{
			//cout << " pushing to delta " << x->getVal() << " at pos " << i <<  " "<< j << endl;
			setOfDelta[i][j++] = createNode(x->getVal(),x->getType());
			myDelta(x->left,setOfDelta);
			if(x->left != NULL)	myDelta(x->left->right,setOfDelta);
		}
	}

	void makeST(tree *t){
		//tree *root;
		//standardizeAnd(t);
		makeStandard(t);
	}

	/*print_preOrder(int dots, tree *t){
		int n=0;
		while(n<dots){
			cout << ".";
			n++;
		}
		//cout << val << " and " << type << endl;
		if(t->getType() == "ID" || t->getType() == "STR" || t->getType() == "INT"){
			t->setVal("<" + t->getType() + ":"+ t->getVal()+ ">") ;
		}

		cout << t->getVal() << endl;

		if(t->left != NULL){
			print_preOrder(dots+1,t->left);
		}
		if(t->right != NULL){
			print_preOrder(dots,t->right);
		}
	} */

	tree *standardizeAnd(tree *t){
		if(t==NULL) return NULL;
		//cout << " val " << t->getVal() << endl;

		if(t->getVal()=="and"){
			tree *equal =  t->left;
			//cout << " my left " << equal->getVal() << endl;
			//tree *restEqual = t->left->right;
			t->setVal("=");
			t->setType("KEYWORD");
			t->left = createNode(",","PUNCTION");
			tree *comma = t->left;
			comma->left = createNode(equal->left);
			t->left->right = createNode("tau","KEYWORD");
			tree *tau = t->left->right;

			tau->left = createNode(equal->left->right);
			tau = tau->left;
			comma = comma->left;
			equal = equal->right;

			while(equal != NULL){
				comma->right = createNode(equal->left);
				comma = comma->right;
				tau->right = createNode(equal->left->right);
				tau = tau->right;

				equal = equal->right;
			}
		}
			//while()
		standardizeAnd(t->left);
		standardizeAnd(t->right);

		return t;
	}

	tree *makeStandard(tree *t){
		if(t == NULL) return NULL;
		makeStandard(t->left);
		makeStandard(t->right);

		//cout << " my val " << t->getVal() << endl;

		if(t->getVal()=="let"){
			if(t->left->getVal()=="="){
				t->setVal("gamma");
				t->setType("KEYWORD");
				tree *P = createNode(t->left->right);
				tree *X = createNode(t->left->left);
				tree *E = createNode(t->left->left->right);
				//t = t->left;
				t->left = createNode("lambda","KEYWORD");
				t->left->right = E; // do i set the right pointer to null
				tree *lambda = t->left;
				lambda->left = X;
				lambda->left->right = P;
				//if(t->left->left != NULL)cout << " find comma" << t->left->left->getVal() << endl;
				/*if(t->left->left->getVal()==","){
					tree *lamba = createNode(t->left);
					//cout << " hello 1" << endl;
					tree *X = lambda->left->left;
					tree *E = createNode(lambda->left->right);
					stack<tree*> num;
					//cout << " hello 2" << endl;
					while(X != NULL){
						tree *temp = createNode(X);
						num.push(temp);
						X = X->right;
					}
					//cout << " hello 3" << endl;
					lambda->left = createNode("T","KEYWORD");
					//lambda->left->right = createNode("gamma","KEYWORD");
					//lambda = lambda->left;
					while(!num.empty()){
						//cout << " hello 4" << endl;
						lambda->left->right = createNode("gamma","KEYWORD");
						lambda = lambda->left->right;
						//cout << " hello 4 a" << endl;

						lambda->left = createNode("lambda","KEYWORD");
						lambda->left->right = createNode("gamma","KEYWORD");
						lambda->left->left = num.top();
						//cout << " hello 4 b" << endl;

						//string  len = (string)num.size();
						std::stringstream ss;
						ss	<< num.size();
						string len = ss.str();
						num.pop();
						lambda->left->right->left = createNode("T","KEYWORD");
						lambda->left->right->left->right = createNode(len,"KEYWORD");
						//cout << " lambda->left " << lambda->left->getVal() << endl;
						lambda= lambda->left;
						//	cout << " hello 4 c" << endl;

					}
					//cout << " hello 5" << endl;
					if(num.empty()){
						lambda->left->right = E;
					}
					//cout << " hello 6" << endl;
			} */
		}
		} else if(t->getVal()=="and" && t->left->getVal()=="="){
			tree *equal =  t->left;
			//cout << " my left " << equal->getVal() << endl;
			//tree *restEqual = t->left->right;
			t->setVal("=");
			t->setType("KEYWORD");
			t->left = createNode(",","PUNCTION");
			tree *comma = t->left;
			comma->left = createNode(equal->left);
			t->left->right = createNode("tau","KEYWORD");
			tree *tau = t->left->right;

			tau->left = createNode(equal->left->right);
			tau = tau->left;
			comma = comma->left;
			equal = equal->right;

			while(equal != NULL){
				comma->right = createNode(equal->left);
				comma = comma->right;
				tau->right = createNode(equal->left->right);
				tau = tau->right;

				equal = equal->right;
			}
		}else if(t->getVal()=="where"){
			t->setVal("gamma");
			t->setType("KEYWORD");
			//cout << " where left " << t->left->getVal() << endl;
			//cout << " where left right "<<t->left->right->getVal() << endl;
			if(t->left->right->getVal()=="="){
				tree *P = createNode(t->left);
				tree *X = createNode(t->left->right->left);
				tree *E = createNode(t->left->right->left->right);
				t->left = createNode("lambda","KEYWORD");
				t->left->right = E;
				//t = t->left;
				t->left->left = X;
				t->left->left->right = P;
			}
		}/*else if(t->getVal()=="tau"){
			//cout << t->getVal() << endl;
			t->setVal("gamma");
			t->setType("KEYWORD");
			//if(t->left != NULL){
				tree *E = (t->left);
			//	tree *firstE = t->left ;
				//firstE->right = NULL;

				stack<tree*> num;
				//num.push(firstE);
				while(E != NULL){
					tree *temp = createNode(E);
					//temp->right = NULL;
					num.push(temp);
					E = E->right;
			//		cout<<"HELLO"<<endl;
				}

			//	cout << " here " << endl;
				tree *firstE = num.top();
				num.pop();
			//	cout << " first E "<<firstE->getVal() << endl;

				t->left = createNode("gamma","KEYWORD");

				tree *gamma = t->left;
				gamma->right = createNode(firstE);
				gamma->left = createNode("aug", "KEYWORD");
				gamma = gamma->left;
				//cout << " now start adding rest " << gamma->getVal() << endl;

				while(!num.empty()){
					tree *temp = num.top();
					num.pop();
					gamma->right = createNode("gamma","KEYWORD");
					gamma = gamma->right;
					//cout << " on right of aug should be gamma " << gamma->getVal() << endl;
					gamma->left = createNode("gamma", "KEYWORD");
					//cout << " new gamma on left " << gamma->left->getVal() << endl;

					gamma->left->right = createNode(temp);
					//cout << " gamma->left->right " << gamma->left->right->getVal() << endl;
					gamma->left->left = createNode("aug", "KEYWORD");
					//cout << " gamma->left->left should be aug " << gamma->left->left->getVal() << endl;
					gamma = gamma->left->left;
					//cout << " for next stay at aug " << gamma->getVal() << endl;
				}

				if(num.empty()){
					gamma->right = createNode("nil","KEYWORD");
				}
				//cout << " t "<<t->getVal() << endl;
				//cout << " t-> left " << t->left->getVal() << endl;
				//cout << " t left right " << t->left->right->getVal() << endl;

		} else if(t->getVal()=="->"){
			tree *B = createNode(t->left);
			tree *T = createNode(t->left->right);
			tree *E = createNode(t->left->right->right); //after this need to see what links to change
			//t = createNode("gamma","KEYWORD");
			t->setVal("gamma");
			t->setType("KEYWORD");

			t->left = createNode("gamma","KEYWORD");
			t->left->right = createNode("nil","KEYWORD");
			tree *tLeft = t->left;
			tLeft->left = createNode("gamma","KEYWORD");
			tLeft->left->right = createNode("lambda","KEYWORD");
			tLeft->left->right->left = createNode("()","KEYWORD");
			tLeft->left->right->left->right = E;
			tLeft = tLeft->left;
			tLeft->left = createNode("gamma","KEYWORD");
			tLeft->left->right = createNode("lambda","KEYWORD");
			tLeft->left->right->left = createNode("()","KEYWORD");
			tLeft->left->right->left->right = T;
			tLeft->left->left = createNode("Cond","KEYWORD");
			tLeft->left->left->right = B;

		}else if(t->getVal()=="not"){
			tree *E = createNode(t->left);
			//t = createNode("gamma","KEYWORD");
			t->setVal("gamma");
			t->setType("KEYWORD");

			t->left = createNode("not","KEYWORD");
			t->left->right = E;

		}else if(t->getVal()=="neg"){
			tree *E = createNode(t->left);
			//t = createNode("gamma","KEYWORD");
			t->setVal("gamma");
			t->setType("KEYWORD");
			t->left = createNode("neg","KEYWORD");
			t->left->right = E;

		}*/else if(t->getVal()=="within"){
			if(t->left->getVal()=="=" && t->left->right->getVal()=="="){
				tree *X1 = createNode(t->left->left);
				tree *E1 = createNode(t->left->left->right);
				tree *X2 = createNode(t->left->right->left);
				tree *E2 = createNode(t->left->right->left->right);
				//t = createNode("=","KEYWORD");
				t->setVal("=");
				t->setType("KEYWORD");
				t->left = X2;
				t->left->right = createNode("gamma","KEYWORD");
				tree *temp = t->left->right;
				temp->left = createNode("lambda","KEYWORD");
				temp->left->right = E1;
				temp= temp->left;
				temp->left = X1;
				temp->left->right = E2;
			}
		}else if(t->getVal()=="rec" && t->left->getVal()=="="){
			//cout << t->getVal() << " here in rec " << endl;
				tree *X = createNode(t->left->left);
				tree *E = createNode(t->left->left->right);
				//cout << " x left " << E->left->getVal() << endl;
				//cout << " x left right " << E->left->right->getVal() << endl;
				//cout << " x left right left" << E->left->right->left->getVal() << endl;
				//cout << " x left->left " << E->left->left->getVal() << endl;

				t->setVal("=");
				t->setType("KEYWORD");
				t->left = X;
				t->left->right = createNode("gamma","KEYWORD");
				t->left->right->left = createNode("YSTAR","KEYWORD");
				tree *ystar = t->left->right->left;
				//t->left->right->left->right = createNode("lambda","KEYWORD");
				ystar->right = createNode("lambda","KEYWORD");
				//cout<<"CHECK+ "<<t->left->right->left->right->getVal();
				//cout << X->getVal() << endl;
				//cout << E->getVal() << endl << " hello 54535";
				//t->left->right->left->right->left = X;
				//t->left->right->left->right->left->right = E;
				ystar->right->left = createNode(X);
				ystar->right->left->right = createNode(E);

			//}
				//cout << " printing rec tree" << endl;
				//t->print_my_tree(0) ;


		}else if( t->getVal()=="function_form"){
			//cout << " function form " << t->getVal() << endl;
			tree *P = createNode(t->left);
			tree *V = t->left->right;
			//tree *E = t->left->right->right;
			//t->setVal("=");
			//t= createNode("=","KEYWORD");
			t->setVal("=");
			t->setType("KEYWORD");
			t->left = P;
			//t->left->right = createNode("lambda","KEYWORD");
			tree *temp = t;
			while(V->right->right != NULL){
				//cout << " here 1 " << V->getVal() << endl;
				temp->left->right =  createNode("lambda","KEYWORD");
				temp = temp->left->right;
				temp->left = createNode(V);
				//temp = temp->left->right;
				V = V->right;
			}

			temp->left->right =  createNode("lambda","KEYWORD");
			temp = temp->left->right;
			//cout << " lambda " << temp->getVal() << endl ;
			//cout << " V " << V->getVal() << endl;
			//cout << " V->right " << V->right->getVal() << endl;

			//if(V->right == NULL){
				temp->left = createNode(V);
				temp->left->right = V->right;
			//}

			}else if(t->getVal()=="lambda"){
				//cout << "HELLOOOOOO LAMBDA" << endl;
				if(t->left != NULL){
					tree *V = t->left;
					tree *temp = t;
					if(V->right != NULL && V->right->right != NULL){
						while(V->right->right != NULL){
							//cout << " here 1 " << V->getVal() << endl;
							temp->left->right =  createNode("lambda","KEYWORD");
							temp = temp->left->right;
							temp->left = createNode(V);
							//temp = temp->left->right;
							V = V->right;
						}

						temp->left->right =  createNode("lambda","KEYWORD");
						temp = temp->left->right;
						//cout << " lambda " << temp->getVal() << endl ;
						//cout << " V " << V->getVal() << endl;
						//cout << " V->right " << V->right->getVal() << endl;
						//if(V->right == NULL){
						temp->left = createNode(V);
						temp->left->right = V->right;
					}
										//}
				}
			}/*else if(t->getVal() == "+" || t->getVal() == "-" || t->getVal() == "*" || t->getVal() == "/" || t->getVal() == "**"
			|| t->getVal() == "&" || t->getVal() == "gr" || t->getVal() == "ge" || t->getVal() == "ls" || t->getVal() == "le"
			|| t->getVal() == "eq" || t->getVal() == "ne"  ){
			string op = t->getVal();
			//cout << " operator " << t->getVal() << endl;
			tree *E1 = createNode(t->left);
			//cout << " E1 " << t->left->getVal() << endl;
			if(t->left->right != NULL){
				tree *E2 = createNode(t->left->right);
				//cout << " E2 " << E2->getVal() << endl;
				//t =  createNode("gamma","KEYWORD");
				t->setVal("gamma");
				t->setType("KEYWORD");
				t->left = createNode("gamma","KEYWORD");
				t->left->right = E2;
				tree *tLeft= t->left;
				tLeft->left = createNode(op,"KEYWORD");
				tLeft->left->right = E1;
			}else{
				//t = createNode("gamma","KEYWORD");
				t->setVal("gamma");
				t->setType("KEYWORD");
				t->left = createNode("gamma","KEYWORD");
				t->left->right = E1;
			}
		}*/else if(t->getVal()=="@"){
			tree *E1 = createNode(t->left);
			tree *N = createNode(t->left->right);
			tree *E2 = createNode(t->left->right->right);
			//t=  createNode("gamma","KEYWORD");
			t->setVal("gamma");
			t->setType("KEYWORD");
			t->left = createNode("gamma","KEYWORD");
			t->left->right = E2;
			//t= t->left;
			t->left->left = N;
			t->left->left->right = E1;
		}

					//tree *temp = st.top();
					//cout << " tree printed now " << endl;
					//temp->print_my_tree(0);
					//cout << endl;

	}

void proc_E(){
	//cout << " enter E" << " with token " << nt.getVal() << " token type " << nt.getType() << " and size " << st.size() <<endl;
	//cout << "in E with size " << st.size() << endl;
	//token temp;
	if(nt.getVal()=="let"){
//		cout << " token " << nt.getVal() << endl;
		read("let", "KEYWORD");
		proc_D();
	//	cout << " back to E with token here " << nt.getVal() << " and type " << nt.getVal() << " to read in " << " with size " << st.size() << endl;
		//token temp;
		//temp.setVal("in");
		//temp.setType("KEYWORD");
		read("in","KEYWORD"); //read in
		proc_E();
	//	cout << " back to E with token dsfsdg " << nt.getVal() << " and type " << nt.getVal() << " to read in " << " with size " << st.size() << endl;

		build_tree("let","KEYWORD",2);
	}else if( nt.getVal() =="fn") {
		int n =0;
		read("fn","KEYWORD");
		do{
			proc_Vb();
			n++;
		}while(nt.getType()=="ID" || nt.getVal() == "(");
		read(".","OPERATOR"); //read "."
		proc_E();
	//	cout << " back to E with token 33444 " << nt.getVal() << " and type " << nt.getVal() << " to read in " << " with size " << st.size() << endl;
		build_tree("lambda","KEYWORD",n+1);
	}else{
		proc_Ew();
	//	cout << " back to E from Ew " << nt.getVal() << " and type " << nt.getType() << endl;
	}
	//cout << " Exit E" << " with token " << nt.getVal() << " and type " << nt.getType() << " and size " << st.size() <<endl;

}

void proc_Ew(){
	//cout << " inside EW" << endl;
	//cout << " Enter Ew with token " << nt.getVal() << " with type " << nt.getType() << " with size " << st.size() << endl;
	proc_T();
	//cout << " back to Ew with token " << nt.getVal()<< " stack size " <<  st.size() << endl;
	if(nt.getVal()=="where"){
		read("where","KEYWORD");
		proc_Dr();
		build_tree("where","KEYWORD",2);
	}
	//	cout << " Exit Ew with token " << nt.getVal() << " with type " << nt.getType() << " with size " << st.size() << endl;

}

void proc_T(){
	//cout << "in T with size " << st.size() << endl;
	//cout << " Enter T" << " with token "<< nt.getVal() << " end type " << nt.getType() <<  " and size " << st.size() << endl;
	proc_Ta();
	//cout << " back to t with " << nt.getVal() << " and type " << nt.getType() << " with size "<< st.size() << endl;
	int n =1;
	while(nt.getVal()==","){
		n++;
		read(",","PUNCTION");
		proc_Ta();
	//	cout << " back to t in the while " << nt.getVal() << " and type " << nt.getType() << endl;
	}
	if(n>1){
		build_tree("tau","KEYWORD",n);
	}
	//cout << " Exit T" << " with token "<< nt.getVal() << " end type " << nt.getType() <<  " and size " << st.size() << endl;

}

void proc_Ta(){
	//cout << " Enter Ta with token " << nt.getVal() <<  " and type " << nt.getType() << " and stack size " << st.size() << endl;
	//cout << "in Ta with size " << st.size() << endl;
	proc_Tc();
	//cout << " back to tA with token " << nt.getVal() << " and type " << nt.getType() << " with stack size "<<st.size() <<endl;
	while(nt.getVal()=="aug"){
		read("aug","KEYWORD");
		proc_Tc();
		build_tree("aug","KEYWORD",2);
	}
	//cout << " Exit Ta with token " << nt.getVal() <<  " and type " << nt.getType() << " and stack size " << st.size() << endl;
}

void proc_Tc(){
	//cout << " inside Tc " << endl;
	//cout << " Enter Tc with token " << nt.getVal() << " and type " << nt.getType() << " with size " << st.size() << endl;
	proc_B();
	//cout << " back to Tc with token " << nt.getVal() <<  " and type " << nt.getType() << " with size " << st.size() << endl;
	if(nt.getVal()=="->"){
		read("->","OPERATOR");
		proc_Tc();
		read("|","OPERATOR"); //read "|"
		proc_Tc();
		build_tree("->","KEYWORD",3);
	}
	//cout << " Exit Tc with token " << nt.getVal() << " and type " << nt.getType() << " with size " << st.size() << endl;
}

void proc_B(){
	//cout<< " Enter B with token "  << nt.getVal() << " and type " << nt.getType() << " with size " << st.size() << endl;
	//cout << " inside B" << endl;
	proc_Bt();
	//cout << " back to b with token " << nt.getVal() << " and type " << nt.getType() << " stack size " << st.size() << endl;
	while(nt.getVal()=="or"){
		read("or","KEYWORD");
		proc_Bt();
		build_tree("or","KEYWORD",2);
	}
	//cout<< " Exit B with token "  << nt.getVal() << " and type " << nt.getType() << " with size " << st.size() << endl;
}

void proc_Bt(){
	//cout << " Enter Bt with token " << nt.getVal() << " with type " << nt.getType() << "  with size " << st.size() << endl;
	//cout << " inside Bt " << endl;
	proc_Bs();
	//cout << " back to bt with token " << nt.getVal() << " and type " << nt.getType() << " stack size " << st.size() << endl;
	while( nt.getVal() == "&"){
		read("&","OPERATOR");
		proc_Bs();
		build_tree("&","KEYWORD",2);
	}
	//	cout << " Exit Bt with token " << nt.getVal() << " with type " << nt.getType() << "  with size " << st.size() << endl;
}

void proc_Bs(){
	//cout << " inside bs " << endl;
	//cout << " Enter Bs with token " << nt.getVal() << " with type " << nt.getType() << "  with size " << st.size() << endl;

	if(nt.getVal()== "not"){
		read("not","KEYWORD");
		proc_Bp();
		build_tree("not","KEYWORD",1);
	}else{
		proc_Bp();
	//	cout << " back to bs with token " << nt.getVal() << " and type " << nt.getType() << " stack size " << st.size() << endl;
	}
	//cout << " Exit Bs with token " << nt.getVal() << " with type " << nt.getType() << "  with size " << st.size() << endl;

}

void proc_Bp(){
	//cout << " inside Bp" << endl;
	//cout << " Enter Bp with token " << nt.getVal() << " with type " << nt.getType() << "  with size " << st.size() << endl;

	proc_A();
	string temp = nt.getVal();
	string temp2 = nt.getType();
	//cout << " back to Bp with token " << nt.getVal() << " and type " << nt.getType() << "stack size " << st.size() << endl;
	if(nt.getVal()=="gr" || nt.getVal()==">"){
		//read("gr","KEYWORD");
		read(temp,temp2);
		proc_A();
		build_tree("gr","KEYWORD",2);
	}else if(nt.getVal()=="ge" || nt.getVal() == ">="){
		//read("ge","KEYWORD");
		read(temp,temp2);
		proc_A();
		build_tree("ge","KEYWORD",2);
	}else if(nt.getVal()=="ls" || nt.getVal() == "<"){
		//read("ls","KEYWORD");
		read(temp,temp2);
		proc_A();
		build_tree("ls","KEYWORD",2);
	}else if(nt.getVal()=="le" || nt.getVal() == "<="){
		//read("le","KEYWORD");
		read(temp,temp2);
		proc_A();
		build_tree("le","KEYWORD",2);
	}else if(nt.getVal()=="eq"){
		//read("eq","KEYWORD");
		read(temp,temp2);
		proc_A();
	//	cout << " back to Bp with token " << nt.getVal() << " with stack size " << st.size() << endl;
		build_tree("eq","KEYWORD",2);
	}else if(nt.getVal()=="ne"){
		//read("ne","KEYWORD");
		read(temp,temp2);
		proc_A();
		build_tree("ne","KEYWORD",2);
	}
//cout << " Exit Bp with token " << nt.getVal() << " with type " << nt.getType() << "  with size " << st.size() << endl;

}

void proc_A(){
	//cout << " inside A " << endl;
//	cout << " Enter A with token " << nt.getVal() << " with type " << nt.getType() << "  with size " << st.size() << endl;
	if(nt.getVal()=="-"){
		read("-","OPERATOR");
		proc_At();
		build_tree("neg","KEYWORD",1);
	}else if(nt.getVal()=="+"){
		read("+","OPERATOR");
		proc_At();
	}else{
		proc_At();
	//	cout << " back to A with token " << nt.getVal() << " and type  " << nt.getType() << " and size "<< st.size() <<endl;
	}

	while(nt.getVal()=="+" || nt.getVal() == "-"){
		string temp = nt.getVal();
		read(temp,"OPERATOR");
		proc_At();
		build_tree(temp,"OPERATOR",2);
	}
//	cout << " Exit A with token " << nt.getVal() << " with type " << nt.getType() << "  with size " << st.size() << endl;

}

void proc_At(){
	//cout << " inside At " << endl;
//	cout << " Enter At with token " << nt.getVal() << " with type " << nt.getType() << "  with size " << st.size() << endl;

	proc_Af();
//	cout << " back to At with token " << nt.getVal() << " and type " << nt.getType() << " with size "<< st.size() <<endl;
	while(nt.getVal() == "*" || nt.getVal() == "/"){
		string temp = nt.getVal();
			read(temp,"OPERATOR");
			proc_Af();
		build_tree(temp,"OPERATOR",2);
	}
//	cout << " Exit At with token " << nt.getVal() << " with type " << nt.getType() << "  with size " << st.size() << endl;

}

void proc_Af(){
	//cout << " inside Af " << endl;
//	cout << " Enter Af with token " << nt.getVal() << " with type " << nt.getType() << "  with size " << st.size() << endl;
	proc_Ap();
//	cout << " back to Af with token " << nt.getVal() << " and type " << nt.getType() << " and size "<< st.size() <<endl;
	if(nt.getVal()=="**"){
		read("**","OPERATOR");
		proc_Af();
		build_tree("**","KEYWORD",2);
	}
//	cout << " Exit Af with token " << nt.getVal() << " with type " << nt.getType() << "  with size " << st.size() << endl;
}

void proc_Ap(){
	//cout << " inside Ap " << endl;
//	cout << " Enter Ap with token " << nt.getVal() << " with type " << nt.getType() << "  with size " << st.size() << endl;

	proc_R();
//	cout << " back to Ap with token " << nt.getVal() << " and size " << st.size() << endl;
	//token temp;
	while(nt.getVal()=="@"){
		read("@","OPERATOR"); //read a type ID
		if(nt.getType() != "ID"){
			cout << " unexpected token ";
		}else{
			read(nt.getVal(),"ID");
		    proc_R();
//		    cout << " back to Ap token is " << nt.getVal() << " type " << nt.getType() << " and size "  << st.size() << endl;
		   build_tree("@","KEYWORD",3);

		}
	}
//	cout << " Exit Ap with token " << nt.getVal() << " with type " << nt.getType() << "  with size " << st.size() << endl;

}

void proc_R(){
	//cout << " inside R " << endl;
//	cout << " Enter R with token " << nt.getVal() << " with type " << nt.getType() << "  with size " << st.size() << endl;
	proc_Rn();
//	cout << " coming back to R with token " << nt.getVal() <<  " with size "<< st.size()  <<endl;
//	cout << " in R " << nt.getType() << " with token " << nt.getVal() << endl;
		//cout << " again executing rn for token " << nt.getVal() << endl;
		while(nt.getType()== "ID" || nt.getType()=="INT" || nt.getType()=="STR" || nt.getVal() == "true"
		|| nt.getVal() == "false" || nt.getVal() == "dummy" || nt.getVal() == "nil" || nt.getVal() == "("	){
		proc_Rn();
		build_tree("gamma","KEYWORD",2); //keep reading till valid node from rn comes
//		cout << " token " << nt.getVal() << " if of type " << nt.getType() << endl;
	}
//	cout << " Exit R with token " << nt.getVal() << " with type " << nt.getType() << "  with size " << st.size() << endl;

}

void proc_Rn(){
//	cout << " Enter Rn  " <<" with token " << nt.getVal() << " and type " << nt.getType() << "in Rn with size " << st.size() <<endl;
	if(nt.getType()== "ID" || nt.getType()=="INT" || nt.getType()=="STR"){
		//cout << " this is " << nt.getType() << endl;
		read(nt.getVal(),nt.getType());
	}else if(nt.getVal()=="true"){
		read("true","KEYWORD");
		build_tree("true","BOOL",0);
	}else if(nt.getVal() == "false"){
		read("false","KEYWORD");
		build_tree("false","BOOL",0);
	}else if(nt.getVal()=="nil"){
		read("nil","KEYWORD"); // i think i need to pass tokentype
		build_tree("nil","NIL",0);
	}else if(nt.getVal() =="("){
		//cout << " i got here with " << nt.getVal() << endl;
		read("(","PUNCTION");
		//cout << nt.getVal() << " with type before E " << nt.getType() << endl;
		proc_E();
		//cout << " came back to Rn with token " << nt.getVal() << endl;
		read(")","PUNCTION"); // read ")" punction
	}else if( nt.getVal()=="dummy"){
		read("dummy","KEYWORD");
		build_tree("dummy","DUMMY",0);
	}
//	cout << " Exit Rn  " <<" with token " << nt.getVal() << " and type " << nt.getType() << " with size " << st.size() <<endl;
}

void proc_D(){
//	cout << " Enter D  " <<" with token " << nt.getVal() << " and type " << nt.getType() << " with size " << st.size() <<endl;
	//cout << " inside D " << endl;
	proc_Da();
	if(nt.getVal()=="within"){
		read("within","KEYWORD");
		proc_Da();
		build_tree("within","KEYWORD",2);
	}
//	cout << " Exit D  " <<" with token " << nt.getVal() << " and type " << nt.getType() << " with size " << st.size() <<endl;

}

void proc_Da(){
//	cout << " Enter Da  " <<" with token " << nt.getVal() << " and type " << nt.getType() << " with size " << st.size() <<endl;
	//cout << " inside Da" << endl;
	proc_Dr();
	int n=1;
	while(nt.getVal()=="and"){
		n++;
		read("and","KEYWORD");
		proc_Dr();
	}
	if(n>1){
		build_tree("and","KEYWORD",n);
	}
//	cout << " Exit Da  " <<" with token " << nt.getVal() << " and type " << nt.getType() << " with size " << st.size() <<endl;

}

void proc_Dr(){
//	cout << " Enter Dr  " <<" with token " << nt.getVal() << " and type " << nt.getType() << " with size " << st.size() <<endl;
	//cout << " inside Dr " << endl;
	if(nt.getVal()=="rec"){
		read("rec","KEYWORD");
		proc_Db();
		build_tree("rec","KEYWORD",1);
	}else{
		proc_Db();
	}
//	cout << " Exit Dr  " <<" with token " << nt.getVal() << " and type " << nt.getType() << " with size " << st.size() <<endl;

}

void proc_Db(){ //verify this function doubtful, sort of read functuon and getTOken
//	cout << " inside Db " << endl;
//cout << " Enter Db  " <<" with token " << nt.getVal() << " and type " << nt.getType() << " with size " << st.size() <<endl;
		//token temp;
	if(nt.getVal()=="("){
		read("(","PUNCTION");
		proc_D();
		read(")","PUNCTION"); //read ")"
	}else if( nt.getType() == "ID"){
		read(nt.getVal(),"ID");
		int n =1;
		if(nt.getVal()=="=" || nt.getVal()==","){ //do i need to read a token here ????
			while(nt.getVal()==","){
				read(",","PUNCTION");
				//temp.setVal(nt.getVal());
				//temp.setType("ID");
				read(nt.getVal(),"ID");
				//read(nt);
				n++;
			}
			if(n>1){
				build_tree(",","KEYWORD",n);
			}
			read("=","OPERATOR"); //read "="
			proc_E();
			build_tree("=","KEYWORD",2);
		}else{
			do{
				proc_Vb();
				n++;
			}while(nt.getType()=="ID" || nt.getVal()=="(");
			read("=","OPERATOR"); //read "="
			proc_E();
			build_tree("function_form","KEYWORD",n+1);
		}
	}
//	cout << " Exit Db  " <<" with token " << nt.getVal() << " and type " << nt.getType() << " with size " << st.size() <<endl;

}

void proc_Vb(){
//	cout << " inside Vb" << endl;
//cout << " Enter Vb  " <<" with token " << nt.getVal() << " and type " << nt.getType() << " with size " << st.size() <<endl;
		if (nt.getType()=="ID"){
		read(nt.getVal(),"ID");
	}else if (nt.getVal()=="("){
		read("(","PUNCTION");
		if(nt.getVal()==")"){
			read(")","PUNCTION");
			build_tree("()","KEYWORD",0);
		}else{
			proc_Vl();
//			cout << " came back from Vl " << endl;
			read(")","PUNCTION"); //read ")"
		}
	}
//cout << " Exit Vb  " <<" with token " << nt.getVal() << " and type " << nt.getType() << " with size " << st.size() <<endl;

}

void proc_Vl(){
//	cout << " inside Vl" << endl;
//cout << " Enter Vl  " <<" with token " << nt.getVal() << " and type " << nt.getType() << " with size " << st.size() <<endl;
		int n =1;
	read(nt.getVal(),"ID"); // read type ID

	while(nt.getVal()==","){
		read(",","PUNCTION"); //read "," and get next ID
		read(nt.getVal(),"ID"); //read and build tree for next ID and get next "," or anything else
		n++;
	}
	if(n>1){
		build_tree(",","KEYWORD",n);
	}
//cout << " Exit Vl  " <<" with token " << nt.getVal() << " and type " << nt.getType() << " with size " << st.size() <<endl;

}

};
