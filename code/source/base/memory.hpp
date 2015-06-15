/* 
 * File:   memory.hpp
 * Author: Christian Ertler <christian@ertler.org>
 *
 * Created on 13. Juni 2015, 19:06
 */

#ifndef MEMORY_HPP
#define	MEMORY_HPP

#include <memory>

// c++11 does not have std::make_unique but it's possible:
// Visual Studio 2012 and 2013 do not have a properly set __cplusplus flag
#if __cplusplus < 201402L && _MSC_VER != 1800 && _MSC_VER != 1700

namespace std
{

  template<typename T, typename ...Args>
  std::unique_ptr<T> make_unique(Args&& ...args)
  {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)... ));
  }
}

#endif

#endif	/* MEMORY_HPP */

