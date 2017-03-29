/** @file
 *  @brief Exception hierarchy of dssolve.
 */

#pragma once

#include <stdexcept>
/**
 * dssolve namespace
 */
namespace dss
{

/**
 * Base class of our exception hierarchy
 */
class exception : public std::runtime_error
{
  public:
	using std::runtime_error::runtime_error;
};

/**
* Recoverable conditions
*/
class recoverable_error : public dss::exception
{
  public:
	using dss::exception::exception;
};

/**
 * Everything connected to packing and unpacking
 */
class serialization_error : public dss::recoverable_error
{
  public:
	using dss::recoverable_error::recoverable_error;
};

/**
 * Fatal conditions
 */
class fatal_error : public dss::exception
{
  public:
	using dss::exception::exception;
};
}
