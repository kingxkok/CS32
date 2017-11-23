#ifndef SUPPORT
#define SUPPORT

#include <string>
#include <cctype>
#include <vector>
#include "provided.h"


std::string toLower(std::string original);
bool arrived(const StreetSegment& ss, std::vector<StreetSegment>& vss);
 bool operator==(const StreetSegment& lhs, const StreetSegment& rhs);
 bool operator!=(const StreetSegment& lhs, const StreetSegment& rhs);
 //MyMap needs > 
 bool operator>(const GeoSegment& lhs, const GeoSegment& rhs);
 bool operator==(const GeoSegment& lhs, const GeoSegment& rhs);
 bool operator==(const GeoCoord& lhs, const GeoCoord& rhs);

 //finds N NE E SE etc from 0<=angle<360
 std::string directionOf(double angle);  



#endif // !SUPPORT

