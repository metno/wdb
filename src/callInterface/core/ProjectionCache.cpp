#include "ProjectionCache.h"
#include "WdbProjection.h"
#include <wdbMath.h>
#include <sstream>
#include <string>

#include <iostream>

using namespace std;

extern "C"
{

//void * projectionCacheSetup(const struct PlaceSpecification * ps)
//{
//	void * buffer = malloc/*memory_allocator*/(sizeof(ProjectionCache));
//
//	ProjectionCache * pc = new (buffer) ProjectionCache(* ps);
//	return (void*) pc;
//}

//void * projectionCacheSetup(const struct PlaceSpecification * ps, void *(alloc)(size_t))
void * projectionCacheSetup(const struct PlaceSpecification * ps, void * alloc(unsigned) )
{
	void * buffer = alloc(sizeof(ProjectionCache));

	ProjectionCache * pc = new (buffer) ProjectionCache(* ps);
	return (void*) pc;
}


unsigned projectionCacheSize(void * projectionCache)
{
	const ProjectionCache * pc = (ProjectionCache *) projectionCache;
	return pc->size();
}

bool projectionCacheHasNext(void * projectionCache)
{
	const ProjectionCache * pc = (ProjectionCache *) projectionCache;
	return pc->hasNext();
}

struct PlacePoint projectionCacheNext(void * projectionCache)
{
	ProjectionCache * pc = (ProjectionCache *) projectionCache;
	return pc->next();
}

void projectionCacheDelete(void * projectionCache, void (memory_deleter)(void*))
{
	ProjectionCache * pc = (ProjectionCache *) projectionCache;
	pc->~ProjectionCache();
	memory_deleter(projectionCache);
}

}

const ProjectionCache::size_type ProjectionCache::cacheSize_;// = 65536;

ProjectionCache::ProjectionCache(const PlaceSpecification & grid)
	: grid_(grid), idx_(0)
{
}

ProjectionCache::~ProjectionCache()
{
}

ProjectionCache::size_type ProjectionCache::size() const
{
	return grid_.xNumber_ * grid_.yNumber_;
}

bool ProjectionCache::hasNext() const
{
	return idx_ < size();
}

const PlacePoint ProjectionCache::next()
{
	if ( ! (idx_ % cacheSize_) )
	{
		fillCache();
		pos_ = 0;
	}
	else
		++ pos_;

	double lon = lon_[pos_];
	double lat = lat_[pos_];

	PlacePoint ret = createPlacePoint(idx_);
	++ idx_;

	return ret;
}

void ProjectionCache::fillCache()
{
	size_t neededSpace = std::min(cacheSize_, size() - idx_);

	cout << __func__ << "(" << neededSpace << ") - " << idx_/cacheSize_ << "/" << size()/cacheSize_  << endl;

	const int offset = idx_;
//	lon_.resize(neededSpace);
//	lat_.resize(neededSpace);
	for ( int pos = 0; pos < neededSpace; ++ pos )
	{
		lon_[pos] = (grid_.startingLongitude_ + (xFromIndex(offset + pos, grid_) * grid_.xIncrement_)) * DEG_TO_RAD;
		lat_[pos] = (grid_.startingLatitude_  + (yFromIndex(offset + pos, grid_) * grid_.yIncrement_)) * DEG_TO_RAD;
	}
	WdbProjection projection(grid_.projDefinition_);
	WdbProjection dest("+proj=longlat +ellps=WGS84 +no_defs");
	projection.transform(dest, neededSpace, & lon_[0], & lat_[0]);

	for ( int pos = 0; pos < neededSpace; ++ pos )
	{
		lon_[pos] *= RAD_TO_DEG;
		lat_[pos] *= RAD_TO_DEG;
	}
}

PlacePoint ProjectionCache::createPlacePoint(ProjectionCache::size_type idx)
{
	PlacePoint ret;
	ret.i = xFromIndex(idx, grid_);
	ret.j = yFromIndex(idx, grid_);

	idx %= cacheSize_;

	ostringstream geo;
	geo << "POINT(" << wdb::round(lon_[idx],4) << " " << wdb::round(lat_[idx],4) << ")";
	lastGeometry_ = geo.str();

	ret.geometry = lastGeometry_.c_str();

	return ret;
}
