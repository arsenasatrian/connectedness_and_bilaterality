#include <iostream>
#include <string.h>
#include <cassert>
#include <cstdlib>

using namespace std;

class cmdl {
    static cmdl* cmdl_p;
public:
    bool is_connected;
    bool is_bi_graph;
    bool check_both;
    bool file;
    char* file_name;
    bool report;
    char* report_file_name;

    int argc_;
    char** argv_;

    cmdl(int argc, char** argv);
    ~cmdl();

    void semantic_error(int n);
    static const cmdl* get_cmdl_p() { return cmdl_p; }
};

cmdl *cmdl::cmdl_p = NULL;

cmdl::cmdl(int argc, char** argv)
{
    cmdl_p = this;
    is_connected = false;
    is_bi_graph = false;
    check_both = false;
    file = false;
    file_name = nullptr;
    report = false;
    report_file_name = nullptr;
    int arg_index = 1;

    argc_ = argc;
    argv_ = new char*[argc_];
    for(int i = 0 ; i < argc_ ; i++){
        argv_[i] = new char[strlen(argv[i])+1];
        strcpy(argv_[i],argv[i]);
    }

    while(arg_index < argc_) {
        char* flag = argv_[arg_index++];

        if( strcmp(flag, "-is_connected")  == 0 ){
            if( check_both == true ) {
                semantic_error(1);
            }
            is_connected = true;
        }
        if( strcmp(flag, "-is_bi_graph")  == 0 ){
            if( check_both == true ) {
                semantic_error(2);
            }
            is_bi_graph = true;
        }
        if( strcmp(flag, "-check_both")  == 0 ){
            if( is_connected == true || is_bi_graph == true ) {
                semantic_error(3);
            }
            check_both = true;
        }
        if( strcmp(flag, "-file")  == 0 ){
            file = true;
            file_name = argv_[arg_index++];
        }
        if( strcmp(flag, "-report")  == 0 ){
            report = true;
            report_file_name = argv_[arg_index++];
        }
    }
    if( !(is_connected || is_bi_graph) && !check_both ) {
        semantic_error(4);
    }
    if( !file && !(file_name == nullptr) ) {
        semantic_error(5);
    }

}

void cmdl::semantic_error(int n)
{
    switch (n) {
        case 1: {
            cout << "\nERROR: -is_connected and -check_both combination is meaningless use one of those.\n";
        } break;
        case 2: {
            cout << "\nERROR: -is_bi_graph and -check_both combination is meaningless use one of those.\n";
        } break;
        case 3: {
            cout << "\nERROR: -check_both  and (-is_connected or -is_bi_graph) combination is meaningless use one of those.\n";
        } break;
        case 4: {
            cout << "\nERROR: need to use one of (-is_connected or -is_bi_graph) or -check_both.\n";
            cout << "n = " << n << endl;  
        }
        case 5: {
            cout << "n = " << n << endl;  
            cout << "\nERROR: -file <file_name> is a required argument.\n";
        } break;
    } 
    cerr << flush;
    exit(1);
}

cmdl::~cmdl() {
    if(argv_ != nullptr){
        for(int i = 0 ; i < argc_ ; i++){
            delete [] argv_[i];
        }
        delete [] argv_;
        argc_ = 0;
        argv_ = nullptr;
    }
}
	
