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



import java.sql.*;
import java.io.*;


/**
 * This file gives an example on querying data from an wdb database. The
 * example query returns a field ie a binary blob.
 *
 * The example illustrates how we connect to the database, initialize the wdb, 
 * query the database and reads the blob.
 *
 * wdb functions used in the example is wci.begin and wci.read.
 *
 * wci.begin 
 * ----------
 *   wci.begin('username')
 *
 * This function initialize the connection for querying the database. It 
 * sets up an session and initialize some defined default values for the user.
 *
 * The username my or may not be the same user we connected to the 
 * database as.
 *
 * wci.read
 * ---------
 * To select data from wdb the function wdi.read is used. This is
 * an overloaded function. The functions differ on the return type.
 * One funtion return data for a given geographical point and one
 * function return a reference to a field. The field is an binary
 * blob.
 *
 * The function signature to get a field.       
 * 
 * wci.read( dataprovider  text[],         - 'HIRLAM10', etc
 *           location      text,           - 'nearest POINT( 10.0 59.0 )'  
 *           referencetime wci.timeSpec,   - ('2008-03-06 12:00:00', '2008-03-06 12:00:00', 'exact')
 *           validtime     wci.timeSpec,   - ('2008-03-06 12:00:00', '2008-03-06 18:00:00', 'inside')
 *           parameter     text[],         - array['instant temperature of air' ]
 *           level         wci.levelSpec,  - ( 2, 2, 'above ground', 'exact' )
 *           dataversion   integer[],      - array[-1] 
 *           returntype    wci.returnFloat - NULL::wci.returnOid
 *        )
 *
 * wci.timespec  - (fromtime, totime, indeterminate type)
 * wci.levelspec - (fromlevel, tolevel, levelparameter,  indeterminate type)
 *
 * The 'indeterminate type' decribes the "fuzziness" in selecting from 
 * the range parameters from* and to*. Valid types are 'any', 'exact' and 'inside'. 
 *
 * 'any' match anything, regardless of the values of from* and to*.
 * 'exact' match exactly from* to to* with the same values in the data set.
 * 'inside' match all in the range from* to to*, inclusive the end points.
 * 
 * COMPILE
 *   javac -classpath postgresql-8.1-412.jdbc3.jar wdbGetDataField.java
 *  
 * RUN
 *   java -classpath 'postgresql-8.1-412.jdbc3.jar:.' wdbGetDataField
 */

public class wdbGetDataField {
    
    public static void main(String[] args)
    {
    	final String dbdriver="org.postgresql.Driver";
    	final String dbconnect="jdbc:postgresql://localhost:5432/wdb";
    	final String dbuser="wcitest";
    	final String dbpasswd="";
    	
    	Connection con = null;
        Statement statement = null;
        
        try {
    		Class.forName(dbdriver);
    		con = DriverManager.getConnection(dbconnect, dbuser, dbpasswd);
    		statement = con.createStatement();
    	} catch(Exception e) {
    		System.out.println("FATAL: cant load the database driver <"+
    						   dbdriver+">!");
    		System.exit(1);
    	}
    	
    	String wdbInit="SELECT wci.begin('"+dbuser+"', 999, 999, 999 )";
    	String sql=" SELECT * " + 
        " FROM wci.read( array['test wci 5'], NULL, " +
        "                '2009-11-13 00:00:00+00', " + 
        "                NULL, " + 
        "                array['air temperature', " +
        "                      'air pressure'], " +  
        "                NULL, " + 
        "                array[-1], " +
        "                NULL::wci.returngid )";
    	
    	final String colNames[] = {"valueParameterName", "validTimeFrom"};
    	
    	ResultSet rs=null;
    	
    	try {
    		statement.execute( wdbInit );
    		
    		rs = statement.executeQuery(sql);
    		ResultSetMetaData rsMetaData=rs.getMetaData();
    		String data;
    		boolean first;
    		
    		while( rs.next() ){
    			Statement getGridStatement = con.createStatement();
    			
    			first = true;
    		    			
    			long gridId = rs.getLong( "value" );
    			ResultSet blobResult = getGridStatement.executeQuery("SELECT * FROM wci.fetch(" + gridId + ", NULL::wci.grid)");
    			
    			// We only expect a single value here
    			while ( blobResult.next() ) {
	    			int size = blobResult.getInt("numberX") + blobResult.getInt("numberX");
	    			
	    		    // Do something with the data read here
	    			InputStream blob = blobResult.getBinaryStream("grid");
	    			DataInputStream blobReader = new DataInputStream(blob);
	    			
	    			int floatToRead = 42; // read the 42nd byte
	    			blobReader.skip(floatToRead * 4);
	    			float value = blobReader.readFloat();
	    			
	    			System.out.print(value + "\t");
	    		
	       			for( String name : colNames ) {
	    				data = rs.getString( name );
	    				
	    				if( first ){
	    					first=false;
	    				}else{
	    					System.out.print(", ");
	    				}
	    				
	    				if( data!=null )
	    					System.out.print( data );
	    				else
	    					System.out.print("\\N");
	    				
	    			}
	    			
	    			System.out.print("  (BLOB size: " + size + ")");
	    			System.out.println();
	    			
    			}
    			blobResult.close();
    		}
    	}
    	catch(Exception ex)
    	{
    		System.out.println( "SELECT error: " + ex );
    	}
        finally{
        	try {
        		if( rs != null ) 
        			rs.close();
        	} catch( SQLException ex ) {
        		System.out.println("EXCEPTION: rs.close: " + ex);
        	}
        }	
    }
    
    public wdbGetDataField()
    {
    }
}