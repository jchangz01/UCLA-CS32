#include "provided.h"
#include <vector>
using namespace std;

class DeliveryOptimizerImpl
{
public:
    DeliveryOptimizerImpl(const StreetMap* sm);
    ~DeliveryOptimizerImpl();
    void optimizeDeliveryOrder(
        const GeoCoord& depot,
        vector<DeliveryRequest>& deliveries,
        double& oldCrowDistance,
        double& newCrowDistance) const;
private:
    double calcCrowDistance(const vector<DeliveryRequest>& reqs, const GeoCoord& start) const;
    const StreetMap* sMap;
};

DeliveryOptimizerImpl::DeliveryOptimizerImpl(const StreetMap* sm)
    :sMap(sm)
{
}

DeliveryOptimizerImpl::~DeliveryOptimizerImpl()
{
}

double DeliveryOptimizerImpl::calcCrowDistance(const vector<DeliveryRequest>& reqs, const GeoCoord& start) const
{
    double crowDist = 0;
    crowDist += distanceEarthMiles(start, reqs[0].location);

    for (int i = 0; i < reqs.size() - 1; i++)
        crowDist += distanceEarthMiles(reqs[i].location, reqs[i + 1].location);

    crowDist += distanceEarthMiles(reqs.back().location, start);
    return crowDist;
}

void DeliveryOptimizerImpl::optimizeDeliveryOrder(
    const GeoCoord& depot,
    vector<DeliveryRequest>& deliveries,
    double& oldCrowDistance,
    double& newCrowDistance) const
{
    oldCrowDistance = 0;
    newCrowDistance = 0;

    if (deliveries.empty())
        return;

    ///////////////////////////
    //calculate oldCrowDistance
    ///////////////////////////
    oldCrowDistance = calcCrowDistance(deliveries, depot);
    vector<DeliveryRequest> oldCrowOrder = deliveries;

    ///////////////////////////////////////
    //process and calculate newCrowDistance
    ///////////////////////////////////////
    vector<DeliveryRequest> newCrowOrder;
    GeoCoord currentLoc = depot;
    while (!deliveries.empty())
    {
        GeoCoord empty;
        DeliveryRequest closestReqtoCurrent("", empty);
        int vecPosition = 0;
        double closestDisttoCurrent = 1000000000000000;
        for (int i = 0; i < deliveries.size(); i++)
        {
            if (distanceEarthMiles(currentLoc, deliveries[i].location) < closestDisttoCurrent)
            {
                closestDisttoCurrent = distanceEarthMiles(currentLoc, deliveries[i].location);
                closestReqtoCurrent = deliveries[i];
                vecPosition = i;
            }
        }
        newCrowOrder.push_back(closestReqtoCurrent);
        deliveries.erase(deliveries.begin() + vecPosition);
        currentLoc = closestReqtoCurrent.location;
    }

    newCrowDistance = calcCrowDistance(newCrowOrder, depot);

    if (newCrowDistance < oldCrowDistance)
        deliveries = newCrowOrder;
    else
        deliveries = oldCrowOrder;
}

//******************** DeliveryOptimizer functions ****************************

// These functions simply delegate to DeliveryOptimizerImpl's functions.
// You probably don't want to change any of this code.

DeliveryOptimizer::DeliveryOptimizer(const StreetMap* sm)
{
    m_impl = new DeliveryOptimizerImpl(sm);
}

DeliveryOptimizer::~DeliveryOptimizer()
{
    delete m_impl;
}

void DeliveryOptimizer::optimizeDeliveryOrder(
    const GeoCoord& depot,
    vector<DeliveryRequest>& deliveries,
    double& oldCrowDistance,
    double& newCrowDistance) const
{
    return m_impl->optimizeDeliveryOrder(depot, deliveries, oldCrowDistance, newCrowDistance);
}
