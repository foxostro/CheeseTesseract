#ifndef VARYING_OVER_TIME_H
#define VARYING_OVER_TIME_H

#include "PropertyBag.h"

/**
Represents the curve of some value over time.
The particle engine makes extensive use of the ParticleGraph class to model
arbitrary curves in the graph over time of the properties of particles and
particle systems e.g. colors, size.
*/
template<typename TYPE>
class VaryingOverTime
{
private:
	struct DataPoint
	{
		float time;
		TYPE value;
	};

	/** data points */
	vector<DataPoint> points;

public:
	/** Default Constructor */
	VaryingOverTime()
	{
		points.clear();
	}

	/**
	Constructor
	@param data data on the curve
	*/
	VaryingOverTime(const PropertyBag &data)
	{
		load(data);
	}

	/** Reset the curve */
	void reset()
	{
		points.clear();
	}

	/**
	Loads graph data from xml
	@param data data on the curve
	*/
	void load(const PropertyBag &data)
	{
		reset();

		const size_t numberOfPoints = data.getNumInstances("point");
		for(size_t i=0; i<numberOfPoints; ++i)
		{
			PropertyBag PointBag;
			DataPoint pt;

			data.get("point", PointBag, i);
			PointBag.get("x", pt.time);
			PointBag.get("y", pt.value);

			points.push_back(pt);
		}
	}

	/**
	Calculates the value at the specified time
	@param t [0.0, 1.0] representing progress through time on the curve
	*/
	TYPE getValue(float t) const
	{
		if(t<0.0f) t=0.0f;
		if(t>1.0f) t=1.0f;

		const size_t numberOfPoints = points.size();

		ASSERT(numberOfPoints>0, "No data points!  Need at least 1.");

		if(numberOfPoints == 1)
		{
			return (points[0].value);
		}

		for(size_t i=0; i<numberOfPoints-1; ++i)
		{
			const DataPoint &one = points[i+0];
			const DataPoint &two = points[i+1];

			if(t >= one.time && t <= two.time)
			{
				TYPE slope = (two.value-one.value)*(1.0f/(two.time-one.time));
				TYPE value = slope * (t - one.time) + one.value;

				return value;
			}
		}

		FAIL("Shouldn't reach here!");
		return TYPE();
	}
};

#endif
