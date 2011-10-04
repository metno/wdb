/*
    wdb - weather and water data storage

    Copyright (C) 2007 met.no

    Contact information:
    Norwegian Meteorological Institute
    Box 43 Blindern
    0313 OSLO
    NORWAY
    E-mail: wdb@met.no

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


#ifndef PLACEGEOMETRYTEST_H_
#define PLACEGEOMETRYTEST_H_

#include <AbstractWciTestFixture.h>
#include <map>
#include <string>

/**
 * @addtogroup Tests
 * @{
 * @addtogroup wci_test
 * @{
 */


/**
 * Signifies a test which should fail because we don't support multiple
 * geometries.
 */
#define CPPUNIT_TEST_FAIL_MULTIGEOMETRY(x) CPPUNIT_TEST_EXCEPTION(x,pqxx::sql_error)

/**
 * Test fixture for placeGeos parameter in wciRead.
 *
 * @see WDB Call Interface System Test Specification contains details
 * about the tests.
 */
class PlaceGeometryTest : public AbstractWciTestFixture
{
	CPPUNIT_TEST_SUITE( PlaceGeometryTest );

	CPPUNIT_TEST( testG1_01A_ValidPoint );
	CPPUNIT_TEST( testG1_01B_ValidPoint );
	CPPUNIT_TEST( testG1_02A_InvalidPoint );
	CPPUNIT_TEST( testG1_02B_InvalidPoint );
	CPPUNIT_TEST( testG1_03A_ValidPolygon );
	CPPUNIT_TEST( testG1_03B_ValidPolygon );
	CPPUNIT_TEST( testG1_04A_InvalidPolygon );
	CPPUNIT_TEST( testG1_04B_InvalidPolygon );

	CPPUNIT_TEST( testG2_01A_NoGeometry );
	CPPUNIT_TEST( testG2_01B_NoGeometry );
	CPPUNIT_TEST( testG2_02A_OneGeometry );
	CPPUNIT_TEST( testG2_02B_OneGeometry );
	CPPUNIT_TEST_FAIL_MULTIGEOMETRY( testG2_03_MultipleGeometry );
	CPPUNIT_TEST_FAIL_MULTIGEOMETRY( testG2_04_MoreThan255Geometries );
	CPPUNIT_TEST( testG2_05A_NullGeometry );
	CPPUNIT_TEST( testG2_05B_NullGeometry );

	// No multiple geometries used yet
	CPPUNIT_TEST_FAIL_MULTIGEOMETRY( testG3_01_NoDuplicates );
	CPPUNIT_TEST_FAIL_MULTIGEOMETRY( testG3_02_OneDuplicate );
	CPPUNIT_TEST_FAIL_MULTIGEOMETRY( testG3_03_SeveralDuplicates );

	// G4 not implemented

	CPPUNIT_TEST( testG5_01_NorthernHemisphere );
	CPPUNIT_TEST( testG5_02_SouthernHemisphere );
	CPPUNIT_TEST( testG5_03_Equator );
	CPPUNIT_TEST( testG5_04_NorthPole );
	CPPUNIT_TEST( testG5_05_SouthPole );
	CPPUNIT_TEST_FAIL_MULTIGEOMETRY( testG5_06_SetOfPointsInNorthernHemisphere );
	CPPUNIT_TEST_FAIL_MULTIGEOMETRY( testG5_07_SetOfPointsInSouthernHemisphere );
	CPPUNIT_TEST_FAIL_MULTIGEOMETRY( testG5_08_SetOfPointsAtEquator );

	CPPUNIT_TEST( testG6_01_EasternHemisphere );
	CPPUNIT_TEST( testG6_02_WesternHemisphere );
	CPPUNIT_TEST( testG6_03_0Meridian );
	CPPUNIT_TEST( testG6_04_180EMeridian );
	CPPUNIT_TEST( testG6_05_180WMeridian );
	CPPUNIT_TEST_FAIL_MULTIGEOMETRY( testG6_06_SetOfPointsInEasternHemisphere );
	CPPUNIT_TEST_FAIL_MULTIGEOMETRY( testG6_07_SetOfPointsInWesternHemisphere );
	CPPUNIT_TEST_FAIL_MULTIGEOMETRY( testG6_08_SetOfPointsAt0Meridian );
	CPPUNIT_TEST_FAIL_MULTIGEOMETRY( testG6_09_SetOfPointsAt180Meridian );
	CPPUNIT_TEST_FAIL_MULTIGEOMETRY( testG6_10_SetOfPointsAtN180Meridian );

	CPPUNIT_TEST( testG7_01_OnePoint );
	CPPUNIT_TEST_FAIL_MULTIGEOMETRY( testG7_02_SetOfPoints );
	CPPUNIT_TEST( testG7_03_OnePolygon );
	CPPUNIT_TEST_FAIL_MULTIGEOMETRY( testG7_04_SetOfPolygons );

	CPPUNIT_TEST( testG8_01_NullArea );
	CPPUNIT_TEST( testG8_02_AreaSizeLtProjPrecision );
	CPPUNIT_TEST( testG8_03_PolygonWithMeasurableArea );

	CPPUNIT_TEST( testG9_01_4PointsInPolygon );
	CPPUNIT_TEST( testG9_02_SeveralPointsInPolygon );
	//CPPUNIT_TEST( testG9_03_MoreThan1000PointsInPolygon );
	CPPUNIT_TEST( testG9_02_LongPolygonSpecification );

	//CPPUNIT_TEST( testG10_01_ConvexPolygon );
	//CPPUNIT_TEST( testG10_02_ConcavePolygon );

	CPPUNIT_TEST( testG11_01_NoCrossingPoints );
	CPPUNIT_TEST( testG11_02_1CrossingPoint );
	CPPUNIT_TEST( testG11_03_MoreThan1CrossingPoint );

	CPPUNIT_TEST( testG12_01_PolygonWith1Hole );
	CPPUNIT_TEST( testG12_02_PolygonWith2Holes );
	CPPUNIT_TEST( testG12_03_PolygonWithSeveralHoles );
	/*
	CPPUNIT_TEST( testG12_01_AnglePerpendicular );
	CPPUNIT_TEST( testG12_02_AcuteAngle );
	CPPUNIT_TEST( testG12_03_ObtuseAngle );
	CPPUNIT_TEST( testG12_04_StraightAngle );
	CPPUNIT_TEST( testG12_05_ReflexAngle );

	CPPUNIT_TEST( testG13_01_FewLongSegments );
	CPPUNIT_TEST( testG13_02_ManyShortSegments );
	CPPUNIT_TEST( testG13_03_MixedSegments );

	CPPUNIT_TEST( testG14_01_AllRequestedPointsOnGrid );
	CPPUNIT_TEST( testG14_02_SomeRequestedPointsOnGrid );
	CPPUNIT_TEST( testG14_03_NoPointsOnGrid );

	CPPUNIT_TEST( testG15_01_CompleteOverlap );
	CPPUNIT_TEST( testG15_02_PartialOverlap );
	CPPUNIT_TEST( testG15_03_NoOverlap );

	CPPUNIT_TEST( testG16_01_CompleteIntersection );
	CPPUNIT_TEST( testG16_02_PartialIntersection );
	CPPUNIT_TEST( testG16_03_NoIntersection );
	*/

	CPPUNIT_TEST( testG17_01_CorrectPointSyntax );
	CPPUNIT_TEST( testG17_02_IncorrectPointSyntax );
	CPPUNIT_TEST( testG17_03_CorrectPolygonSyntax );
	CPPUNIT_TEST( testG17_04_IncorrectPolygonSyntax );

	/*
	CPPUNIT_TEST( testG18_01_SpecOnGlobalField );
	CPPUNIT_TEST( testG18_02_SpecOnNonGlobalField );
	 */

	CPPUNIT_TEST( testG19_01_InvalidInterpolationType );
	CPPUNIT_TEST( testG19_02_NoInterpolationType );

	CPPUNIT_TEST( testG20_01_ExactInsideGrid );
	CPPUNIT_TEST( testG20_02_ExactEdgeOfGrid );
	CPPUNIT_TEST( testG20_03_ExactJustOutsideGrid );
	CPPUNIT_TEST( testG20_04_ExactFarOutsideGrid );
	CPPUNIT_TEST( testG20_05_ExactWithUndefinedPoint );

	CPPUNIT_TEST( testG21_01_NearestInsideGrid );
	CPPUNIT_TEST( testG21_02_NearestEdgeOfGrid );
	CPPUNIT_TEST( testG21_03_NearestJustOutsideGrid );
	CPPUNIT_TEST( testG21_04_NearestFarOutsideGrid );
	CPPUNIT_TEST( testG21_05_NearestWithUndefinedPoint );

	CPPUNIT_TEST( testG22_01_SurroundInsideGrid );
	CPPUNIT_TEST( testG22_02_SurroundEdgeOfGrid );
	CPPUNIT_TEST( testG22_03_SurroundJustOutsideGrid );
	CPPUNIT_TEST( testG22_04_SurroundFarOutsideGrid );
	CPPUNIT_TEST( testG22_05_SurroundWithOneUndefinedPoint );

	CPPUNIT_TEST( testG23_01_BilinearInsideGrid );
	CPPUNIT_TEST( testG23_02_BilinearEdgeOfGrid );
	CPPUNIT_TEST( testG23_03_BilinearJustOutsideGrid );
	CPPUNIT_TEST( testG23_04_BilinearFarOutsideGrid );
	CPPUNIT_TEST( testG23_05_BilinearWithOneUndefinedPoint );

	CPPUNIT_TEST( testG24_01_BilinearReturnsCorrectGeometry );

	CPPUNIT_TEST( testG25_01_PointSource_ExactHit );
	CPPUNIT_TEST( testG25_02_PointSource_ExactMiss );
	CPPUNIT_TEST( testG25_03_PointSource_Nearest );
	CPPUNIT_TEST( testG25_04_PointSource_Bilinear );
	CPPUNIT_TEST( testG25_05_PointSource_Surround );
	CPPUNIT_TEST( testG25_06_PointSource_Polygon );

	CPPUNIT_TEST( testG30_02_AllPointsCorrectlyLocatedRotated );

	CPPUNIT_TEST( testG31_01_ReturnAllRows );
	CPPUNIT_TEST( testG31_02_ReturnSomeRows );
	CPPUNIT_TEST( testG31_03_ReturnNoRows );

	CPPUNIT_TEST( testG40_01_AddPlacePoint );
	CPPUNIT_TEST( testG40_02_AddPlaceRegularGrid );

	CPPUNIT_TEST( testG41_01_addWithTrimmedSrid );
	CPPUNIT_TEST( testG41_02_addWithRightSpacedSrid );
	CPPUNIT_TEST( testG41_03_addWithLeftSpacedSrid );
	CPPUNIT_TEST( testG41_04_addWithBothSpacedSrid );
	CPPUNIT_TEST_EXCEPTION( testG41_05_addWithArbitrarySpacedSrid, pqxx::sql_error );

	// Bug Tests
	CPPUNIT_TEST( testG101_01_lonlatoutofbounds );
	CPPUNIT_TEST( testG101_02_lonlatoutofbounds );
	CPPUNIT_TEST( testG102_01_testLocationInterference );

	CPPUNIT_TEST_SUITE_END();

public:
	PlaceGeometryTest();
	virtual ~PlaceGeometryTest();

	// Validity
	void testG1_01A_ValidPoint();
	void testG1_01B_ValidPoint();
	void testG1_02A_InvalidPoint();
	void testG1_02B_InvalidPoint();
	void testG1_03A_ValidPolygon();
	void testG1_03B_ValidPolygon();
	void testG1_04A_InvalidPolygon();
	void testG1_04B_InvalidPolygon();

	// Number of points/polygons
	void testG2_01A_NoGeometry();
	void testG2_01B_NoGeometry();
	void testG2_02A_OneGeometry();
	void testG2_02B_OneGeometry();
	void testG2_03_MultipleGeometry();
	void testG2_04_MoreThan255Geometries();
	void testG2_05A_NullGeometry();
	void testG2_05B_NullGeometry();

	// Duplicates (Multiple Geometries)
	void testG3_01_NoDuplicates();
	void testG3_02_OneDuplicate();
	void testG3_03_SeveralDuplicates();

	// Multiple Geometries
	// G4 not implemented

	// Position (N/S)
	void testG5_01_NorthernHemisphere();
	void testG5_02_SouthernHemisphere();
	void testG5_03_Equator();
	void testG5_04_NorthPole();
	void testG5_05_SouthPole();
	void testG5_06_SetOfPointsInNorthernHemisphere();
	void testG5_07_SetOfPointsInSouthernHemisphere();
	void testG5_08_SetOfPointsAtEquator();

	// Position (E/W)
	void testG6_01_EasternHemisphere();
	void testG6_02_WesternHemisphere();
	void testG6_03_0Meridian();
	void testG6_04_180EMeridian();
	void testG6_05_180WMeridian();
	void testG6_06_SetOfPointsInEasternHemisphere();
	void testG6_07_SetOfPointsInWesternHemisphere();
	void testG6_08_SetOfPointsAt0Meridian();
	void testG6_09_SetOfPointsAt180Meridian();
	void testG6_10_SetOfPointsAtN180Meridian();

	// Type
	void testG7_01_OnePoint();
	void testG7_02_SetOfPoints();
	void testG7_03_OnePolygon();
	void testG7_04_SetOfPolygons();

	// Area
	void testG8_01_NullArea();
	void testG8_02_AreaSizeLtProjPrecision();
	void testG8_03_PolygonWithMeasurableArea();

	// Points in polygon
	void testG9_01_4PointsInPolygon();
	void testG9_02_SeveralPointsInPolygon();
	void testG9_03_MoreThan1000PointsInPolygon();
	void testG9_02_LongPolygonSpecification();

	// Convexity
	void testG10_01_ConvexPolygon();
	void testG10_02_ConcavePolygon();

	// Crossing points
	void testG11_01_NoCrossingPoints();
	void testG11_02_1CrossingPoint();
	void testG11_03_MoreThan1CrossingPoint();

	// Holes
	void testG12_01_PolygonWith1Hole();
	void testG12_02_PolygonWith2Holes();
	void testG12_03_PolygonWithSeveralHoles();

	/*
	// Angles in polygon
	void testG12_01_AnglePerpendicular();
	void testG12_02_AcuteAngle();
	void testG12_03_ObtuseAngle();
	void testG12_04_StraightAngle();
	void testG12_05_ReflexAngle();

	// Offset
	void testG14_01_AllRequestedPointsOnGrid();
	void testG14_02_SomeRequestedPointsOnGrid();
	void testG14_03_NoPointsOnGrid();

	// Overlap
	void testG15_01_CompleteOverlap();
	void testG15_02_PartialOverlap();
	void testG15_03_NoOverlap();

	// Overlap with multiple geometry
	void testG16_01_CompleteIntersection();
	void testG16_02_PartialIntersection();
	void testG16_03_NoIntersection();
		*/

	// Syntax
	void testG17_01_CorrectPointSyntax();
	void testG17_02_IncorrectPointSyntax();
	void testG17_03_CorrectPolygonSyntax();
	void testG17_04_IncorrectPolygonSyntax();

	// Complete field
	void testG18_01_SpecOnGlobalField();
	void testG18_02_SpecOnNonGlobalField();

	// Interpolation type
	void testG19_01_InvalidInterpolationType();
	void testG19_02_NoInterpolationType();

	// Offset for exact
	void testG20_01_ExactInsideGrid();
	void testG20_02_ExactEdgeOfGrid();
	void testG20_03_ExactJustOutsideGrid();
	void testG20_04_ExactFarOutsideGrid();
	void testG20_05_ExactWithUndefinedPoint();


	// Offset for nearest
	void testG21_01_NearestInsideGrid();
	void testG21_02_NearestEdgeOfGrid();
	void testG21_03_NearestJustOutsideGrid();
	void testG21_04_NearestFarOutsideGrid();
	void testG21_05_NearestWithUndefinedPoint();


	// Offset for surround
	void testG22_01_SurroundInsideGrid();
	void testG22_02_SurroundEdgeOfGrid();
	void testG22_03_SurroundJustOutsideGrid();
	void testG22_04_SurroundFarOutsideGrid();

	void testG22_05_SurroundWithOneUndefinedPoint();

	// Bilinear interpolations
	void testG23_01_BilinearInsideGrid();
	void testG23_02_BilinearEdgeOfGrid();
	void testG23_03_BilinearJustOutsideGrid();
	void testG23_04_BilinearFarOutsideGrid();

	void testG23_05_BilinearWithOneUndefinedPoint();

	// Bilinear interpolation (correct return)
	void testG24_01_BilinearReturnsCorrectGeometry();

	// Points
	void testG25_01_PointSource_ExactHit();
	void testG25_02_PointSource_ExactMiss();
	void testG25_03_PointSource_Nearest();
	void testG25_04_PointSource_Bilinear();
	void testG25_05_PointSource_Surround();
	void testG25_06_PointSource_Polygon();

	// Correctness
	void testG30_02_AllPointsCorrectlyLocatedRotated();

	/**
	 * Give coordinates exactly equal to the hirlam 10 grid, and expect to get
	 * back all points.
	 */
	void testG31_01_ReturnAllRows();
	void testG31_02_ReturnSomeRows();
	void testG31_03_ReturnNoRows();

	/**
	 * Add Place Points + Place Regular Grids
	 */
	void testG40_01_AddPlacePoint();
	void testG40_02_AddPlaceRegularGrid();

	/**
	 * Test the adding of grids for SRID inconsistencies
	 */
	void testG41_01_addWithTrimmedSrid();
	void testG41_02_addWithRightSpacedSrid();
	void testG41_03_addWithLeftSpacedSrid();
	void testG41_04_addWithBothSpacedSrid();
	void testG41_05_addWithArbitrarySpacedSrid();

	void testG101_01_lonlatoutofbounds();
	void testG101_02_lonlatoutofbounds();

	void testG102_01_testLocationInterference();

private:
	/**
	 * We use paramid to separate different sets of test data. ParamId is
	 * essentially the GRIB/internal parameter id; setting this incorrectly
	 * will result in undefined behaviour. Check the parameter mapping before
	 * using.
	 *
	 * @warning behavior is undefined if paramid does not exist in specFromParamNumber_.
	 */
	std::string statement_( const std::string & geo, int paramid = 1 ) const;
	/**
	 * We use paramid to separate different sets of test data. ParamId is
	 * essentially the GRIB/internal parameter id; setting this incorrectly
	 * will result in undefined behaviour. Check the parameter mapping before
	 * using.
	 *
	 * @warning behavior is undefined if paramid does not exist in specFromParamNumber_.
	 */
	std::string statementFloat_( const std::string & geo, int paramid = 1 ) const;

	/**
	 * The paramId to parameter name map
	 */
	static const std::map<int, std::string> specFromParamNumber_;

};

/**
 * @}
 * @}
 */

#endif /*PLACEGEOMETRYTEST_H_*/
