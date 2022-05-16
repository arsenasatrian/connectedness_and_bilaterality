#include </home/arasatry/cpp/graph/flat_graph.h>
#include </home/arasatry/cpp/graph/cmdl.h>

using namespace std;

class run_graph {
public:
    run_graph() 
    {
        const cmdl* cmdl_p = cmdl::get_cmdl_p();
        run(*cmdl_p);
    }
    ~run_graph() {}

    void run(const cmdl& cmd);
    void read_from_file(const cmdl& cmd); 
    void write_to_report(const cmdl& cmd); 
    void check(const cmdl& cmd, ofstream& f);
    void check_connectivity(const cmdl& cmd, ofstream& f);
    void check_bilaterality(const cmdl& cmd, ofstream& f);

private:
    Flat_Graph g_;
};

void run_graph::run(const cmdl& cmd)
{
    read_from_file(cmd);
    write_to_report(cmd);
}
void run_graph::read_from_file(const cmdl& cmd)
{
    g_.read_from_file(cmd.file_name);
}
void run_graph::write_to_report(const cmdl& cmd)
{
    ofstream file;
    file.open(cmd.report_file_name);
    if (file.is_open()) {
        check(cmd, file);
    } 
    else {
        cout << "\nCannot open " << cmd.report_file_name << " file.";
        return;
    }
    file.close();    
}

void run_graph::check(const cmdl& cmd, ofstream& f)
{
    if( cmd.is_connected == true ) {
        g_.check_connectedness(f);
    }
    if( cmd.is_bi_graph == true ) {
        g_.check_bilaterality(f);
    }
    if( cmd.check_both == true ) {
        g_.check_connectedness(f);
        g_.check_bilaterality(f);
    }
}

