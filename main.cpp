#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string.h>
#include "parser.h"

using namespace std;

int main( int argc , char** argv ){
	if( argc > 3 || argc <=1 ){
		cout << " Incorrect number of inputs "<< endl;
	}else if(argc==3){
		if(strcmp(argv[1],"-ast") == 0){
			string filename = argv[2];
			const char* file = filename.c_str();
			ifstream input(file);
	    		string myfile( (istreambuf_iterator<char>(input) ), (istreambuf_iterator<char>()) );
			char mychar;
			
			int i =0;
			char readnew[myfile.size()];
			for(i=0;i<myfile.size();i++){
				readnew[i] = myfile[i];
			}
			parser newparser(readnew, 0, myfile.size(),1);
			newparser.start_parsing();
		}
	}else if(argc==2){
			string filename = argv[1];
			const char* file = filename.c_str();
			ifstream input(file);
	    		string myfile( (istreambuf_iterator<char>(input) ), (istreambuf_iterator<char>()) );
			char mychar;
			
			int i =0;
			char readnew[myfile.size()];
			for(i=0;i<myfile.size();i++){
				readnew[i] = myfile[i];
			}
			parser newparser(readnew, 0, myfile.size(),0);
			newparser.start_parsing();
	}else{
 			cout << " Incorrect input value " << endl;
			
	}

	return 0;
}
