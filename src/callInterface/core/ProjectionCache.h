#ifndef PROJECTIONCACHE_H_
#define PROJECTIONCACHE_H_

#include <PlaceSpecification.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct PlacePoint
{
	int i;
	int j;
	const char * geometry;
};

//void * projectionCacheSetup(const struct PlaceSpecification * ps);
void * projectionCacheSetup(const struct PlaceSpecification * ps, void * alloc(unsigned));
bool projectionCacheHasNext(void * projectionCache);
struct PlacePoint projectionCacheNext(void * projectionCache);
void projectionCacheDelete(void * projectionCache, void (memory_deleter)(void*));

#ifdef __cplusplus
}

#include <PlaceSpecification.h>
#include <boost/noncopyable.hpp>
#include <vector>
#include <string>


/**
 * Cached projections
 *
 * @warning This class is brittle, in the sense that if it contains any
 * dynamically allocated memory, it will give postgresql a memory leak. The
 * leak occurs if ProjectionCache is created by a query, and the query is
 * cancelled before all data has been read. See code in placePointInternal.c
 */
class ProjectionCache : boost::noncopyable
{
public:
	ProjectionCache(const PlaceSpecification & ps);
	~ProjectionCache();

	typedef size_t size_type;

	size_type size() const;

	bool hasNext() const;
	const PlacePoint next();

private:
	void fillCache();
	PlacePoint createPlacePoint(size_type idx);

	static const size_type cacheSize_ = 65536;

	const PlaceSpecification grid_;
	size_type idx_;

//	typedef std::vector<double> Cache;
//	Cache lon_;
//	Cache lat_;

	double lon_[cacheSize_];
	double lat_[cacheSize_];

//	Cache::size_type pos_;
	size_type pos_;
	std::string lastGeometry_;
};

#endif // __cplusplus

#endif /*PROJECTIONCACHE_H_*/
