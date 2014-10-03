/*
 wdb

 Copyright (C) 2014 met.no

 Contact information:
 Norwegian Meteorological Institute
 Box 43 Blindern
 0313 OSLO
 NORWAY
 E-mail: post@met.no

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 MA  02110-1301, USA
 */

#include "LRUFileListTest.h"
#include "../LRUList.h"

CPPUNIT_TEST_SUITE_REGISTRATION( lo::LRUFileListTest );


namespace lo
{

LRUFileListTest::LRUFileListTest()
{
}

LRUFileListTest::~LRUFileListTest()
{
}
namespace
{
boost::shared_ptr<std::string> s(const char * c)
{
	return boost::shared_ptr<std::string>(new std::string(c));
}

template<typename P>
bool strptr_eq(const std::string & s, P p)
{
	if ( ! p )
		return false;
	return s == * p;
}

}
void LRUFileListTest::correctRead()
{
	LRUList<int, std::string> lru(2);

	lru.set(1, s("a"));
	lru.set(2, s("b"));
	lru.set(3, s("c"));

	CPPUNIT_ASSERT_EQUAL(2u, lru.size());
	CPPUNIT_ASSERT( ! lru.get(1));
	CPPUNIT_ASSERT(strptr_eq("b", lru.get(2)));
	CPPUNIT_ASSERT(strptr_eq("c", lru.get(3)));
}

void LRUFileListTest::correctEviction()
{
	LRUList<int, std::string> lru(3);

	lru.set(1, s("a"));
	lru.set(2, s("b"));
	lru.set(3, s("c"));
	lru.get(1); // reading value 1, so it is no longer least recently used
	lru.set(4, s("d"));

	CPPUNIT_ASSERT( ! lru.get(2));
	CPPUNIT_ASSERT(strptr_eq("c", lru.get(3)));
	CPPUNIT_ASSERT(strptr_eq("d", lru.get(4)));

}

void LRUFileListTest::overwriteProtection()
{
	LRUList<int, std::string> lru(2);
	lru.set(1, s("a"));
	CPPUNIT_ASSERT_THROW(lru.set(1, s("b")), std::runtime_error);
}

} /* namespace lo */
