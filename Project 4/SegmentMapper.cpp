#include "provided.h"
#include "MyMap.h"
#include <vector>
using namespace std;

class SegmentMapperImpl
{
public:
	SegmentMapperImpl();
	~SegmentMapperImpl();
	void init(const MapLoader& ml);
	vector<StreetSegment> getSegments(const GeoCoord& gc) const;
private:
	
	 double  myGC(const GeoCoord& gc) const {
		 return (gc.latitude * 1000 * 1000 * 1000 * 1000) + gc.longitude; //10^12 so should be no overlap since lat is only max 10 sig figs
	}

	void add(double gc, StreetSegment segment) {
		auto ssvp = allGCs.find(gc); //street segment vector pointer
		if (ssvp == nullptr) allGCs.associate(gc, vector<StreetSegment>(1, segment));
		else ssvp->push_back(segment);
	}

	MyMap<double, vector<StreetSegment>> allGCs;

};

SegmentMapperImpl::SegmentMapperImpl()
{
}

SegmentMapperImpl::~SegmentMapperImpl()
{
}

void SegmentMapperImpl::init(const MapLoader& ml)
{
	StreetSegment segment;
	for (int i = 0; i < ml.getNumSegments(); i++) {
		ml.getSegment(i, segment);
		add( myGC(segment.segment.start), segment); 
		add( myGC(segment.segment.end), segment);
		for (auto attraction : segment.attractions) 
			add(myGC(attraction.geocoordinates), segment);
	}
}

vector<StreetSegment> SegmentMapperImpl::getSegments(const GeoCoord& gc) const
{
	return  *( allGCs.find(myGC(gc)) );
}

//******************** SegmentMapper functions ********************************

// These functions simply delegate to SegmentMapperImpl's functions.
// You probably don't want to change any of this code.

SegmentMapper::SegmentMapper()
{
	m_impl = new SegmentMapperImpl;
}

SegmentMapper::~SegmentMapper()
{
	delete m_impl;
}

void SegmentMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

vector<StreetSegment> SegmentMapper::getSegments(const GeoCoord& gc) const
{
	return m_impl->getSegments(gc);
}
