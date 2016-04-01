/* 
 * File:   Singleton.hpp
 * Author: tku
 *
 * Created on 1 April 2016, 02:09
 */

#ifndef SINGLETON_HPP
#define SINGLETON_HPP

namespace NGE {
    namespace Core {

        template <typename T> class Singleton {
          public:

            static T& GetInstance() {
                static T instance;
                return instance;
            }
        };
    }
}

#endif /* SINGLETON_HPP */

