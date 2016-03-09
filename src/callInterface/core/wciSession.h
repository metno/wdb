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

#ifndef WCISESSION_H_
#define WCISESSION_H_

#include <boost/noncopyable.hpp>
#include <string>

/**
 * @addtogroup wci 
 * @{
 */

/**
 * User specific data for a wci session. This is the data stored after a call 
 * to wci.begin(...)
 * 
 * @note This class has a private constructor and destructor. You may get 
 * access to an instance of it by calling WciSession::get().  
 */
class WciSession : boost::noncopyable {
 public:

  /**
   * Get the WciSession instance associated to the current process/database connection.
   */
  static WciSession & get() {
    return session_;
  }

  /**
   * Initialize wci session, with various codespaces.
   *
   * Role change is not performed by this function, and the given username
   * is not used for anything, but in order to ensure forward compatibility,
   * it should be the same as the database role to use wci.
   */
  void initialize(const std::string & userName, int dataProviderCs, int valueDomainCs, int levelDomainCs);

  /**
   * Clear session data, terminating the current wci session.
   */
  void reset();

  /// Do we have an initialized session?
  bool isInitialized() const {
    return not user().empty();
  }

  /// Get the user name provided with the initialize call.
  const std::string & user() const {
    return user_;
  }

  /// Get wanted data provider codespace
  int dataProviderNs() const {
    return dataProviderNs_;
  }

  /// Get wanted parameter codespace
  int placeNs() const {
    return placeNs_;
  }

  /// Get wanted level parameter codespace
  int parameterNs() const {
    return parameterNs_;
  }

 private:

  WciSession();
  ~WciSession();

  static WciSession session_;

  std::string user_;
  int dataProviderNs_;
  int placeNs_;
  int parameterNs_;
};

/// @}

#endif /*WCISESSION_H_*/
