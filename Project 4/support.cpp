#include "support.h"
#include  <string>

std::string toLower(std::string original) {
	for (int i = 0; i < original.size(); i++) {
		original[i] = tolower(original[i]);
	}
	return original;
}
bool arrived(const StreetSegment& ss, std::vector<StreetSegment>& vss) {
	for (auto item : vss)
		if (item.segment.end.latitude == ss.segment.end.latitude && item.segment.end.longitude == ss.segment.end.longitude) return true;
	return false;
}

bool operator==(const StreetSegment& lhs, const StreetSegment& rhs) {
	return	 lhs.segment.end.latitude == rhs.segment.end.latitude
		&& lhs.segment.end.longitude == rhs.segment.end.longitude
		&& lhs.segment.start.latitude == rhs.segment.start.latitude
		&& lhs.segment.start.longitude == rhs.segment.start.longitude;
}
bool operator!=(const StreetSegment& lhs, const StreetSegment& rhs) {
	return !(lhs == rhs);
}
bool operator>(const GeoSegment& lhs, const GeoSegment& rhs) {
	if (lhs.start.latitude > rhs.start.latitude) return true;
	else if (lhs.start.latitude < rhs.start.latitude) return false;
	
	if (lhs.start.longitude > rhs.start.longitude) return true;
	else if (lhs.start.longitude < rhs.start.longitude) return false;

	if (lhs.end.latitude > rhs.end.latitude) return true;
	else if (lhs.end.latitude < rhs.end.latitude) return false;

	else return lhs.end.longitude > rhs.end.longitude;

}

bool operator==(const GeoSegment& lhs, const GeoSegment& rhs) {
	return lhs.start == rhs.start && lhs.end == rhs.end;
}


bool operator==(const GeoCoord& lhs, const GeoCoord& rhs) {
	return lhs.latitude == rhs.latitude && lhs.longitude == rhs.longitude;
}

std::string directionOf(double angle) {
	if (0 <= angle && angle <= 22.5) return "east";
	if (22.5 < angle &&angle <= 67.5) return "northeast";
	if (67.5 < angle &&angle <= 112.5) return "north";
	if (112.5 < angle && angle<= 157.5) return "northwest";
	if (157.5 < angle && angle <= 202.5) return "west";
	if (202.5 < angle && angle <= 247.5) return "southwest";
	if (247.5 < angle && angle <= 292.5) return "south";
	if (292.5 < angle && angle <= 337.5) return "southeast";
	if (337.5 < angle && angle < 360) return "east";
	return "no dir";



}