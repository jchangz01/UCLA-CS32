#include "provided.h"
#include "ExpandableHashMap.h"
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <functional>

using namespace std;

unsigned int hasher(const GeoCoord& g)
{
	return std::hash<string>()(g.latitudeText + g.longitudeText);
}
unsigned int hasher(const int& key)
{
	return key;
}

class StreetMapImpl
{
public:
	StreetMapImpl();
	~StreetMapImpl();
	bool load(string mapFile);
	bool getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const;
private:
	ExpandableHashMap<GeoCoord, vector<StreetSegment>> coordToStreet;
};

StreetMapImpl::StreetMapImpl()
	:coordToStreet(0.5)
{
}

StreetMapImpl::~StreetMapImpl()
{
}

bool StreetMapImpl::load(string mapFile)
{
	ifstream inf(mapFile);
	if (!inf)
		return false;

	int segmentCount = 0; //number of segments in a given street
	int segmentRemain = 0; //number of segments remaining for a given street
	string streetName = ""; //used to store streets name
	string slat, slong, elat, elong; //used to store the starting/ending latitude and longitude
	StreetSegment segR; //used to track the reverse street segments
	string line;
	while (getline(inf, line))
	{
		istringstream iss(line);
		if (segmentRemain == 0 && streetName.size() < 1) //case for getting streetname
		{
			streetName = line;
		}
		else if (segmentRemain == 0) //case for getting segmentcount
		{
			iss >> segmentCount;
			segmentRemain = segmentCount;
		}
		else //case for processing lat and long of street segments
		{
			iss >> slat; //get starting latitude
			iss >> slong; //get starting longitude
			iss >> elat; //get ending latitude
			iss >> elong; //get ending longitude
			GeoCoord start(slat, slong);
			GeoCoord end(elat, elong);
			StreetSegment segN(start, end, streetName);
			vector<StreetSegment> streetSegs;

			if (segmentRemain != segmentCount)
			{
				streetSegs.push_back(segR);
				if (segR.start != start)
				{
					if (coordToStreet.find(segR.start))
					{
						vector<StreetSegment> randSegs = *coordToStreet.find(segR.start);
						for (int i = 0; i < randSegs.size(); i++)
							streetSegs.push_back(randSegs[i]);
					}
					coordToStreet.associate(segR.start, streetSegs);
					streetSegs.clear();
				}
			}

			streetSegs.push_back(segN);

			if (coordToStreet.find(start) != nullptr) //adds previous street segments (if any) associated with a geoCoord to the current street segments
			{
				vector<StreetSegment> prevSegs = *coordToStreet.find(start);
				for (int i = 0; i < prevSegs.size(); i++)
					streetSegs.push_back(prevSegs[i]);
			}

			coordToStreet.associate(start, streetSegs);

			//assigning reverse values of a given street segment
			segR.start = segN.end;
			segR.end = segN.start;
			segR.name = segN.name;

			segmentRemain--;
			if (segmentRemain == 0)
			{
				vector<StreetSegment> finalStreetSegs;
				if (coordToStreet.find(end) != nullptr) //adds previous street segments (if any) associated with a geoCoord to the current street segments
				{
					vector<StreetSegment> prevSegs = *coordToStreet.find(end);
					for (int i = 0; i < prevSegs.size(); i++)
						finalStreetSegs.push_back(prevSegs[i]);
				}
				finalStreetSegs.push_back(segR);
				coordToStreet.associate(end, finalStreetSegs);
				streetName = "";
				segmentCount = 0;
			}
		}
	}
	return true;
}

bool StreetMapImpl::getSegmentsThatStartWith(const GeoCoord& gc, std::vector<StreetSegment>& segs) const
{
	if (coordToStreet.find(gc))
	{
		segs = *coordToStreet.find(gc);
		return true;
	}
	else
		return false;
}

//******************** StreetMap functions ************************************

// These functions simply delegate to StreetMapImpl's functions.
// You probably don't want to change any of this code.

StreetMap::StreetMap()
{
	m_impl = new StreetMapImpl;
}

StreetMap::~StreetMap()
{
	delete m_impl;
}

bool StreetMap::load(std::string mapFile)
{
	return m_impl->load(mapFile);
}

bool StreetMap::getSegmentsThatStartWith(const GeoCoord& gc, std::vector<StreetSegment>& segs) const
{
	return m_impl->getSegmentsThatStartWith(gc, segs);
}