#include "provided.h"
#include <string>
#include <fstream>
#include <iostream>
using namespace std;

class MapLoaderImpl
{
public:
	MapLoaderImpl();
	~MapLoaderImpl();
	bool load(string mapFile);
	size_t getNumSegments() const;
	bool getSegment(size_t segNum, StreetSegment& seg) const;
private:
	vector<StreetSegment> streetsegments;
};

MapLoaderImpl::MapLoaderImpl(): streetsegments()
{
}

MapLoaderImpl::~MapLoaderImpl()
{
}

bool MapLoaderImpl::load(string mapFile)
{
	enum Line { name, coordinates, c };

	ifstream infile(mapFile);    // infile is a name of our choosing
	if (!infile)		        // Did opening the file fail?
	{
		cerr << "Error: Cannot open data.txt!" << endl;
		return false;
	}

	string s;
	Line line = name;

	while (getline(infile, s)) {

		if (line == name) streetsegments.push_back(StreetSegment());

		StreetSegment& printed = streetsegments.back();
		if (line == name) {
			printed.streetName = s;
			line = (Line)(line + 1);
		}
		else if (line == coordinates)
		{
			int delim = s.find(',');
			string lat = s.substr(0, delim);
			s = s.substr(delim + 1); //", "
			while (s[0] == ' ') s = s.substr(1);
			delim = s.find(' ');
			string lon = s.substr(0, delim);
			printed.segment.start = GeoCoord(lat, lon);
			s = s.substr(delim + 1);

			delim = s.find(',');
			lat = s.substr(0, delim);
			lon = s.substr(delim + 1); //","
			printed.segment.end = GeoCoord(lat, lon);

			line = (Line)(line + 1);
		}
		else {
			int numAttractions = stoi(s);
			for (int i = 0; i < numAttractions; i++)
			{
				if (getline(infile, s)) {
					printed.attractions.push_back(Attraction());

					int delim = s.find('|');
					string aName = s.substr(0, delim);
					printed.attractions.back().name = aName;

					s = s.substr(delim + 1);
					delim = s.find(',');
					string lat = s.substr(0, delim);
					string lon = s.substr(delim + 1); //", "
					while (s[0] == ' ') s = s.substr(1);
					printed.attractions.back().geocoordinates = GeoCoord(lat, lon);
				}
			}
			line = name;
		}
	}
	return true;
}

size_t MapLoaderImpl::getNumSegments() const
{
	return streetsegments.size(); // This compiles, but may not be correct
}

bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const
{
	if(segNum > getNumSegments())
	return false;  // This compiles, but may not be correct
	seg = streetsegments[segNum];
	return true;
}

//******************** MapLoader functions ************************************

// These functions simply delegate to MapLoaderImpl's functions.
// You probably don't want to change any of this code.

MapLoader::MapLoader()
{
	m_impl = new MapLoaderImpl;
}

MapLoader::~MapLoader()
{
	delete m_impl;
}

bool MapLoader::load(string mapFile)
{
	return m_impl->load(mapFile);
}

size_t MapLoader::getNumSegments() const
{
	return m_impl->getNumSegments();
}

bool MapLoader::getSegment(size_t segNum, StreetSegment& seg) const
{
   return m_impl->getSegment(segNum, seg);
}
