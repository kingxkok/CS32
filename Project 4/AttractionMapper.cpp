
#include "provided.h"
#include "MyMap.h"
#include <string>
#include "support.h"
using namespace std;



class AttractionMapperImpl
{
public:
	AttractionMapperImpl();
	~AttractionMapperImpl();
	void init(const MapLoader& ml);
	bool getGeoCoord(string attraction, GeoCoord& gc) const;
private:
	MyMap<string, GeoCoord>  allAttractions;
	

};

AttractionMapperImpl::AttractionMapperImpl() : allAttractions()
{
}

AttractionMapperImpl::~AttractionMapperImpl()
{

}

void AttractionMapperImpl::init(const MapLoader& ml)
{
	StreetSegment segment;
	for (int i = 0; i < ml.getNumSegments(); i++) {
		ml.getSegment(i, segment);
		for (auto attraction : segment.attractions)
			allAttractions.associate(toLower(attraction.name), attraction.geocoordinates);
	}
}

bool AttractionMapperImpl::getGeoCoord(string attraction, GeoCoord& gc) const
{
	const GeoCoord* gcp = allAttractions.find(toLower(attraction));
	if(gcp == nullptr)
		return false;  
	//found!
	gc = *gcp;
	return true;
}

//******************** AttractionMapper functions *****************************

// These functions simply delegate to AttractionMapperImpl's functions.
// You probably don't want to change any of this code.

AttractionMapper::AttractionMapper()
{
	m_impl = new AttractionMapperImpl;
}

AttractionMapper::~AttractionMapper()
{
	delete m_impl;
}

void AttractionMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

bool AttractionMapper::getGeoCoord(string attraction, GeoCoord& gc) const
{
	return m_impl->getGeoCoord(attraction, gc);
}
