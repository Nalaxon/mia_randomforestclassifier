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
#if __cplusplus < 201402L

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

