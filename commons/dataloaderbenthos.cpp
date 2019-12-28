#include "dataloaderbenthos.h"
#include "myutils.h"
#include "dataloader.h"
#include "Node.h"
#include "Benthos.h"
#include "helpers.h"
#include "options.h"

#include <msqlitecpp/v2/storage.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <deque>
#include <map>
#include <list>
#include <set>

namespace sql = msqlitecpp::v2;
using namespace std;

Dataloaderbenthos::Dataloaderbenthos() = default;

// FOR BENTHOS
multimap<int, double>
read_prop_funcgr_biomass_per_node_per_landscape(string folder_name_parameterization, string inputfolder)
{

    string filename = inputfolder + "/benthosspe_" + folder_name_parameterization +
                      "/prop_funcgr_biomass_per_node_per_landscape.dat";

    ifstream file_prop_funcgr_per_node;
    file_prop_funcgr_per_node.open(filename.c_str());
    if (file_prop_funcgr_per_node.fail()) {
        string error_msg = "error opening file " + filename;
        cout << error_msg << "\n";

        exit(-1);
    }
    multimap<int, double> prop_funcgr_per_node;
    fill_multimap_from_specifications_i_d(file_prop_funcgr_per_node,  prop_funcgr_per_node);
    file_prop_funcgr_per_node.close();

    return(prop_funcgr_per_node);
}


multimap<int, double> read_longevity_classes_condition_per_node(string folder_name_parameterization, string inputfolder)
{

    string filename=  inputfolder+"/benthosspe_"+folder_name_parameterization+"/longevity_classes_condition_per_node.dat";

    ifstream file_longevity_classes_condition_per_node;
    file_longevity_classes_condition_per_node.open(filename.c_str());
    if(file_longevity_classes_condition_per_node.fail())
    {
        string error_msg = "error opening file " + filename;
        cout << error_msg << "\n";

        exit(-1);
     }
    multimap<int, double> longevity_classes_condition_per_node;
    fill_multimap_from_specifications_i_d(file_longevity_classes_condition_per_node,  longevity_classes_condition_per_node);
    file_longevity_classes_condition_per_node.close();

    return(longevity_classes_condition_per_node);
}


multimap<int, double> read_prop_funcgr_number_per_node_per_landscape(string folder_name_parameterization, string inputfolder)
{

    string filename=  inputfolder+"/benthosspe_"+folder_name_parameterization+"/prop_funcgr_number_per_node_per_landscape.dat";

    ifstream file_prop_funcgr_per_node;
    file_prop_funcgr_per_node.open(filename.c_str());
    if(file_prop_funcgr_per_node.fail())
    {
        string error_msg = "error opening file " + filename;
        cout << error_msg << "\n";

        exit(-1);
    }
    multimap<int, double> prop_funcgr_per_node;
    fill_multimap_from_specifications_i_d(file_prop_funcgr_per_node,  prop_funcgr_per_node);
    file_prop_funcgr_per_node.close();

    return(prop_funcgr_per_node);
}

multimap<int, double> read_meanw_funcgr_per_landscape(string folder_name_parameterization, string inputfolder)
{

    string filename=  inputfolder+"/benthosspe_"+folder_name_parameterization+"/meanw_funcgr_per_landscape.dat";

    ifstream file_meanw_funcgr_per_node;
    file_meanw_funcgr_per_node.open(filename.c_str());
    if(file_meanw_funcgr_per_node.fail())
    {
        string error_msg = "error opening file " + filename;
        cout << error_msg << "\n";

        exit(-1);
     }
    multimap<int, double> meanw_funcgr_per_node;
    fill_multimap_from_specifications_i_d(file_meanw_funcgr_per_node,  meanw_funcgr_per_node);
    file_meanw_funcgr_per_node.close();

    return(meanw_funcgr_per_node);
}

multimap<int, double> read_logistic_recovery_rates_per_month_per_funcgr(string folder_name_parameterization, string inputfolder)
{

    string filename=  inputfolder+"/benthosspe_"+folder_name_parameterization+"/logistic_recovery_rates_per_month_per_funcgr.dat";

    ifstream logistic_recovery_rates_per_month_per_funcgr;
    logistic_recovery_rates_per_month_per_funcgr.open(filename.c_str());
    if(logistic_recovery_rates_per_month_per_funcgr.fail())
    {
        string error_msg = "error opening file " + filename;
        cout << error_msg << "\n";

        exit(-1);
    }
    multimap<int, double> rates_per_month_per_funcgr;
    fill_multimap_from_specifications_i_d(logistic_recovery_rates_per_month_per_funcgr,  rates_per_month_per_funcgr);
    logistic_recovery_rates_per_month_per_funcgr.close();

    return(rates_per_month_per_funcgr);
}


multimap<int, double> read_habitat_deltas_per_pop(string folder_name_parameterization, string inputfolder)
{

    string filename=  inputfolder+"/benthosspe_"+folder_name_parameterization+"/habitat_deltas_per_pop.dat";

    ifstream habitat_deltas_per_pop_file;
    habitat_deltas_per_pop_file.open(filename.c_str());
    if(habitat_deltas_per_pop_file.fail())
    {
        string error_msg = "error opening file " + filename;
        cout << error_msg << "\n";

        exit(-1);
    }
    multimap<int, double> habitat_deltas_per_pop;
    fill_multimap_from_specifications_i_d(habitat_deltas_per_pop_file,  habitat_deltas_per_pop);
    habitat_deltas_per_pop_file.close();

    return(habitat_deltas_per_pop);
}



multimap<int, double> read_benthos_biomass_carrying_capacity_K_per_landscape_per_funcgr(string folder_name_parameterization, string inputfolder)
{

    string filename=  inputfolder+"/benthosspe_"+folder_name_parameterization+"/benthos_biomass_carrying_capacity_K_per_landscape_per_funcgr.dat";

    ifstream benthos_biomass_carrying_capacity_K_per_landscape_per_funcgr;
    benthos_biomass_carrying_capacity_K_per_landscape_per_funcgr.open(filename.c_str());
    if(benthos_biomass_carrying_capacity_K_per_landscape_per_funcgr.fail())
    {
        string error_msg = "error opening file " + filename;
        cout << error_msg << "\n";

        exit(-1);
     }
    multimap<int, double> K_per_landscape_per_funcgr;
    fill_multimap_from_specifications_i_d(benthos_biomass_carrying_capacity_K_per_landscape_per_funcgr,  K_per_landscape_per_funcgr);
    benthos_biomass_carrying_capacity_K_per_landscape_per_funcgr.close();

    return(K_per_landscape_per_funcgr);
}


multimap<int, double> read_benthos_number_carrying_capacity_K_per_landscape_per_funcgr(string folder_name_parameterization, string inputfolder)
{

    string filename=  inputfolder+"/benthosspe_"+folder_name_parameterization+"/benthos_number_carrying_capacity_K_per_landscape_per_funcgr.dat";

    ifstream benthos_number_carrying_capacity_K_per_landscape_per_funcgr;
    benthos_number_carrying_capacity_K_per_landscape_per_funcgr.open(filename.c_str());
    if(benthos_number_carrying_capacity_K_per_landscape_per_funcgr.fail())
    {
        string error_msg = "error opening file " + filename;
        cout << error_msg << "\n";

        exit(-1);
    }
    multimap<int, double> K_per_landscape_per_funcgr;
    fill_multimap_from_specifications_i_d(benthos_number_carrying_capacity_K_per_landscape_per_funcgr,  K_per_landscape_per_funcgr);
    benthos_number_carrying_capacity_K_per_landscape_per_funcgr.close();

    return(K_per_landscape_per_funcgr);
}



int Dataloaderbenthos::gettype()
{
    return(2);
}

int Dataloaderbenthos::features(const string &folder_name_parameterization,
                                const string &inputfolder,
                                PopSceOptions const &dyn_pop_sce,
                                DynAllocOptions const &dyn_alloc_sce,
                                string const &biolsce,
                                string const &fleetsce)
{
    cout << "Loading benthos features" << endl;

    if (dyn_pop_sce.option(Options::modelBenthosInN)) {
        data.int1 = 1; // is_benthos_in_numbers
    } else {
        data.int1 = 0; // is_benthos_in_numbers // if not N then it impacts the benthos biomass by default
    }

    if (dyn_pop_sce.option(Options::modelBenthosInLongevity)) {
        data.int2 = 1; //is_benthos_in_longevity_classes
    } else {
        data.int2 = 0; // is_benthos_in_longevity_classes
    }


    // read estimates
    data.mmapidparam1 = read_meanw_funcgr_per_landscape(folder_name_parameterization,
                                                        inputfolder);


    if (dyn_pop_sce.option(Options::modelBenthosInLongevity)) {
        data.mmapidparam2 = read_longevity_classes_condition_per_node(folder_name_parameterization,
                                                                      inputfolder);
        data.mmapidparam3 = read_benthos_biomass_carrying_capacity_K_per_landscape_per_funcgr(
                folder_name_parameterization, inputfolder);
    } else {
        if (dyn_pop_sce.option(Options::modelBenthosInN)) {
            data.mmapidparam4 = read_prop_funcgr_number_per_node_per_landscape(folder_name_parameterization,
                                                                               inputfolder);
            data.mmapidparam5 = read_benthos_number_carrying_capacity_K_per_landscape_per_funcgr(
                    folder_name_parameterization, inputfolder);
        } else {
            data.mmapidparam6 = read_prop_funcgr_biomass_per_node_per_landscape(folder_name_parameterization,
                                                                                inputfolder);
            data.mmapidparam3 = read_benthos_biomass_carrying_capacity_K_per_landscape_per_funcgr(
                    folder_name_parameterization, inputfolder);
        }
    }


    data.mmapidparam7 = read_logistic_recovery_rates_per_month_per_funcgr(
            folder_name_parameterization, inputfolder);

    data.mmapidparam8 = read_habitat_deltas_per_pop(folder_name_parameterization,
                                                    inputfolder);


    return 0;
}
