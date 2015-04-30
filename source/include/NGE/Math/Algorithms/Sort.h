/* 
 * File:   Sort.hpp
 * Author: tku
 *
 * Created on 4 marzec 2014, 14:16
 */

#ifndef SORT_HPP
#define	SORT_HPP

namespace NGE
{
    namespace Math
    {
        namespace Algorithms
        {
            class Sort
            {
              public:
                template <typename T> inline static void BubbleSort(T* array, size_t size, int(*compare)(const void* a, const void* b) = DefaultCompare<T>)
                {
                    size_t n = size;
                    
                    while (n > 0)
                    {
                        int newn = 0;
                        for (size_t i = 1; i < n; ++i)
                        {
                            if (compare(&array[i-1], &array[i]) == 1)
                            {
                                std::swap(array[i-1], array[i]);
                                newn = i;
                            }
                        }
                        n = newn;
                    }
                }
                
                template <typename T> inline static void SelectionSort(T* array, size_t size, int(*compare)(const void* a, const void* b) = DefaultCompare<T>)
                {
                    size_t min;
                    
                    for (size_t j = 0; j < size - 1; ++j)
                    {
                        min = j;
                        for (size_t i = j + 1; i < size; ++i)
                        {
                            if (compare(&array[min], &array[i]) == 1)
                                min = i;
                        }
                        if (min != j)
                            std::swap(array[j], array[min]);
                    }
                }
                
                template <typename T> inline static void InsertionSort(T* array, size_t size, int(*compare)(const void* a, const void* b) = DefaultCompare<T>)
                {
                    size_t j = 0;
                    for (size_t i = 1; i < size; ++i)
                    {
                        j = i;
                        while ((j > 0) && (array[j-1] > array[j]))
                        {
                            std::swap(array[j], array[j-1]);
                            j--;
                        }           
                    }
                }
                
              private:
                template <typename T> inline static int DefaultCompare(const void* a, const void* b)
                {
                    T arg1 = *reinterpret_cast<const T*>(a);
                    T arg2 = *reinterpret_cast<const T*>(b);
                    if (arg1 < arg2) return -1;
                    if (arg1 > arg2) return 1;
                    return 0;
                }
            };
        }
    }
}

#endif	/* SORT_HPP */

