#include "provided.h"
#include "ExpandableHashMap.h"
#include "support.h"
#include <list>
#include <queue>
#include <set>
using namespace std;

class PointToPointRouterImpl
{
public:
	PointToPointRouterImpl(const StreetMap* sm);
	~PointToPointRouterImpl();
	DeliveryResult generatePointToPointRoute(
		const GeoCoord& start,
		const GeoCoord& end,
		list<StreetSegment>& route,
		double& totalDistanceTravelled) const;
private:
	list<StreetSegment> generateListRoute(const GeoCoord& start, vector<GeoCoord> encounteredCoords, double& totalDistance) const;
	const StreetMap* m_sMap;
};

struct compare
{
	compare(ExpandableHashMap<GeoCoord, double>* m) { f_N = m; }
	bool operator() (const GeoCoord& g1, const GeoCoord& g2)
	{
		return *f_N->find(g1) > * f_N->find(g2);
	}

	ExpandableHashMap<GeoCoord, double>* f_N;
};

PointToPointRouterImpl::PointToPointRouterImpl(const StreetMap* sm)
	:m_sMap(sm)
{
}

PointToPointRouterImpl::~PointToPointRouterImpl()
{
}

DeliveryResult PointToPointRouterImpl::generatePointToPointRoute(
	const GeoCoord& start,
	const GeoCoord& end,
	list<StreetSegment>& route,
	double& totalDistanceTravelled) const
{
	totalDistanceTravelled = 0;
	route.clear();

	if (start == end)
		return DELIVERY_SUCCESS;

	vector <StreetSegment> potSegments;
	if (!m_sMap->getSegmentsThatStartWith(end, potSegments))
		return BAD_COORD;

	ExpandableHashMap<GeoCoord, double> gScore; //track gScore of every child coord
	gScore.associate(start, 0);

	ExpandableHashMap<GeoCoord, double> fScore; //track fScore of every child coord
	fScore.associate(start, calch_N(start, end));

	priority_queue<GeoCoord, vector<GeoCoord>, compare> openSet(&fScore); //priority queue used for lower time complexities
	openSet.push(start);

	set<GeoCoord> includedInSet; //Used alongside a priority queue to track if a coord is inside the open set
	includedInSet.insert(start);

	vector<GeoCoord> routeCoords; //track the route of starting coord to ending coord

	while (!openSet.empty())
	{
		GeoCoord current = openSet.top();
		/////////////////////////////////////////////
		//Check if current is at the goal destination
		/////////////////////////////////////////////
		if (current == end)
		{
			routeCoords.push_back(current);
			route = generateListRoute(start, routeCoords, totalDistanceTravelled);
			return DELIVERY_SUCCESS;
		}

		openSet.pop();
		includedInSet.erase(current);
		routeCoords.push_back(current);

		/////////////////
		//A * Magic 
		/////////////////
		if (m_sMap->getSegmentsThatStartWith(current, potSegments))
		{
			for (int i = 0; i < potSegments.size(); i++)
			{
				double tentative_gScore = distanceEarthMiles(current, potSegments[i].end);
				if (gScore.find(potSegments[i].end) == nullptr || tentative_gScore < *gScore.find(potSegments[i].end))
				{
					gScore.associate(potSegments[i].end, tentative_gScore);
					fScore.associate(potSegments[i].end, *gScore.find(potSegments[i].end) + calch_N(potSegments[i].end, end));

					if (includedInSet.find(potSegments[i].end) != includedInSet.end())
						continue;

					openSet.push(potSegments[i].end);
					includedInSet.insert(potSegments[i].end);
				}
			}
		}
		else
			return BAD_COORD;
	}
	return NO_ROUTE;
}

list <StreetSegment> PointToPointRouterImpl::generateListRoute(const GeoCoord& start, vector<GeoCoord> encounteredCoords, double& totalDistance) const
{
	list <StreetSegment> route;
	bool connected = false;
	for (int i = encounteredCoords.size() - 1; i > 0; i--)
	{
		vector <StreetSegment> connectedStreets;
		m_sMap->getSegmentsThatStartWith(encounteredCoords[i], connectedStreets);
		for (int j = 0; j < connectedStreets.size(); j++)
		{
			connected = false;
			if (connectedStreets[j].end == encounteredCoords[i - 1])
			{
				StreetSegment seg (connectedStreets[j].end, encounteredCoords[i], connectedStreets[j].name);
				route.push_back(seg);
				totalDistance += distanceEarthMiles(encounteredCoords[i], connectedStreets[j].end);
				if (connectedStreets[j].end == start)
				{
					route.reverse();
					return route;
				}
				connected = true;
				break;
			}
		}
		while (!connected)
		{
			encounteredCoords.erase(encounteredCoords.begin() + i - 1);
			i--;
			for (int j = 0; j < connectedStreets.size(); j++)
			{
				if (connectedStreets[j].end == encounteredCoords[i - 1])
				{
					StreetSegment seg(connectedStreets[j].end, encounteredCoords[i], connectedStreets[j].name);
					route.push_back(seg);
					totalDistance += distanceEarthMiles(encounteredCoords[i], connectedStreets[j].end);
					if (connectedStreets[j].end == start)
					{
						route.reverse();
						return route;
					}
					connected = true;
					break;
				}
			}
		}
	}
	return route;
}
//******************** PointToPointRouter functions ***************************

// These functions simply delegate to PointToPointRouterImpl's functions.
// You probably don't want to change any of this code.

PointToPointRouter::PointToPointRouter(const StreetMap* sm)
{
	m_impl = new PointToPointRouterImpl(sm);
}

PointToPointRouter::~PointToPointRouter()
{
	delete m_impl;
}

DeliveryResult PointToPointRouter::generatePointToPointRoute(
	const GeoCoord& start,
	const GeoCoord& end,
	list<StreetSegment>& route,
	double& totalDistanceTravelled) const
{
	return m_impl->generatePointToPointRoute(start, end, route, totalDistanceTravelled);
}
