//Kevin Kim
//CS330

#include "closestpair.h"
#include <algorithm>
#include <limits>
#include <cmath>
#include <iostream>
#include <utility>

/***************
    Utilities
 ***************/
float GetDistanceSQ(const Point& lhs, const Point& rhs)
{
	const float x = std::abs(lhs.x - rhs.x);
	const float y = std::abs(lhs.y - rhs.y);
	return x * x + y * y;
}
bool compareX(const Point& p, const Point& q)
{
	return p.x < q.x;
}
std::pair<Point, Point> MakeMidBox(const Point& p, float square_dist)
{
	double dist = std::sqrt(square_dist);
	
	std::pair<Point, Point> box{
				{ p.x,                           static_cast<float>(p.y + dist) },
				{ static_cast<float>(p.x + dist),  static_cast<float>(p.y - dist) }
	};
	return box;
}
bool IsInsideBox(const Point& input, const Point& up_left, const Point& bottom_right)
{
	return (   up_left.x <= input.x 
			&& up_left.y >= input.y 
			&& bottom_right.x >= input.x
			&& bottom_right.y <= input.y);
}
bool ShouldBeChecked(float pX, float midX, float distance)
{
	return (std::abs(pX - midX) <= distance);
}
float GetShortestInBox(std::vector<Point>list, float min)
{
	int size = list.size();
	for (int i = 0; i < size; ++i)
	{
		for (int j = i + 1; j < size && (list.at(j).y - list.at(i).y) < min; ++j)
		{
			if (GetDistanceSQ(list.at(j), list.at(i)) < min)
			{
				min = GetDistanceSQ(list.at(j), list.at(i));
			}
		}
	}
	return min;
}
std::ostream& operator<< (std::ostream& os, Point const& p)
{
	os << "(" << p.x << " , " << p.y << ") ";
	return os;
}
std::istream& operator>> (std::istream& os, Point & p)
{
	os >> p.x >> p.y;
	return os;
}

////////////////////////////////////////////////////////////////////////////////
float closestPair_aux (std::vector<Point>::iterator first, std::vector<Point>::iterator last);

////////////////////////////////////////////////////////////////////////////////
float closestPair ( std::vector< Point > const& points )
{
	int size = points.size();

	if (size==0) throw "zero size subset";
	if (size==1) return std::numeric_limits<float>::max();

	/*std::sort(points.begin(), points.end(), [](const auto& lhs, const auto& rhs)
	{
		return lhs.x < rhs.x;
	});*/

	std::vector<Point> copy;
	for(int i=0; i<size;++i)
	{
		copy.push_back(points.at(i));
	}
	//TODO sort points
	std::sort(copy.begin(), copy.end(), compareX);

	return std::sqrt( closestPair_aux(copy.begin(), copy.end()) );
}

float bruteForce(std::vector<Point>::iterator first, std::vector<Point>::iterator last)
{
	std::pair<Point, Point> closest{ *first, *std::next(first) };

	for (auto out = first; out != last; ++out)
	{
		std::pair<Point, Point> temp;
		temp.first = *out;
		for (auto in = std::next(out); in != last; ++in)
		{
			temp.second = *in;
			closest = std::min(closest, temp, [](const std::pair<Point, Point>& lhs, const std::pair<Point, Point>& rhs)
			{
				return GetDistanceSQ(lhs.first, lhs.second) < GetDistanceSQ(rhs.first, rhs.second);
			});
		}
	}
	return GetDistanceSQ(closest.first, closest.second);
}

////////////////////////////////////////////////////////////////////////////////
float closestPair_aux(std::vector<Point>::iterator first, std::vector<Point>::iterator last)
{
	int size = std::distance(first, last);
	//Check distance only when there are <=3 points
	if (size <= 3)
	{
		return bruteForce(first, last);
	}
	
	auto mid = std::next(first, std::distance(first, last) / 2);
	float left = closestPair_aux(first, mid);
	float right = closestPair_aux(mid, last);

	//Points must be at least this short to be checked
	float shortRequirement = std::min(left, right);

	std::vector<Point>possibleList;
	for(int i=0; i<size; ++i)
	{
		if(ShouldBeChecked((first+i)->x, mid->x, shortRequirement))
		{
			possibleList.push_back(*(first + i));
		}
	}
	return std::min(shortRequirement, GetShortestInBox(possibleList, shortRequirement));
}