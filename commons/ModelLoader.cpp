//
// Created by happycactus on 12/28/19.
//

#include "ModelLoader.h"
#include "Benthos.h"
#include "helpers.h"
#include "options.h"

#include <boost/optional.hpp>

struct ModelLoader::Impl {
    bool scenarioLoaded = false;
    bool configLoaded = false;
    bool nodesLoaded = false;

    bool doConsistencyTest = false;

    size_t nbbenthospops;
    bool is_benthos_in_numbers = false; // otherwise the impact is on biomass by default
    bool is_benthos_in_longevity_classes = false;

    vector<Node *> nodes;
};

ModelLoader::ModelLoader()
        : p(spimpl::make_unique_impl<Impl>())
{

}

std::vector<Benthos *> ModelLoader::loadBenthos(std::vector<int> const &graph_point_code_landscape,
                                                PopSceOptions const &dyn_pop_sce,
                                                DynAllocOptions const &dyn_alloc_sce,
                                                std::string const &biolsce,
                                                std::string const &fleetsce)
{
    ensure(p->scenarioLoaded && p->nodesLoaded && p->configLoaded);

    auto loadedDataBenthos = loadBenthosData(dyn_pop_sce, dyn_alloc_sce, biolsce, fleetsce);

    int nbland = graph_point_code_landscape.size();


    // creation of a vector of benthos shared (one benthos shared per landscape)
    auto benthoss = vector<Benthos *>(nbland);

    outc(cout << "nb of marine landscapes " << nbland << endl);

    // LOOP OVER BENTHOS OBJECT
    for (int landscape = 0; landscape < nbland; landscape++) {

        vector<double> init_meanw_funcgr_per_node;
        vector<double> init_prop_funcgr_number_per_node;
        vector<double> init_prop_funcgr_biomass_per_node;
        vector<double> init_benthos_number_carrying_capacity_K_per_landscape_per_funcgr;
        vector<double> init_benthos_biomass_carrying_capacity_K_per_landscape_per_funcgr;
        vector<double> init_recovery_rates_per_funcgr;
        vector<double> init_h_betas_per_pop;

        int a_marine_landscape = graph_point_code_landscape.at(landscape);

        outc(cout << "a marine landscape " << a_marine_landscape << endl);

        if (dyn_pop_sce.option(Options::modelBenthosInLongevity)) {
            multimap<int, double>::iterator lower_it_lgy = loadedDataBenthos.mmapidparam2.lower_bound(0);
            multimap<int, double>::iterator upper_it_lgy = loadedDataBenthos.mmapidparam2.upper_bound(0);

            vector<double> a_vector;
            for (multimap<int, double>::iterator pos = lower_it_lgy; pos != upper_it_lgy; pos++) {
                a_vector.push_back(pos->second);
            }


            if (a_vector.size() != p->nbbenthospops) {
                cout << "on node 0: " << " nb longevity class is " << a_vector.size() <<
                     ": error for benthos file: check the dims in longevity_classes_condition_per_node input file. kill, correct and re-run."
                     << endl;
                int aa;
                cin >> aa;
            }

            multimap<int, double>::iterator lower_landdd = loadedDataBenthos.mmapidparam3.lower_bound(
                    a_marine_landscape);
            multimap<int, double>::iterator upper_landdd = loadedDataBenthos.mmapidparam3.upper_bound(
                    a_marine_landscape);
            for (multimap<int, double>::iterator pos = lower_landdd; pos != upper_landdd; pos++) {
                outc(cout << pos->second << endl);
                // logistic recovery rates for this group specific to this landscape
                init_benthos_biomass_carrying_capacity_K_per_landscape_per_funcgr.push_back(pos->second);
            }
        } else {
            if (dyn_pop_sce.option(Options::modelBenthosInN)) {
                multimap<int, double>::iterator lower_land = loadedDataBenthos.mmapidparam4.lower_bound(
                        a_marine_landscape);
                multimap<int, double>::iterator upper_land = loadedDataBenthos.mmapidparam4.upper_bound(
                        a_marine_landscape);
                for (multimap<int, double>::iterator pos = lower_land; pos != upper_land; pos++) {
                    outc(cout << pos->second << endl);
                    // biomass per cell for this group specific to this landscape
                    init_prop_funcgr_number_per_node.push_back(pos->second);
                }

                multimap<int, double>::iterator lower_landddd = loadedDataBenthos.mmapidparam5.lower_bound(
                        a_marine_landscape);
                multimap<int, double>::iterator upper_landddd = loadedDataBenthos.mmapidparam5.upper_bound(
                        a_marine_landscape);
                for (multimap<int, double>::iterator pos = lower_landddd; pos != upper_landddd; pos++) {
                    outc(cout << pos->second << endl);
                    // logistic recovery rates for this group specific to this landscape
                    init_benthos_number_carrying_capacity_K_per_landscape_per_funcgr.push_back(pos->second);
                }


            } else {

                multimap<int, double>::iterator lower_land2 = loadedDataBenthos.mmapidparam6.lower_bound(
                        a_marine_landscape);
                multimap<int, double>::iterator upper_land2 = loadedDataBenthos.mmapidparam6.upper_bound(
                        a_marine_landscape);

                for (multimap<int, double>::iterator pos = lower_land2; pos != upper_land2; pos++) {
                    outc(cout << "check this: " << pos->second << endl);
                    // biomass per cell for this group specific to this landscape
                    init_prop_funcgr_biomass_per_node.push_back(pos->second);
                }


                if (init_prop_funcgr_biomass_per_node.size() != p->nbbenthospops) {
                    cout << a_marine_landscape << " nb funcgr is " << init_prop_funcgr_biomass_per_node.size() <<
                         ": error for benthos file: the file is likely to get an extra blank space here. stop, remove and rerun."
                         << endl;
                    int aa;
                    cin >> aa;
                }

                multimap<int, double>::iterator lower_landdd = loadedDataBenthos.mmapidparam3.lower_bound(
                        a_marine_landscape);
                multimap<int, double>::iterator upper_landdd = loadedDataBenthos.mmapidparam3.upper_bound(
                        a_marine_landscape);
                for (multimap<int, double>::iterator pos = lower_landdd; pos != upper_landdd; pos++) {
                    outc(cout << pos->second << endl);
                    // logistic recovery rates for this group specific to this landscape
                    init_benthos_biomass_carrying_capacity_K_per_landscape_per_funcgr.push_back(pos->second);
                }


            }
        }


        multimap<int, double>::iterator lower_land3 = loadedDataBenthos.mmapidparam1.lower_bound(a_marine_landscape);
        multimap<int, double>::iterator upper_land3 = loadedDataBenthos.mmapidparam1.upper_bound(a_marine_landscape);
        for (multimap<int, double>::iterator pos = lower_land3; pos != upper_land3; pos++) {
            outc(cout << pos->second << endl);
            // biomass per cell for this group specific to this landscape
            init_meanw_funcgr_per_node.push_back(pos->second);
        }


        multimap<int, double>::iterator lower_landd = loadedDataBenthos.mmapidparam7.lower_bound(a_marine_landscape);
        multimap<int, double>::iterator upper_landd = loadedDataBenthos.mmapidparam7.upper_bound(a_marine_landscape);
        for (multimap<int, double>::iterator pos = lower_landd; pos != upper_landd; pos++) {
            outc(cout << pos->second << endl);
            // logistic recovery rates for this group specific to this landscape
            init_recovery_rates_per_funcgr.push_back(pos->second);
        }

        multimap<int, double>::iterator lower_land2 = loadedDataBenthos.mmapidparam8.lower_bound(a_marine_landscape);
        multimap<int, double>::iterator upper_land2 = loadedDataBenthos.mmapidparam8.upper_bound(a_marine_landscape);
        for (multimap<int, double>::iterator pos = lower_land2; pos != upper_land2; pos++) {
            outc(cout << pos->second << endl);
            // habitat_deltas_per_pop specific to this landscape
            init_h_betas_per_pop.push_back(pos->second);
        }


        // add e.g. 2 functional groups per shared
        // and init with an arbitrary biomass.
        // init_biomass will be distributed evenly among nodes
        // belonging to this particular landscape

        benthoss[landscape] = new Benthos(landscape,
                                          a_marine_landscape,
                                          p->nodes,
                                          init_prop_funcgr_biomass_per_node,
                                          init_prop_funcgr_number_per_node,
                                          init_meanw_funcgr_per_node,
                                          init_recovery_rates_per_funcgr,
                                          init_benthos_biomass_carrying_capacity_K_per_landscape_per_funcgr,
                                          init_benthos_number_carrying_capacity_K_per_landscape_per_funcgr,
                                          p->is_benthos_in_numbers,
                                          p->is_benthos_in_longevity_classes,
                                          init_h_betas_per_pop,
                                          loadedDataBenthos.mmapidparam2
        );
        //out(cout << "marine landscape for this benthos shared is " << benthoss.at(landscape)->get_marine_landscape() << endl);
        //out(cout <<"...and the biomass this node this func. grp is "  << benthoss.at(landscape)-> get_list_nodes().at(0)-> get_benthos_tot_biomass(0) << endl);

    }

    if (p->doConsistencyTest) {
        doBenthosConsistencyTest(benthoss);
    }

    return benthoss;
}

void ModelLoader::doBenthosConsistencyTest(std::vector<Benthos *> const &benthoss)
{
    // check
    for (unsigned int a_idx = 0; a_idx < p->nodes.size(); a_idx++) {
        dout(cout << "this node " << p->nodes.at(a_idx)->get_idx_node().toIndex() <<
                  " nb func. gr. " << p->nodes.at(a_idx)->get_benthos_tot_biomass().size() << endl);

        if (p->nodes.at(a_idx)->get_benthos_tot_biomass().size() != p->nbbenthospops) {
            cout
                    << "something wrong for benthos_tot_biomass here!...kill displace.exe and check consistency in landscape coding and benthos input files before trying again"
                    << endl;
            throw ConsistencyTestFailed("benthos_tot_biomass");
        }
    }

    // check the area distribution for benthos shared 0
    //vector<Node* > some_nodes= benthoss.at(0)-> get_list_nodes();
    //for(int a_idx=0; a_idx<some_nodes.size(); a_idx++){
    //    cout << some_nodes.at(a_idx)->get_idx_node() << endl;
    //}

    // check the biomasses
    vector<double> a_prop_funcgr_per_node = benthoss[0]->get_prop_funcgr_biomass_per_node();
    outc(cout << "check biomass per func. gr. for benthos shared 0  " << endl);
    for (unsigned int gr = 0; gr < a_prop_funcgr_per_node.size(); gr++) {
        outc(cout << a_prop_funcgr_per_node[gr] << " ");
    }
    outc(cout << endl);

    // check the biomasses for benthos shared 0 on the first node for the
    // first functional group
    //cout <<"...and the biomass this node this func. grp is "  <<
    //     benthoss.at(0)-> get_list_nodes().at(0)-> get_benthos_tot_biomass(0) << endl;

    //cout <<"...and the biomass this node this func. grp is "  <<
    //     benthoss.at(0)-> get_list_nodes().at(100)-> get_benthos_tot_biomass(1) << endl;

    //cout <<"...and the biomass this node this func. grp is "  <<
    //     benthoss.at(4)-> get_list_nodes().at(100)-> get_benthos_tot_biomass(1) << endl;

}

bool ModelLoader::loadConfig(int &nbpops, int &nbbenthospops, std::vector<int> &implicit_pops,
                             std::vector<int> &implicit_pops_level2, std::vector<int> &grouped_tacs,
                             std::vector<int> &nbcp_coupling_pops, std::vector<double> &calib_oth_landings,
                             std::vector<double> &calib_w, std::vector<double> &calib_cpue,
                             std::vector<types::NodeId> &interesting_harbours)
{
    p->configLoaded = loadConfigImpl(nbpops,
                                     nbbenthospops,
                                     implicit_pops,
                                     implicit_pops_level2,
                                     grouped_tacs,
                                     nbcp_coupling_pops,
                                     calib_oth_landings,
                                     calib_w,
                                     calib_cpue,
                                     interesting_harbours);
    return p->configLoaded;
}

bool ModelLoader::loadScenario(displace::commons::Scenario &scenario)
{
    p->scenarioLoaded = loadScenarioImpl(scenario);
    return p->scenarioLoaded;
}
