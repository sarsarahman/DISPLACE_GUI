#include <dtree/variables.h>

using namespace dtree ;

const int VariableNames::VERSION = 4;

/** \brief The names if the variables, as used in the decision tree files.
 * When changing this array, changes must account relative changes to VariableNames::bins and Variable .
 * \note The name must not contain spaces or other character used as separator by the parsers.
 * */
const char *const VariableNames::names[] = {
    "arrived_on_the_ground",
    "bycatch_risk",
    "catch_volume",
    "distance_to_port",
    "feeling_for_higher_catches_elsewhere",
    "fish_detection_with_echosounder",
    "fish_price",
    "fish_price_is",
    "high_potential_catch",
    "last_trip_on_the_ground",
    "lastTripProfitIs",
    "lastTripRevenueIs",
    "remaining_quota_is",
    "suitable_bottom_detection",
    "weather_is",

    "probability",
    // keep this last
    0
};

#if ((__cplusplus >= 201103L))
const std::vector<std::string> VariableNames::bins[] = {
    { "yes", "no" },
    { "low", "high" },
    { "low", "high" },
    { "near", "far" },
    { "yes", "no" },
    { "yes", "no" },
    { "low", "high" },
    { "low", "high" },
    { "yes", "no" },
    { "good", "bad" },
    { "below", "above"},    //lastTripProfitIs
    { "below", "above"},    //lastTripRevenueIs
    { "low", "high" },
    { "low", "high" },
    { "good", "bad" },

    { },  // probability
    // keep this last
};
#endif

const char *VariableNames::variableName(Variable id)
{
    if ( id < 0 || id >= VarLast)
        return 0;

    return names[id];
}

Variable VariableNames::variableCode(const std::string &name)
{
    int i = 0;
    while (names[i] != 0) {
        if (names[i] == name)
            return static_cast<Variable>(i);
        ++i;
    }

    return VarUndefined;
}
