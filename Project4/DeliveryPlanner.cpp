#include "provided.h"
#include <vector>
using namespace std;

class DeliveryPlannerImpl
{
public:
    DeliveryPlannerImpl(const StreetMap* sm);
    ~DeliveryPlannerImpl();
    DeliveryResult generateDeliveryPlan(
        const GeoCoord& depot,
        const vector<DeliveryRequest>& deliveries,
        vector<DeliveryCommand>& commands,
        double& totalDistanceTravelled) const;
private:
    void processReturnDepotCommands(const list<StreetSegment>& route, vector<DeliveryCommand>& commands) const;
    void processDeliveryCommands(const list<StreetSegment>& route, vector<DeliveryCommand>& commands, string item) const;
    string getDirectionofStreetSegment(const StreetSegment& proceed) const;
    string getTurnDirection(const StreetSegment& current, const StreetSegment& next) const;
    const StreetMap* m_sMap;
    DeliveryOptimizer m_dOptimizer;
    PointToPointRouter m_pRouter;
};

DeliveryPlannerImpl::DeliveryPlannerImpl(const StreetMap* sm)
    :m_sMap(sm), m_dOptimizer(sm), m_pRouter(sm)
{
}

DeliveryPlannerImpl::~DeliveryPlannerImpl()
{
}

DeliveryResult DeliveryPlannerImpl::generateDeliveryPlan(
    const GeoCoord& depot,
    const vector<DeliveryRequest>& deliveries,
    vector<DeliveryCommand>& commands,
    double& totalDistanceTravelled) const
{
    if (deliveries.empty())
        return DELIVERY_SUCCESS;

    //reset distance and commands 
    commands.clear();
    totalDistanceTravelled = 0;

    double newCrow, oldCrow;
    vector<DeliveryRequest> optimizedDeliveries = deliveries;
    m_dOptimizer.optimizeDeliveryOrder(depot, optimizedDeliveries, oldCrow, newCrow);

    ////////////////////////
    //Process every request 
    ///////////////////////
    GeoCoord currentLoc = depot;
    list<StreetSegment> path;
    double distanceTraveled;
    for (int i = 0; i < optimizedDeliveries.size(); i++)
    {
        distanceTraveled = 0;
        if (m_pRouter.generatePointToPointRoute(currentLoc, optimizedDeliveries[i].location, path, distanceTraveled) != DELIVERY_SUCCESS)
            return m_pRouter.generatePointToPointRoute(currentLoc, optimizedDeliveries[i].location, path, distanceTraveled);
        processDeliveryCommands(path, commands, optimizedDeliveries[i].item);
        currentLoc = optimizedDeliveries[i].location;
        totalDistanceTravelled += distanceTraveled;
    }
    
    if (m_pRouter.generatePointToPointRoute(optimizedDeliveries[optimizedDeliveries.size() - 1].location, depot, path, distanceTraveled) != DELIVERY_SUCCESS)
        return m_pRouter.generatePointToPointRoute(optimizedDeliveries[optimizedDeliveries.size() - 1].location, depot, path, distanceTraveled);
    processReturnDepotCommands(path, commands);
    totalDistanceTravelled += distanceTraveled;
    return DELIVERY_SUCCESS;
}

void DeliveryPlannerImpl::processReturnDepotCommands(const list<StreetSegment>& route, vector<DeliveryCommand>& commands) const
{
    //check if we are already at depot
    if (route.empty())
        return;

    //proceed straight from straight from starting street
    list<StreetSegment>::const_iterator itr = route.begin();
    double currentDist = 0;
    string currentStreetName = itr->name;
    string currentDirection = getDirectionofStreetSegment(*itr);
    itr = route.end();
    itr--;
    GeoCoord endLoc = itr->end;
    StreetSegment prev;

    for (itr = route.begin(); itr != route.end(); itr++)
    {
        if (itr->name == currentStreetName)
        {
            currentDist += distanceEarthMiles(itr->start, itr->end);
        }
        else
        {
            commands.resize(commands.size() + 1);
            commands.back().initAsProceedCommand(currentDirection, currentStreetName, currentDist);

            string turnDirection = getTurnDirection(prev, *itr);
            currentDist = 0;
            currentStreetName = itr->name;
            if (turnDirection != "proceed")
            {
                commands.resize(commands.size() + 1);
                commands.back().initAsTurnCommand(turnDirection, currentStreetName);
            }
            currentDirection = getDirectionofStreetSegment(*itr);
            currentDist += distanceEarthMiles(itr->start, itr->end);
        }
        prev = *itr; //track previous streetsegment
    }
    commands.resize(commands.size() + 1);
    commands.back().initAsProceedCommand(currentDirection, currentStreetName, currentDist);
}
void DeliveryPlannerImpl::processDeliveryCommands(const list<StreetSegment>& route, vector<DeliveryCommand>& commands, string item) const
{
    //check if we are already at delivery location
    if (route.empty())
    {
        commands.resize(commands.size() + 1);
        commands.back().initAsDeliverCommand(item);
        return;
    } 

    //proceed straight from straight from starting street
    list<StreetSegment>::const_iterator itr = route.begin();
    double currentDist = 0;
    string currentStreetName = itr->name;
    string currentDirection = getDirectionofStreetSegment(*itr);
    itr = route.end();
    itr--;
    GeoCoord endLoc = itr->end;
    StreetSegment prev;

    for (itr = route.begin(); itr != route.end(); itr++)
    {
        if (itr->name == currentStreetName)
        {
            currentDist += distanceEarthMiles(itr->start, itr->end);
        }
        else
        {
            commands.resize(commands.size() + 1);
            commands.back().initAsProceedCommand(currentDirection, currentStreetName, currentDist);

            string turnDirection = getTurnDirection(prev, *itr);
            currentDist = 0;
            currentStreetName = itr->name;
            if (turnDirection != "proceed")
            {
                commands.resize(commands.size() + 1);
                commands.back().initAsTurnCommand(turnDirection, currentStreetName);
            }
            currentDirection = getDirectionofStreetSegment(*itr);
            currentDist += distanceEarthMiles(itr->start, itr->end);
        }
        prev = *itr; //track previous streetsegment
    }
    commands.resize(commands.size() + 1);
    commands.back().initAsProceedCommand(currentDirection, currentStreetName, currentDist);
    commands.resize(commands.size() + 1);
    commands.back().initAsDeliverCommand(item);
}

string DeliveryPlannerImpl::getDirectionofStreetSegment(const StreetSegment& proceed) const
{
    double angle = angleOfLine(proceed);
    if (angle >= 0 && angle < 22.5)
        return "east";
    else if (angle >= 22.5 && angle < 77.5)
        return "northeast";
    else if (angle >= 77.5 && angle < 112.5)
        return "north";
    else if (angle >= 112.5 && angle < 157.5)
        return "northwest";
    else if (angle >= 157.5 && angle < 202.5)
        return "west";
    else if (angle >= 202.5 && angle < 247.5)
        return "southwest";
    else if (angle >= 247.5 && angle < 292.5)
        return "south";
    else if (angle >= 292.5 && angle < 337.5)
        return "southeast";
    else if (angle >= 337.5)
        return "east";
    else
        return "error!";
}

string DeliveryPlannerImpl::getTurnDirection(const StreetSegment& current, const StreetSegment& next) const
{
    double angle = angleBetween2Lines(current, next);
    if (angle < 1 || angle > 359)
        return "proceed";
    else if (angle >= 1 && angle < 180)
        return "left";
    else if (angle >= 180 && angle <= 359)
        return "right";
    else
        return "error!";
}
//******************** DeliveryPlanner functions ******************************

// These functions simply delegate to DeliveryPlannerImpl's functions.
// You probably don't want to change any of this code.

DeliveryPlanner::DeliveryPlanner(const StreetMap* sm)
{
    m_impl = new DeliveryPlannerImpl(sm);
}

DeliveryPlanner::~DeliveryPlanner()
{
    delete m_impl;
}

DeliveryResult DeliveryPlanner::generateDeliveryPlan(
    const GeoCoord& depot,
    const vector<DeliveryRequest>& deliveries,
    vector<DeliveryCommand>& commands,
    double& totalDistanceTravelled) const
{
    return m_impl->generateDeliveryPlan(depot, deliveries, commands, totalDistanceTravelled);
}
