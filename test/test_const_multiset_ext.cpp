/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2025 John Wellbelove

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the 'Software'), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

#include "unit_test_framework.h"

#include <array>
#include <algorithm>
#include <utility>
#include <iterator>

#include "etl/const_multiset.h"
#include "etl/span.h"

#include "data.h"

namespace
{
  static const size_t Max_Size = 10UL;

  //*************************************************************************
  // The key type
  //*************************************************************************
  struct Key
  {
    // Default constructor
    constexpr Key()
      : k(0)
    {
    }

    // Construct from char key
    constexpr explicit Key(char k_)
      : k(k_)
    {
    }

    char k;
  };

  // Less-than operator for Key < Key
  constexpr bool operator <(const Key& lhs, const Key& rhs) noexcept
  {
    return (lhs.k < rhs.k);
  }

  // Less-than operator for Key < char
  constexpr bool operator <(const Key& lhs, char rhs) noexcept
  {
    return (lhs.k < rhs);
  }

  // Less-than operator for char < Key
  constexpr bool operator <(char lhs, const Key& rhs) noexcept
  {
    return (lhs < rhs.k);
  }

  // Equality operator for Key == Key
  constexpr bool operator ==(const Key& lhs, const Key& rhs) noexcept
  {
    return (lhs.k == rhs.k);
  }

  // Equality operator for Key != Key
  constexpr bool operator !=(const Key& lhs, const Key& rhs) noexcept
  {
    return !(lhs.k == rhs.k);
  }

#define TEST_GREATER_THAN
#ifdef TEST_GREATER_THAN
  using Data                       = etl::const_multiset_ext<Key, etl::greater<Key>>;
  using IData                      = etl::iconst_multiset<Key, etl::greater<Key>>;
  using DataTransparentComparator  = etl::const_multiset_ext<Key, etl::greater<>>;
  using IDataTransparentComparator = etl::iconst_multiset<Key, etl::greater<>>;
#else
  using Data                       = etl::const_multiset_ext<Key, etl::less<Key>>;
  using IData                      = etl::iconst_multiset<Key, etl::less<Key>>;
  using DataTransparentComparator  = etl::const_multiset_ext<Key, etl::less<>>;
  using IDataTransparentComparator = etl::iconst_multiset<Key, etl::less<>>;
#endif

  using value_type     = Data::value_type;
  using key_type       = Data::key_type;
  using const_iterator = Data::const_iterator;
  using span_type      = etl::span<const value_type, Max_Size>;

  SUITE(test_const_multiset_ext)
  {
    //*************************************************************************
    TEST(test_default_constructor)
    {
      static const Data data;

      static const bool   is_valid      = data.is_valid();
      static const size_t size          = data.size();
      static const bool   empty         = data.empty();
      static const bool   full          = data.full();
      static const size_t capacity      = data.capacity();
      static const size_t max_size      = data.max_size();
      static const const_iterator begin = data.begin();
      static const const_iterator end   = data.end();

      CHECK_TRUE(is_valid);
      CHECK_TRUE(size == 0UL);
      CHECK_TRUE(empty);
      CHECK_FALSE(full);
      CHECK_TRUE(capacity == 0UL);
      CHECK_TRUE(max_size == 0UL);
      CHECK_TRUE(begin == end);
    }

    //*************************************************************************
    TEST(test_constructor_min_size)
    {
      static const value_type values[]{ Key('A') };

      static const Data data(values);

      static const bool   is_valid      = data.is_valid();
      static const size_t size          = data.size();
      static const bool   empty         = data.empty();
      static const bool   full          = data.full();
      static const size_t capacity      = data.capacity();
      static const size_t max_size      = data.max_size();
      static const const_iterator begin = data.begin();
      static const const_iterator end   = data.end();

      CHECK_TRUE(is_valid);
      CHECK_TRUE(size == 1U);
      CHECK_FALSE(empty);
      CHECK_TRUE(full);
      CHECK_TRUE(capacity == 1U);
      CHECK_TRUE(max_size == 1U);
      CHECK_FALSE(begin == end);
    }

    //*************************************************************************
    TEST(test_constructor_max_size)
    {
#ifdef TEST_GREATER_THAN
      static const value_type values[]{ Key('J'), Key('G'), Key('G'), Key('G'), Key('F'),
                                        Key('E'), Key('D'), Key('C'), Key('B'), Key('A') };
#else
      static const value_type values[]{ Key('A'), Key('B'), Key('C'), Key('D'), Key('E'),
                                        Key('F'), Key('G'), Key('G'), Key('G'), Key('J') };
#endif

      static const Data data(values);

      static const bool   is_valid      = data.is_valid();
      static const size_t size          = data.size();
      static const bool   empty         = data.empty();
      static const bool   full          = data.full();
      static const size_t capacity      = data.capacity();
      static const size_t max_size      = data.max_size();
      static const const_iterator begin = data.begin();
      static const const_iterator end   = data.end();

      CHECK_TRUE(is_valid);
      CHECK_TRUE(size == Max_Size);
      CHECK_FALSE(empty);
      CHECK_TRUE(full);
      CHECK_TRUE(capacity == Max_Size);
      CHECK_TRUE(max_size == Max_Size);
      CHECK_FALSE(begin == end);
    }

#if ETL_USING_CPP17
    //*************************************************************************
    TEST(test_cpp17_deduced_constructor_from_span)
    {
      static const value_type values[]{ Key('A'), Key('B'), Key('C'), Key('D'), Key('E'),
                                        Key('F'), Key('G'), Key('H'), Key('I'), Key('J') };

      static const span_type span(values);
      static const etl::const_multiset_ext data{span};

      etl::const_multiset<Key, 10> check{ Key('A'), Key('B'), Key('C'), Key('D'), Key('E'),
                                          Key('F'), Key('G'), Key('H'), Key('I'), Key('J') };

      CHECK_TRUE(data.is_valid());
      CHECK_TRUE(data.size() == Max_Size);
      CHECK_FALSE(data.empty());
      CHECK_TRUE(data.full());
      CHECK_TRUE(data.capacity() == Max_Size);
      CHECK_TRUE(data.max_size() == Max_Size);
      CHECK_FALSE(data.begin() == data.end());
    }

    //*************************************************************************
    TEST(test_cpp17_deduced_constructor_from_array)
    {
      static const value_type values[]{ Key('A'), Key('B'), Key('C'), Key('D'), Key('E'),
                                        Key('F'), Key('G'), Key('H'), Key('I'), Key('J') };

      static const etl::const_multiset_ext data{values};

      etl::const_multiset<Key, 10> check{ Key('A'), Key('B'), Key('C'), Key('D'), Key('E'),
                                          Key('F'), Key('G'), Key('H'), Key('I'), Key('J') };

      CHECK_TRUE(data.is_valid());
      CHECK_TRUE(data.size() == Max_Size);
      CHECK_FALSE(data.empty());
      CHECK_TRUE(data.full());
      CHECK_TRUE(data.capacity() == Max_Size);
      CHECK_TRUE(data.max_size() == Max_Size);
      CHECK_FALSE(data.begin() == data.end());
    }
#endif

    //*************************************************************************
    TEST(test_begin_const)
    {
#ifdef TEST_GREATER_THAN
      static const value_type values[]{ Key('J'), Key('G'), Key('G'), Key('G'), Key('F'),
                                        Key('E'), Key('D'), Key('C'), Key('B'), Key('A') };
#else
      static const value_type values[]{ Key('A'), Key('B'), Key('C'), Key('D'), Key('E'),
                                        Key('F'), Key('G'), Key('G'), Key('G'), Key('J') };
#endif

      static const Data data(values);

      CHECK_TRUE(data.is_valid());
      static const auto value = *data.begin();

#ifdef TEST_GREATER_THAN
      CHECK_TRUE(Key('J') == value);
#else
      CHECK_TRUE(Key('A') == value);
#endif
    }

    //*************************************************************************
    TEST(test_end_const)
    {
      static const value_type values[]{ Key('A'), Key('B'), Key('C'), Key('D'), Key('E'),
                                        Key('F'), Key('G'), Key('G'), Key('G'), Key('J') };

      static const Data data(values);
      
      static const const_iterator end_itr = data.end();

      CHECK_TRUE(end_itr == (data.begin() + data.size()));
    }

     //*************************************************************************
    TEST(test_equal_range_const)
    {
#ifdef TEST_GREATER_THAN
      static const value_type values[]{ Key('J'), Key('G'), Key('G'), Key('G'), Key('F'),
                                        Key('E'), Key('D'), Key('C'), Key('B'), Key('A') };
#else
      static const value_type values[]{ Key('A'), Key('B'), Key('C'), Key('D'), Key('E'),
                                        Key('F'), Key('G'), Key('G'), Key('G'), Key('J') };
#endif

      static const Data data(values);

      static const ETL_OR_STD::pair<const_iterator, const_iterator> resultA = data.equal_range(Key('A'));
      static const ETL_OR_STD::pair<const_iterator, const_iterator> resultB = data.equal_range(Key('B'));
      static const ETL_OR_STD::pair<const_iterator, const_iterator> resultC = data.equal_range(Key('C'));
      static const ETL_OR_STD::pair<const_iterator, const_iterator> resultD = data.equal_range(Key('D'));
      static const ETL_OR_STD::pair<const_iterator, const_iterator> resultE = data.equal_range(Key('E'));
      static const ETL_OR_STD::pair<const_iterator, const_iterator> resultF = data.equal_range(Key('F'));
      static const ETL_OR_STD::pair<const_iterator, const_iterator> resultG = data.equal_range(Key('G'));
      static const ETL_OR_STD::pair<const_iterator, const_iterator> resultH = data.equal_range(Key('H'));
      static const ETL_OR_STD::pair<const_iterator, const_iterator> resultI = data.equal_range(Key('I'));
      static const ETL_OR_STD::pair<const_iterator, const_iterator> resultJ = data.equal_range(Key('J'));

#ifdef TEST_GREATER_THAN
      CHECK_EQUAL(9, (std::distance(data.begin(), resultA.first)));
      CHECK_EQUAL(8, (std::distance(data.begin(), resultB.first)));
      CHECK_EQUAL(7, (std::distance(data.begin(), resultC.first)));
      CHECK_EQUAL(6, (std::distance(data.begin(), resultD.first)));
      CHECK_EQUAL(5, (std::distance(data.begin(), resultE.first)));
      CHECK_EQUAL(4, (std::distance(data.begin(), resultF.first)));
      CHECK_EQUAL(1, (std::distance(data.begin(), resultG.first)));
      CHECK_EQUAL(1, (std::distance(data.begin(), resultH.first)));
      CHECK_EQUAL(1, (std::distance(data.begin(), resultI.first)));
      CHECK_EQUAL(0, (std::distance(data.begin(), resultJ.first)));

      CHECK_EQUAL(10, (std::distance(data.begin(), resultA.second)));
      CHECK_EQUAL(9, (std::distance(data.begin(), resultB.second)));
      CHECK_EQUAL(8, (std::distance(data.begin(), resultC.second)));
      CHECK_EQUAL(7, (std::distance(data.begin(), resultD.second)));
      CHECK_EQUAL(6, (std::distance(data.begin(), resultE.second)));
      CHECK_EQUAL(5, (std::distance(data.begin(), resultF.second)));
      CHECK_EQUAL(4, (std::distance(data.begin(), resultG.second)));
      CHECK_EQUAL(1, (std::distance(data.begin(), resultH.second)));
      CHECK_EQUAL(1, (std::distance(data.begin(), resultI.second)));
      CHECK_EQUAL(1, (std::distance(data.begin(), resultJ.second)));
#else
      CHECK_EQUAL(0, (std::distance(data.begin(), resultA.first)));
      CHECK_EQUAL(1, (std::distance(data.begin(), resultB.first)));
      CHECK_EQUAL(2, (std::distance(data.begin(), resultC.first)));
      CHECK_EQUAL(3, (std::distance(data.begin(), resultD.first)));
      CHECK_EQUAL(4, (std::distance(data.begin(), resultE.first)));
      CHECK_EQUAL(5, (std::distance(data.begin(), resultF.first)));
      CHECK_EQUAL(6, (std::distance(data.begin(), resultG.first)));
      CHECK_EQUAL(9, (std::distance(data.begin(), resultH.first)));
      CHECK_EQUAL(9, (std::distance(data.begin(), resultI.first)));
      CHECK_EQUAL(9, (std::distance(data.begin(), resultJ.first)));

      CHECK_EQUAL(1, (std::distance(data.begin(), resultA.second)));
      CHECK_EQUAL(2, (std::distance(data.begin(), resultB.second)));
      CHECK_EQUAL(3, (std::distance(data.begin(), resultC.second)));
      CHECK_EQUAL(4, (std::distance(data.begin(), resultD.second)));
      CHECK_EQUAL(5, (std::distance(data.begin(), resultE.second)));
      CHECK_EQUAL(6, (std::distance(data.begin(), resultF.second)));
      CHECK_EQUAL(9, (std::distance(data.begin(), resultG.second)));
      CHECK_EQUAL(9, (std::distance(data.begin(), resultH.second)));
      CHECK_EQUAL(9, (std::distance(data.begin(), resultI.second)));
      CHECK_EQUAL(10, (std::distance(data.begin(), resultJ.second)));
#endif
    }

    //*************************************************************************
    TEST(test_equal_range_using_transparent_comparator)
    {
#ifdef TEST_GREATER_THAN
      static const value_type values[]{ Key('J'), Key('G'), Key('G'), Key('G'), Key('F'),
                                        Key('E'), Key('D'), Key('C'), Key('B'), Key('A') };
#else
      static const value_type values[]{ Key('A'), Key('B'), Key('C'), Key('D'), Key('E'),
                                        Key('F'), Key('G'), Key('G'), Key('G'), Key('J') };
#endif

      static const DataTransparentComparator data(values);

      static const ETL_OR_STD::pair<const_iterator, const_iterator> resultA = data.equal_range('A');
      static const ETL_OR_STD::pair<const_iterator, const_iterator> resultB = data.equal_range('B');
      static const ETL_OR_STD::pair<const_iterator, const_iterator> resultC = data.equal_range('C');
      static const ETL_OR_STD::pair<const_iterator, const_iterator> resultD = data.equal_range('D');
      static const ETL_OR_STD::pair<const_iterator, const_iterator> resultE = data.equal_range('E');
      static const ETL_OR_STD::pair<const_iterator, const_iterator> resultF = data.equal_range('F');
      static const ETL_OR_STD::pair<const_iterator, const_iterator> resultG = data.equal_range('G');
      static const ETL_OR_STD::pair<const_iterator, const_iterator> resultH = data.equal_range('H');
      static const ETL_OR_STD::pair<const_iterator, const_iterator> resultI = data.equal_range('I');
      static const ETL_OR_STD::pair<const_iterator, const_iterator> resultJ = data.equal_range('J');

#ifdef TEST_GREATER_THAN
      CHECK_EQUAL(9, (std::distance(data.begin(), resultA.first)));
      CHECK_EQUAL(8, (std::distance(data.begin(), resultB.first)));
      CHECK_EQUAL(7, (std::distance(data.begin(), resultC.first)));
      CHECK_EQUAL(6, (std::distance(data.begin(), resultD.first)));
      CHECK_EQUAL(5, (std::distance(data.begin(), resultE.first)));
      CHECK_EQUAL(4, (std::distance(data.begin(), resultF.first)));
      CHECK_EQUAL(1, (std::distance(data.begin(), resultG.first)));
      CHECK_EQUAL(1, (std::distance(data.begin(), resultH.first)));
      CHECK_EQUAL(1, (std::distance(data.begin(), resultI.first)));
      CHECK_EQUAL(0, (std::distance(data.begin(), resultJ.first)));

      CHECK_EQUAL(10, (std::distance(data.begin(), resultA.second)));
      CHECK_EQUAL(9, (std::distance(data.begin(), resultB.second)));
      CHECK_EQUAL(8, (std::distance(data.begin(), resultC.second)));
      CHECK_EQUAL(7, (std::distance(data.begin(), resultD.second)));
      CHECK_EQUAL(6, (std::distance(data.begin(), resultE.second)));
      CHECK_EQUAL(5, (std::distance(data.begin(), resultF.second)));
      CHECK_EQUAL(4, (std::distance(data.begin(), resultG.second)));
      CHECK_EQUAL(1, (std::distance(data.begin(), resultH.second)));
      CHECK_EQUAL(1, (std::distance(data.begin(), resultI.second)));
      CHECK_EQUAL(1, (std::distance(data.begin(), resultJ.second)));
#else
      CHECK_EQUAL(0, (std::distance(data.begin(), resultA.first)));
      CHECK_EQUAL(1, (std::distance(data.begin(), resultB.first)));
      CHECK_EQUAL(2, (std::distance(data.begin(), resultC.first)));
      CHECK_EQUAL(3, (std::distance(data.begin(), resultD.first)));
      CHECK_EQUAL(4, (std::distance(data.begin(), resultE.first)));
      CHECK_EQUAL(5, (std::distance(data.begin(), resultF.first)));
      CHECK_EQUAL(6, (std::distance(data.begin(), resultG.first)));
      CHECK_EQUAL(9, (std::distance(data.begin(), resultH.first)));
      CHECK_EQUAL(9, (std::distance(data.begin(), resultI.first)));
      CHECK_EQUAL(9, (std::distance(data.begin(), resultJ.first)));

      CHECK_EQUAL(1, (std::distance(data.begin(), resultA.second)));
      CHECK_EQUAL(2, (std::distance(data.begin(), resultB.second)));
      CHECK_EQUAL(3, (std::distance(data.begin(), resultC.second)));
      CHECK_EQUAL(4, (std::distance(data.begin(), resultD.second)));
      CHECK_EQUAL(5, (std::distance(data.begin(), resultE.second)));
      CHECK_EQUAL(6, (std::distance(data.begin(), resultF.second)));
      CHECK_EQUAL(9, (std::distance(data.begin(), resultG.second)));
      CHECK_EQUAL(9, (std::distance(data.begin(), resultH.second)));
      CHECK_EQUAL(9, (std::distance(data.begin(), resultI.second)));
      CHECK_EQUAL(10, (std::distance(data.begin(), resultJ.second)));
#endif
    }

    //*************************************************************************
    TEST(test_lower_bound_const)
    {
#ifdef TEST_GREATER_THAN
      static const value_type values[]{ Key('J'), Key('G'), Key('G'), Key('G'), Key('F'),
                                        Key('E'), Key('D'), Key('C'), Key('B'), Key('A') };
#else
      static const value_type values[]{ Key('A'), Key('B'), Key('C'), Key('D'), Key('E'),
                                        Key('F'), Key('G'), Key('G'), Key('G'), Key('J') };
#endif

      static const Data data(values);

      static const const_iterator resultA = data.lower_bound(Key('A'));
      static const const_iterator resultB = data.lower_bound(Key('B'));
      static const const_iterator resultC = data.lower_bound(Key('C'));
      static const const_iterator resultD = data.lower_bound(Key('D'));
      static const const_iterator resultE = data.lower_bound(Key('E'));
      static const const_iterator resultF = data.lower_bound(Key('F'));
      static const const_iterator resultG = data.lower_bound(Key('G'));
      static const const_iterator resultH = data.lower_bound(Key('H'));
      static const const_iterator resultI = data.lower_bound(Key('I'));
      static const const_iterator resultJ = data.lower_bound(Key('J'));
      static const const_iterator resultK = data.lower_bound(Key('K'));

#ifdef TEST_GREATER_THAN
      CHECK_EQUAL(9, (std::distance(data.begin(), resultA)));
      CHECK_EQUAL(8, (std::distance(data.begin(), resultB)));
      CHECK_EQUAL(7, (std::distance(data.begin(), resultC)));
      CHECK_EQUAL(6, (std::distance(data.begin(), resultD)));
      CHECK_EQUAL(5, (std::distance(data.begin(), resultE)));
      CHECK_EQUAL(4, (std::distance(data.begin(), resultF)));
      CHECK_EQUAL(1, (std::distance(data.begin(), resultG)));
      CHECK_EQUAL(1, (std::distance(data.begin(), resultH)));
      CHECK_EQUAL(1, (std::distance(data.begin(), resultI)));
      CHECK_EQUAL(0, (std::distance(data.begin(), resultJ)));
      CHECK_EQUAL(0, (std::distance(data.begin(), resultK)));
#else
      CHECK_EQUAL(0, (std::distance(data.begin(), resultA)));
      CHECK_EQUAL(1, (std::distance(data.begin(), resultB)));
      CHECK_EQUAL(2, (std::distance(data.begin(), resultC)));
      CHECK_EQUAL(3, (std::distance(data.begin(), resultD)));
      CHECK_EQUAL(4, (std::distance(data.begin(), resultE)));
      CHECK_EQUAL(5, (std::distance(data.begin(), resultF)));
      CHECK_EQUAL(6, (std::distance(data.begin(), resultG)));
      CHECK_EQUAL(9, (std::distance(data.begin(), resultH)));
      CHECK_EQUAL(9, (std::distance(data.begin(), resultI)));
      CHECK_EQUAL(9, (std::distance(data.begin(), resultJ)));
      CHECK_EQUAL(10, (std::distance(data.begin(), resultK)));
#endif
    }

    //*************************************************************************
    TEST(test_lower_bound_const_using_transparent_comparator)
    {
#ifdef TEST_GREATER_THAN
      static const value_type values[]{ Key('J'), Key('G'), Key('G'), Key('G'), Key('F'),
                                        Key('E'), Key('D'), Key('C'), Key('B'), Key('A') };
#else
      static const value_type values[]{ Key('A'), Key('B'), Key('C'), Key('D'), Key('E'),
                                        Key('F'), Key('G'), Key('G'), Key('G'), Key('J') };
#endif

      static const DataTransparentComparator data(values);

      static const const_iterator resultA = data.lower_bound('A');
      static const const_iterator resultB = data.lower_bound('B');
      static const const_iterator resultC = data.lower_bound('C');
      static const const_iterator resultD = data.lower_bound('D');
      static const const_iterator resultE = data.lower_bound('E');
      static const const_iterator resultF = data.lower_bound('F');
      static const const_iterator resultG = data.lower_bound('G');
      static const const_iterator resultH = data.lower_bound('H');
      static const const_iterator resultI = data.lower_bound('I');
      static const const_iterator resultJ = data.lower_bound('J');
      static const const_iterator resultK = data.lower_bound('K');

#ifdef TEST_GREATER_THAN
      CHECK_EQUAL(9, (std::distance(data.begin(), resultA)));
      CHECK_EQUAL(8, (std::distance(data.begin(), resultB)));
      CHECK_EQUAL(7, (std::distance(data.begin(), resultC)));
      CHECK_EQUAL(6, (std::distance(data.begin(), resultD)));
      CHECK_EQUAL(5, (std::distance(data.begin(), resultE)));
      CHECK_EQUAL(4, (std::distance(data.begin(), resultF)));
      CHECK_EQUAL(1, (std::distance(data.begin(), resultG)));
      CHECK_EQUAL(1, (std::distance(data.begin(), resultH)));
      CHECK_EQUAL(1, (std::distance(data.begin(), resultI)));
      CHECK_EQUAL(0, (std::distance(data.begin(), resultJ)));
      CHECK_EQUAL(0, (std::distance(data.begin(), resultK)));
#else
      CHECK_EQUAL(0, (std::distance(data.begin(), resultA)));
      CHECK_EQUAL(1, (std::distance(data.begin(), resultB)));
      CHECK_EQUAL(2, (std::distance(data.begin(), resultC)));
      CHECK_EQUAL(3, (std::distance(data.begin(), resultD)));
      CHECK_EQUAL(4, (std::distance(data.begin(), resultE)));
      CHECK_EQUAL(5, (std::distance(data.begin(), resultF)));
      CHECK_EQUAL(6, (std::distance(data.begin(), resultG)));
      CHECK_EQUAL(9, (std::distance(data.begin(), resultH)));
      CHECK_EQUAL(9, (std::distance(data.begin(), resultI)));
      CHECK_EQUAL(9, (std::distance(data.begin(), resultJ)));
      CHECK_EQUAL(10, (std::distance(data.begin(), resultK)));
#endif
    }

    //*************************************************************************
    TEST(test_upper_bound_const)
    {
#ifdef TEST_GREATER_THAN
      static const value_type values[]{ Key('J'), Key('G'), Key('G'), Key('G'), Key('F'),
                                        Key('E'), Key('D'), Key('C'), Key('B'), Key('A') };
#else
      static const value_type values[]{ Key('A'), Key('B'), Key('C'), Key('D'), Key('E'),
                                        Key('F'), Key('G'), Key('G'), Key('G'), Key('J') };
#endif

      static const Data data(values);

      static const const_iterator resultA = data.upper_bound(Key('A'));
      static const const_iterator resultB = data.upper_bound(Key('B'));
      static const const_iterator resultC = data.upper_bound(Key('C'));
      static const const_iterator resultD = data.upper_bound(Key('D'));
      static const const_iterator resultE = data.upper_bound(Key('E'));
      static const const_iterator resultF = data.upper_bound(Key('F'));
      static const const_iterator resultG = data.upper_bound(Key('G'));
      static const const_iterator resultH = data.upper_bound(Key('H'));
      static const const_iterator resultI = data.upper_bound(Key('I'));
      static const const_iterator resultJ = data.upper_bound(Key('J'));
      static const const_iterator resultK = data.upper_bound(Key('K'));

#ifdef TEST_GREATER_THAN
      CHECK_EQUAL(10, (std::distance(data.begin(), resultA)));
      CHECK_EQUAL(9, (std::distance(data.begin(), resultB)));
      CHECK_EQUAL(8, (std::distance(data.begin(), resultC)));
      CHECK_EQUAL(7, (std::distance(data.begin(), resultD)));
      CHECK_EQUAL(6, (std::distance(data.begin(), resultE)));
      CHECK_EQUAL(5, (std::distance(data.begin(), resultF)));
      CHECK_EQUAL(4, (std::distance(data.begin(), resultG)));
      CHECK_EQUAL(1, (std::distance(data.begin(), resultH)));
      CHECK_EQUAL(1, (std::distance(data.begin(), resultI)));
      CHECK_EQUAL(1, (std::distance(data.begin(), resultJ)));
      CHECK_EQUAL(0, (std::distance(data.begin(), resultK)));
#else
      CHECK_EQUAL(1, (std::distance(data.begin(), resultA)));
      CHECK_EQUAL(2, (std::distance(data.begin(), resultB)));
      CHECK_EQUAL(3, (std::distance(data.begin(), resultC)));
      CHECK_EQUAL(4, (std::distance(data.begin(), resultD)));
      CHECK_EQUAL(5, (std::distance(data.begin(), resultE)));
      CHECK_EQUAL(6, (std::distance(data.begin(), resultF)));
      CHECK_EQUAL(9, (std::distance(data.begin(), resultG)));
      CHECK_EQUAL(9, (std::distance(data.begin(), resultH)));
      CHECK_EQUAL(9, (std::distance(data.begin(), resultI)));
      CHECK_EQUAL(10, (std::distance(data.begin(), resultJ)));
      CHECK_EQUAL(10, (std::distance(data.begin(), resultK)));
#endif
    }

    //*************************************************************************
    TEST(test_upper_bound_const_using_transparent_comparator)
    {
#ifdef TEST_GREATER_THAN
      static const value_type values[]{ Key('J'), Key('G'), Key('G'), Key('G'), Key('F'),
                                        Key('E'), Key('D'), Key('C'), Key('B'), Key('A') };
#else
      static const value_type values[]{ Key('A'), Key('B'), Key('C'), Key('D'), Key('E'),
                                        Key('F'), Key('G'), Key('G'), Key('G'), Key('J') };
#endif

      static const DataTransparentComparator data(values);

      static const const_iterator resultA = data.upper_bound('A');
      static const const_iterator resultB = data.upper_bound('B');
      static const const_iterator resultC = data.upper_bound('C');
      static const const_iterator resultD = data.upper_bound('D');
      static const const_iterator resultE = data.upper_bound('E');
      static const const_iterator resultF = data.upper_bound('F');
      static const const_iterator resultG = data.upper_bound('G');
      static const const_iterator resultH = data.upper_bound('H');
      static const const_iterator resultI = data.upper_bound('I');
      static const const_iterator resultJ = data.upper_bound('J');
      static const const_iterator resultK = data.upper_bound('K');

#ifdef TEST_GREATER_THAN
      CHECK_EQUAL(10, (std::distance(data.begin(), resultA)));
      CHECK_EQUAL(9, (std::distance(data.begin(), resultB)));
      CHECK_EQUAL(8, (std::distance(data.begin(), resultC)));
      CHECK_EQUAL(7, (std::distance(data.begin(), resultD)));
      CHECK_EQUAL(6, (std::distance(data.begin(), resultE)));
      CHECK_EQUAL(5, (std::distance(data.begin(), resultF)));
      CHECK_EQUAL(4, (std::distance(data.begin(), resultG)));
      CHECK_EQUAL(1, (std::distance(data.begin(), resultH)));
      CHECK_EQUAL(1, (std::distance(data.begin(), resultI)));
      CHECK_EQUAL(1, (std::distance(data.begin(), resultJ)));
      CHECK_EQUAL(0, (std::distance(data.begin(), resultK)));
#else
      CHECK_EQUAL(1, (std::distance(data.begin(), resultA)));
      CHECK_EQUAL(2, (std::distance(data.begin(), resultB)));
      CHECK_EQUAL(3, (std::distance(data.begin(), resultC)));
      CHECK_EQUAL(4, (std::distance(data.begin(), resultD)));
      CHECK_EQUAL(5, (std::distance(data.begin(), resultE)));
      CHECK_EQUAL(6, (std::distance(data.begin(), resultF)));
      CHECK_EQUAL(9, (std::distance(data.begin(), resultG)));
      CHECK_EQUAL(9, (std::distance(data.begin(), resultH)));
      CHECK_EQUAL(9, (std::distance(data.begin(), resultI)));
      CHECK_EQUAL(10, (std::distance(data.begin(), resultJ)));
      CHECK_EQUAL(10, (std::distance(data.begin(), resultK)));
#endif
    }

    //*************************************************************************
    TEST(test_count_const)
    {
#ifdef TEST_GREATER_THAN
      static const value_type values[]{ Key('J'), Key('G'), Key('G'), Key('G'), Key('F'),
                                        Key('E'), Key('D'), Key('C'), Key('B'), Key('A') };
#else
      static const value_type values[]{ Key('A'), Key('B'), Key('C'), Key('D'), Key('E'),
                                        Key('F'), Key('G'), Key('G'), Key('G'), Key('J') };
#endif

      static const Data data(values);

      CHECK_EQUAL(1, data.count(Key('A')));
      CHECK_EQUAL(1, data.count(Key('B')));
      CHECK_EQUAL(1, data.count(Key('C')));
      CHECK_EQUAL(1, data.count(Key('D')));
      CHECK_EQUAL(1, data.count(Key('E')));
      CHECK_EQUAL(1, data.count(Key('F')));
      CHECK_EQUAL(3, data.count(Key('G')));
      CHECK_EQUAL(0, data.count(Key('H')));
      CHECK_EQUAL(0, data.count(Key('I')));
      CHECK_EQUAL(1, data.count(Key('J')));
      CHECK_EQUAL(0, data.count(Key('K')));
    }

    //*************************************************************************
    TEST(test_count_const_using_transparent_comparator)
    {
#ifdef TEST_GREATER_THAN
      static const value_type values[]{ Key('J'), Key('G'), Key('G'), Key('G'), Key('F'),
                                                   Key('E'), Key('D'), Key('C'), Key('B'), Key('A') };
#else
      static const value_type values[]{ Key('A'), Key('B'), Key('C'), Key('D'), Key('E'),
                                                   Key('F'), Key('G'), Key('G'), Key('G'), Key('J') };
#endif

      static const DataTransparentComparator data(values);

      CHECK_EQUAL(1, data.count('A'));
      CHECK_EQUAL(1, data.count('B'));
      CHECK_EQUAL(1, data.count('C'));
      CHECK_EQUAL(1, data.count('D'));
      CHECK_EQUAL(1, data.count('E'));
      CHECK_EQUAL(1, data.count('F'));
      CHECK_EQUAL(3, data.count('G'));
      CHECK_EQUAL(0, data.count('H'));
      CHECK_EQUAL(0, data.count('I'));
      CHECK_EQUAL(1, data.count('J'));
      CHECK_EQUAL(0, data.count('K'));
    }

    //*************************************************************************
    TEST(test_const_iterator)
    {
#ifdef TEST_GREATER_THAN
      static const value_type values[]{ Key('J'), Key('G'), Key('G'), Key('G'), Key('F'),
                                        Key('E'), Key('D'), Key('C'), Key('B'), Key('A') };
#else
      static const value_type values[]{ Key('A'), Key('B'), Key('C'), Key('D'), Key('E'),
                                        Key('F'), Key('G'), Key('G'), Key('G'), Key('J') };
#endif

      static const Data data(values);

      const_iterator itr = data.begin();

#ifdef TEST_GREATER_THAN
      CHECK_TRUE((Key('J')) == *itr++);
      CHECK_TRUE((Key('G')) == *itr++);
      CHECK_TRUE((Key('G')) == *itr++);
      CHECK_TRUE((Key('G')) == *itr++);
      CHECK_TRUE((Key('F')) == *itr++);
      CHECK_TRUE((Key('E')) == *itr++);
      CHECK_TRUE((Key('D')) == *itr++);
      CHECK_TRUE((Key('C')) == *itr++);
      CHECK_TRUE((Key('B')) == *itr++);
      CHECK_TRUE((Key('A')) == *itr++);
      CHECK_TRUE(itr == data.end());
#else
      CHECK_TRUE((Key('A')) == *itr++);
      CHECK_TRUE((Key('B')) == *itr++);
      CHECK_TRUE((Key('C')) == *itr++);
      CHECK_TRUE((Key('D')) == *itr++);
      CHECK_TRUE((Key('E')) == *itr++);
      CHECK_TRUE((Key('F')) == *itr++);
      CHECK_TRUE((Key('G')) == *itr++);
      CHECK_TRUE((Key('G')) == *itr++);
      CHECK_TRUE((Key('G')) == *itr++);
      CHECK_TRUE((Key('J')) == *itr++);
      CHECK_TRUE(itr == data.end());
#endif      
    }

    //*************************************************************************
    TEST(test_find_const)
    {
#ifdef TEST_GREATER_THAN
      static const value_type values[]{ Key('J'), Key('G'), Key('G'), Key('G'), Key('F'),
                                        Key('E'), Key('D'), Key('C'), Key('B'), Key('A') };
#else
      static const value_type values[]{ Key('A'), Key('B'), Key('C'), Key('D'), Key('E'),
                                        Key('F'), Key('G'), Key('G'), Key('G'), Key('J') };
#endif

      static const Data data(values);

      static const const_iterator resultA = data.find(Key('A'));
      static const const_iterator resultB = data.find(Key('B'));
      static const const_iterator resultC = data.find(Key('C'));
      static const const_iterator resultD = data.find(Key('D'));
      static const const_iterator resultE = data.find(Key('E'));
      static const const_iterator resultF = data.find(Key('F'));
      static const const_iterator resultG = data.find(Key('G'));
      static const const_iterator resultH = data.find(Key('H'));
      static const const_iterator resultI = data.find(Key('I'));
      static const const_iterator resultJ = data.find(Key('J'));

#ifdef TEST_GREATER_THAN
      CHECK_EQUAL(9, (std::distance(data.begin(), resultA)));
      CHECK_EQUAL(8, (std::distance(data.begin(), resultB)));
      CHECK_EQUAL(7, (std::distance(data.begin(), resultC)));
      CHECK_EQUAL(6, (std::distance(data.begin(), resultD)));
      CHECK_EQUAL(5, (std::distance(data.begin(), resultE)));
      CHECK_EQUAL(4, (std::distance(data.begin(), resultF)));
      CHECK_EQUAL(1, (std::distance(data.begin(), resultG)));
      CHECK_EQUAL(10, (std::distance(data.begin(), resultH)));
      CHECK_EQUAL(10, (std::distance(data.begin(), resultI)));
      CHECK_EQUAL(0, (std::distance(data.begin(), resultJ)));
#else
      CHECK_EQUAL(0, (std::distance(data.begin(), resultA)));
      CHECK_EQUAL(1, (std::distance(data.begin(), resultB)));
      CHECK_EQUAL(2, (std::distance(data.begin(), resultC)));
      CHECK_EQUAL(3, (std::distance(data.begin(), resultD)));
      CHECK_EQUAL(4, (std::distance(data.begin(), resultE)));
      CHECK_EQUAL(5, (std::distance(data.begin(), resultF)));
      CHECK_EQUAL(6, (std::distance(data.begin(), resultG)));
      CHECK_EQUAL(10, (std::distance(data.begin(), resultH)));
      CHECK_EQUAL(10, (std::distance(data.begin(), resultI)));
      CHECK_EQUAL(9, (std::distance(data.begin(), resultJ)));
#endif
    }

    //*************************************************************************
    TEST(test_find_const_using_transparent_comparator)
    {
#ifdef TEST_GREATER_THAN
      static const value_type values[]{ Key('J'), Key('G'), Key('G'), Key('G'), Key('F'),
                                        Key('E'), Key('D'), Key('C'), Key('B'), Key('A') };
#else
      static const value_type values[]{ Key('A'), Key('B'), Key('C'), Key('D'), Key('E'),
                                        Key('F'), Key('G'), Key('G'), Key('G'), Key('J') };
#endif

      static const DataTransparentComparator data(values);

      static const const_iterator resultA = data.find('A');
      static const const_iterator resultB = data.find('B');
      static const const_iterator resultC = data.find('C');
      static const const_iterator resultD = data.find('D');
      static const const_iterator resultE = data.find('E');
      static const const_iterator resultF = data.find('F');
      static const const_iterator resultG = data.find('G');
      static const const_iterator resultH = data.find('H');
      static const const_iterator resultI = data.find('I');
      static const const_iterator resultJ = data.find('J');

#ifdef TEST_GREATER_THAN
      CHECK_EQUAL(9, (std::distance(data.begin(), resultA)));
      CHECK_EQUAL(8, (std::distance(data.begin(), resultB)));
      CHECK_EQUAL(7, (std::distance(data.begin(), resultC)));
      CHECK_EQUAL(6, (std::distance(data.begin(), resultD)));
      CHECK_EQUAL(5, (std::distance(data.begin(), resultE)));
      CHECK_EQUAL(4, (std::distance(data.begin(), resultF)));
      CHECK_EQUAL(1, (std::distance(data.begin(), resultG)));
      CHECK_EQUAL(10, (std::distance(data.begin(), resultH)));
      CHECK_EQUAL(10, (std::distance(data.begin(), resultI)));
      CHECK_EQUAL(0, (std::distance(data.begin(), resultJ)));
#else
      CHECK_EQUAL(0, (std::distance(data.begin(), resultA)));
      CHECK_EQUAL(1, (std::distance(data.begin(), resultB)));
      CHECK_EQUAL(2, (std::distance(data.begin(), resultC)));
      CHECK_EQUAL(3, (std::distance(data.begin(), resultD)));
      CHECK_EQUAL(4, (std::distance(data.begin(), resultE)));
      CHECK_EQUAL(5, (std::distance(data.begin(), resultF)));
      CHECK_EQUAL(6, (std::distance(data.begin(), resultG)));
      CHECK_EQUAL(10, (std::distance(data.begin(), resultH)));
      CHECK_EQUAL(10, (std::distance(data.begin(), resultI)));
      CHECK_EQUAL(9, (std::distance(data.begin(), resultJ)));
#endif
    }

    //*************************************************************************
    TEST(test_contains_const)
    {
#ifdef TEST_GREATER_THAN
      static const value_type values[]{ Key('J'), Key('G'), Key('G'), Key('G'), Key('F'),
                              Key('E'), Key('D'), Key('C'), Key('B'), Key('A') };
#else
      static const value_type values[]{ Key('A'), Key('B'), Key('C'), Key('D'), Key('E'),
                              Key('F'), Key('G'), Key('G'), Key('G'), Key('J') };
#endif

      static const Data data(values);

      static const bool containsA = data.contains(Key('A'));
      static const bool containsB = data.contains(Key('B'));
      static const bool containsC = data.contains(Key('C'));
      static const bool containsD = data.contains(Key('D'));
      static const bool containsE = data.contains(Key('E'));
      static const bool containsF = data.contains(Key('F'));
      static const bool containsG = data.contains(Key('G'));
      static const bool containsH = data.contains(Key('H'));
      static const bool containsI = data.contains(Key('I'));
      static const bool containsJ = data.contains(Key('J'));
      static const bool containsK = data.contains(Key('K'));

      CHECK_TRUE(containsA);
      CHECK_TRUE(containsB);
      CHECK_TRUE(containsC);
      CHECK_TRUE(containsD);
      CHECK_TRUE(containsE);
      CHECK_TRUE(containsF);
      CHECK_TRUE(containsG);
      CHECK_FALSE(containsH);
      CHECK_FALSE(containsI);
      CHECK_TRUE(containsJ);
      CHECK_FALSE(containsK);
    }

    //*************************************************************************
    TEST(test_contains_with_transparent_comparator_const)
    {
#ifdef TEST_GREATER_THAN
      static const value_type values[]{ Key('J'), Key('G'), Key('G'), Key('G'), Key('F'),
                                                       Key('E'), Key('D'), Key('C'), Key('B'), Key('A') };
#else
      static const value_type values[]{ Key('A'), Key('B'), Key('C'), Key('D'), Key('E'),
                                                       Key('F'), Key('G'), Key('G'), Key('G'), Key('J') };
#endif

      static const DataTransparentComparator data(values);

      static const bool containsA = data.contains('A');
      static const bool containsB = data.contains('B');
      static const bool containsC = data.contains('C');
      static const bool containsD = data.contains('D');
      static const bool containsE = data.contains('E');
      static const bool containsF = data.contains('F');
      static const bool containsG = data.contains('G');
      static const bool containsH = data.contains('H');
      static const bool containsI = data.contains('I');
      static const bool containsJ = data.contains('J');
      static const bool containsK = data.contains('K');

      CHECK_TRUE(containsA);
      CHECK_TRUE(containsB);
      CHECK_TRUE(containsC);
      CHECK_TRUE(containsD);
      CHECK_TRUE(containsE);
      CHECK_TRUE(containsF);
      CHECK_TRUE(containsG);
      CHECK_FALSE(containsH);
      CHECK_FALSE(containsI);
      CHECK_TRUE(containsJ);
      CHECK_FALSE(containsK);
    }

    //*************************************************************************
    TEST(test_key_comp_const)
    {
      static const Data data;
      static const Data::key_compare compare = data.key_comp();

      static const bool compareAA = compare(Key{ 'A' }, Key{ 'A' });
      static const bool compareBA = compare(Key{ 'B' }, Key{ 'A' });
      static const bool compareAB = compare(Key{ 'A' }, Key{ 'B' });

    #ifdef TEST_GREATER_THAN
      CHECK_FALSE(compareAA);
      CHECK_TRUE(compareBA);
      CHECK_FALSE(compareAB);
    #else
      CHECK_FALSE(compareAA);
      CHECK_FALSE(compareBA);
      CHECK_TRUE(compareAB);
    #endif
    }

    //*************************************************************************
    TEST(test_key_comp_const_transparent_comparator)
    {
      static const DataTransparentComparator data;
      static const DataTransparentComparator::key_compare compare = data.key_comp();

      static const bool compareAA = compare('A', 'A');
      static const bool compareBA = compare('B', 'A');
      static const bool compareAB = compare('A', 'B');

    #ifdef TEST_GREATER_THAN
      CHECK_FALSE(compareAA);
      CHECK_TRUE(compareBA);
      CHECK_FALSE(compareAB);
    #else
      CHECK_FALSE(compareAA);
      CHECK_FALSE(compareBA);
      CHECK_TRUE(compareAB);
    #endif
    }

    //*************************************************************************
    TEST(test_value_comp_const)
    {
      static const Data data;
      static const Data::value_compare compare = data.value_comp();

      static const bool compareAA1 = compare(Key('A'), Key('A'));
      static const bool compareAA2 = compare(Key('A'), Key{ 'A' });
      static const bool compareAA3 = compare(Key{ 'A' }, Key('A'));

      static const bool compareBA1 = compare(Key('B'), Key('A'));
      static const bool compareBA2 = compare(Key('B'), Key{ 'A' });
      static const bool compareBA3 = compare(Key{ 'B' }, Key('A'));

      static const bool compareAB1 = compare(Key('A'), Key('B'));
      static const bool compareAB2 = compare(Key('A'), Key{ 'B' });
      static const bool compareAB3 = compare(Key{ 'A' }, Key('B'));;

    #ifdef TEST_GREATER_THAN
      CHECK_FALSE(compareAA1);
      CHECK_FALSE(compareAA2);
      CHECK_FALSE(compareAA3);

      CHECK_TRUE(compareBA1);
      CHECK_TRUE(compareBA2);
      CHECK_TRUE(compareBA3);

      CHECK_FALSE(compareAB1);
      CHECK_FALSE(compareAB2);
      CHECK_FALSE(compareAB3);
    #else
      CHECK_FALSE(compareAA1);
      CHECK_FALSE(compareAA2);
      CHECK_FALSE(compareAA3);

      CHECK_FALSE(compareBA1);
      CHECK_FALSE(compareBA2);
      CHECK_FALSE(compareBA3);

      CHECK_TRUE(compareAB1);
      CHECK_TRUE(compareAB2);
      CHECK_TRUE(compareAB3);
    #endif
    }

    //*************************************************************************
    TEST(test_value_comp_const_transparent_comparator)
    {
      static const DataTransparentComparator data;
      static const DataTransparentComparator::value_compare compare = data.value_comp();

      static const bool compareAA1 = compare(Key('A'), Key('A'));
      static const bool compareAA2 = compare(Key('A'), 'A');
      static const bool compareAA3 = compare('A', Key('A'));

      static const bool compareBA1 = compare(Key('B'), Key('A'));
      static const bool compareBA2 = compare(Key('B'), 'A');
      static const bool compareBA3 = compare('B', Key('A'));

      static const bool compareAB1 = compare(Key('A'), Key('B'));
      static const bool compareAB2 = compare(Key('A'), 'B');
      static const bool compareAB3 = compare('A', Key('B'));;

    #ifdef TEST_GREATER_THAN
      CHECK_FALSE(compareAA1);
      CHECK_FALSE(compareAA2);
      CHECK_FALSE(compareAA3);

      CHECK_TRUE(compareBA1);
      CHECK_TRUE(compareBA2);
      CHECK_TRUE(compareBA3);

      CHECK_FALSE(compareAB1);
      CHECK_FALSE(compareAB2);
      CHECK_FALSE(compareAB3);
    #else
      CHECK_FALSE(compareAA1);
      CHECK_FALSE(compareAA2);
      CHECK_FALSE(compareAA3);

      CHECK_FALSE(compareBA1);
      CHECK_FALSE(compareBA2);
      CHECK_FALSE(compareBA3);

      CHECK_TRUE(compareAB1);
      CHECK_TRUE(compareAB2);
      CHECK_TRUE(compareAB3);
    #endif
    }

    //*************************************************************************
    TEST(test_equal)
    {
      static const value_type values1[]{ Key('A'), Key('B'), Key('C'), Key('D'), Key('E'),
                                         Key('F'), Key('G'), Key('H'), Key('I'), Key('J') };

      static const value_type values2[]{ Key('A'), Key('B'), Key('C'), Key('D'), Key('E'),
                                         Key('F'), Key('G'), Key('H'), Key('I'), Key('J') };

      static const value_type values3[]{ Key('A'), Key('B'), Key('C'), Key('D'), Key('E'),
                                         Key('F'), Key('G'), Key('G'), Key('I'), Key('J') };

      static const value_type values4[]{ Key('A'), Key('B'), Key('C'), Key('D'), Key('E'),
                                         Key('F'), Key('G'), Key('H'), Key('I'), Key('J'), Key('K') };

      static const Data data1(values1);
      static const Data data2(values2);
      static const Data data3(values3);
      static const Data data4(values4);

      static const bool equal12 = (data1 == data2);
      static const bool equal13 = (data1 == data3);
      static const bool equal14 = (data1 == data4);

      CHECK_TRUE(equal12);
      CHECK_FALSE(equal13);
      CHECK_FALSE(equal14);
    }

    //*************************************************************************
    TEST(test_equal_with_transparent_comparator)
    {
      static const value_type values1[]{ Key('A'), Key('B'), Key('C'), Key('D'), Key('E'),
                                         Key('F'), Key('G'), Key('H'), Key('I'), Key('J') };

      static const value_type values2[]{ Key('A'), Key('B'), Key('C'), Key('D'), Key('E'),
                                         Key('F'), Key('G'), Key('H'), Key('I'), Key('J') };

      static const value_type values3[]{ Key('A'), Key('B'), Key('C'), Key('D'), Key('E'),
                                         Key('F'), Key('G'), Key('G'), Key('I'), Key('J') };

      static const value_type values4[]{ Key('A'), Key('B'), Key('C'), Key('D'), Key('E'),
                                         Key('F'), Key('G'), Key('H'), Key('I'), Key('J'), Key('K') };

      static const DataTransparentComparator data1(values1);
      static const DataTransparentComparator data2(values2);
      static const DataTransparentComparator data3(values3);
      static const DataTransparentComparator data4(values4);

      static const bool equal12 = (data1 == data2);
      static const bool equal13 = (data1 == data3);
      static const bool equal14 = (data1 == data4);

      CHECK_TRUE(equal12);
      CHECK_FALSE(equal13);
      CHECK_FALSE(equal14);
    }

    //*************************************************************************
    TEST(test_not_equal)
    {
      static const value_type values1[]{ Key('A'), Key('B'), Key('C'), Key('D'), Key('E'),
                                         Key('F'), Key('G'), Key('H'), Key('I'), Key('J') };

      static const value_type values2[]{ Key('A'), Key('B'), Key('C'), Key('D'), Key('E'),
                                         Key('F'), Key('G'), Key('H'), Key('I'), Key('J') };

      static const value_type values3[]{ Key('A'), Key('B'), Key('C'), Key('D'), Key('E'),
                                         Key('F'), Key('G'), Key('G'), Key('I'), Key('J') };

      static const value_type values4[]{ Key('A'), Key('B'), Key('C'), Key('D'), Key('E'),
                                         Key('F'), Key('G'), Key('H'), Key('I'), Key('J'), Key('K') };

      static const Data data1(values1);
      static const Data data2(values2);
      static const Data data3(values3);
      static const Data data4(values4);

      static const bool not_equal12 = (data1 != data2);
      static const bool not_equal13 = (data1 != data3);
      static const bool not_equal14 = (data1 != data4);

      CHECK_FALSE(not_equal12);
      CHECK_TRUE(not_equal13);
      CHECK_TRUE(not_equal14);
    }

    //*************************************************************************
    TEST(test_not_equal_with_transparent_comparator)
    {
      static const value_type values1[]{ Key('A'), Key('B'), Key('C'), Key('D'), Key('E'),
                                         Key('F'), Key('G'), Key('H'), Key('I'), Key('J') };

      static const value_type values2[]{ Key('A'), Key('B'), Key('C'), Key('D'), Key('E'),
                                         Key('F'), Key('G'), Key('H'), Key('I'), Key('J') };

      static const value_type values3[]{ Key('A'), Key('B'), Key('C'), Key('D'), Key('E'),
                                         Key('F'), Key('G'), Key('G'), Key('I'), Key('J') };

      static const value_type values4[]{ Key('A'), Key('B'), Key('C'), Key('D'), Key('E'),
                                         Key('F'), Key('G'), Key('H'), Key('I'), Key('J'), Key('K') };

      static const DataTransparentComparator data1(values1);
      static const DataTransparentComparator data2(values2);
      static const DataTransparentComparator data3(values3);
      static const DataTransparentComparator data4(values4);

      static const bool not_equal12 = (data1 != data2);
      static const bool not_equal13 = (data1 != data3);
      static const bool not_equal14 = (data1 != data4);

      CHECK_FALSE(not_equal12);
      CHECK_TRUE(not_equal13);
      CHECK_TRUE(not_equal14);
    }

    //*************************************************************************
    TEST(test_less_than)
    {
      static const value_type values1[]{ Key('A'), Key('B'), Key('B'), Key('D'), Key('E'),
                                         Key('F'), Key('G'), Key('H'), Key('I'), Key('J') };

      static const value_type values2[]{ Key('A'), Key('B'), Key('C'), Key('D'), Key('E'),
                                         Key('F'), Key('G'), Key('H'), Key('I'), Key('J') };

      static const value_type values3[]{ Key('A'), Key('B'), Key('D'), Key('D'), Key('E'),
                                         Key('F'), Key('G'), Key('H'), Key('I'), Key('J') };

      static const Data data1(values1);
      static const Data data2(values2);
      static const Data data3(values3);

      static const bool less_than12 = (data1 < data2);
      static const bool less_than23 = (data2 < data3);
      static const bool less_than21 = (data2 < data1);
      static const bool less_than32 = (data3 < data2);

#ifdef TEST_GREATER_THAN
      CHECK_FALSE(less_than12);
      CHECK_FALSE(less_than23);
      CHECK_TRUE(less_than21);
      CHECK_TRUE(less_than32);
#else
      CHECK_TRUE(less_than12);
      CHECK_TRUE(less_than23);
      CHECK_FALSE(less_than21);
      CHECK_FALSE(less_than32);
#endif
    }

    //*************************************************************************
    TEST(test_less_than_with_transparent_comparator)
    {
      static const value_type values1[]{ Key('A'), Key('B'), Key('B'), Key('D'), Key('E'),
                                         Key('F'), Key('G'), Key('H'), Key('I'), Key('J') };

      static const value_type values2[]{ Key('A'), Key('B'), Key('C'), Key('D'), Key('E'),
                                         Key('F'), Key('G'), Key('H'), Key('I'), Key('J') };

      static const value_type values3[]{ Key('A'), Key('B'), Key('D'), Key('D'), Key('E'),
                                         Key('F'), Key('G'), Key('H'), Key('I'), Key('J') };

      static const DataTransparentComparator data1(values1);
      static const DataTransparentComparator data2(values2);
      static const DataTransparentComparator data3(values3);

      static const bool less_than12 = (data1 < data2);
      static const bool less_than23 = (data2 < data3);
      static const bool less_than21 = (data2 < data1);
      static const bool less_than32 = (data3 < data2);

#ifdef TEST_GREATER_THAN
      CHECK_FALSE(less_than12);
      CHECK_FALSE(less_than23);
      CHECK_TRUE(less_than21);
      CHECK_TRUE(less_than32);
#else
      CHECK_TRUE(less_than12);
      CHECK_TRUE(less_than23);
      CHECK_FALSE(less_than21);
      CHECK_FALSE(less_than32);
#endif
    }

    //*************************************************************************
    TEST(test_less_than_equal)
    {
      static const value_type values1[]{ Key('A'), Key('B'), Key('B'), Key('D'), Key('E'),
                                         Key('F'), Key('G'), Key('H'), Key('I'), Key('J') };

      static const value_type values2[]{ Key('A'), Key('B'), Key('C'), Key('D'), Key('E'),
                                         Key('F'), Key('G'), Key('H'), Key('I'), Key('J') };

      static const value_type values3[]{ Key('A'), Key('B'), Key('D'), Key('D'), Key('E'),
                                         Key('F'), Key('G'), Key('H'), Key('I'), Key('J') };

      static const Data data1(values1);
      static const Data data2(values2);
      static const Data data3(values3);

      static const bool less_than_equal12 = (data1 <= data2);
      static const bool less_than_equal23 = (data2 <= data3);
      static const bool less_than_equal21 = (data2 <= data1);
      static const bool less_than_equal32 = (data3 <= data2);
      static const bool less_than_equal11 = (data1 <= data1);

#ifdef TEST_GREATER_THAN
      CHECK_FALSE(less_than_equal12);
      CHECK_FALSE(less_than_equal23);
      CHECK_TRUE(less_than_equal21);
      CHECK_TRUE(less_than_equal32);
      CHECK_TRUE(less_than_equal11);
#else
      CHECK_TRUE(less_than_equal12);
      CHECK_TRUE(less_than_equal23);
      CHECK_FALSE(less_than_equal21);
      CHECK_FALSE(less_than_equal32);
      CHECK_TRUE(less_than_equal11);
#endif
    }

    //*************************************************************************
    TEST(test_less_than_equal_with_transparent_comparator)
    {
      static const value_type values1[]{ Key('A'), Key('B'), Key('B'), Key('D'), Key('E'),
                                         Key('F'), Key('G'), Key('H'), Key('I'), Key('J') };

      static const value_type values2[]{ Key('A'), Key('B'), Key('C'), Key('D'), Key('E'),
                                         Key('F'), Key('G'), Key('H'), Key('I'), Key('J') };

      static const value_type values3[]{ Key('A'), Key('B'), Key('D'), Key('D'), Key('E'),
                                         Key('F'), Key('G'), Key('H'), Key('I'), Key('J') };

      static const DataTransparentComparator data1(values1);
      static const DataTransparentComparator data2(values2);
      static const DataTransparentComparator data3(values3);

      static const bool less_than_equal12 = (data1 <= data2);
      static const bool less_than_equal23 = (data2 <= data3);
      static const bool less_than_equal21 = (data2 <= data1);
      static const bool less_than_equal32 = (data3 <= data2);
      static const bool less_than_equal11 = (data1 <= data1);

#ifdef TEST_GREATER_THAN
      CHECK_FALSE(less_than_equal12);
      CHECK_FALSE(less_than_equal23);
      CHECK_TRUE(less_than_equal21);
      CHECK_TRUE(less_than_equal32);
      CHECK_TRUE(less_than_equal11);
#else
      CHECK_TRUE(less_than_equal12);
      CHECK_TRUE(less_than_equal23);
      CHECK_FALSE(less_than_equal21);
      CHECK_FALSE(less_than_equal32);
      CHECK_TRUE(less_than_equal11);
#endif
    }

    //*************************************************************************
    TEST(test_greater_than)
    {
      static const value_type values1[]{ Key('A'), Key('B'), Key('B'), Key('D'), Key('E'),
                                         Key('F'), Key('G'), Key('H'), Key('I'), Key('J') };

      static const value_type values2[]{ Key('A'), Key('B'), Key('C'), Key('D'), Key('E'),
                                         Key('F'), Key('G'), Key('H'), Key('I'), Key('J') };

      static const value_type values3[]{ Key('A'), Key('B'), Key('D'), Key('D'), Key('E'),
                                         Key('F'), Key('G'), Key('H'), Key('I'), Key('J') };

      static const Data data1(values1);
      static const Data data2(values2);
      static const Data data3(values3);

      static const bool greater_than12 = (data1 > data2);
      static const bool greater_than23 = (data2 > data3);
      static const bool greater_than21 = (data2 > data1);
      static const bool greater_than32 = (data3 > data2);

#ifdef TEST_GREATER_THAN
      CHECK_TRUE(greater_than12);
      CHECK_TRUE(greater_than23);
      CHECK_FALSE(greater_than21);
      CHECK_FALSE(greater_than32);
#else
      CHECK_FALSE(greater_than12);
      CHECK_FALSE(greater_than23);
      CHECK_TRUE(greater_than21);
      CHECK_TRUE(greater_than32);
#endif
    }

    //*************************************************************************
    TEST(test_greater_than_with_transparent_comparator)
    {
      static const value_type values1[]{ Key('A'), Key('B'), Key('B'), Key('D'), Key('E'),
                                         Key('F'), Key('G'), Key('H'), Key('I'), Key('J') };

      static const value_type values2[]{ Key('A'), Key('B'), Key('C'), Key('D'), Key('E'),
                                         Key('F'), Key('G'), Key('H'), Key('I'), Key('J') };

      static const value_type values3[]{ Key('A'), Key('B'), Key('D'), Key('D'), Key('E'),
                                         Key('F'), Key('G'), Key('H'), Key('I'), Key('J') };

      static const DataTransparentComparator data1(values1);
      static const DataTransparentComparator data2(values2);
      static const DataTransparentComparator data3(values3);

      static const bool greater_than12 = (data1 > data2);
      static const bool greater_than23 = (data2 > data3);
      static const bool greater_than21 = (data2 > data1);
      static const bool greater_than32 = (data3 > data2);

#ifdef TEST_GREATER_THAN
      CHECK_TRUE(greater_than12);
      CHECK_TRUE(greater_than23);
      CHECK_FALSE(greater_than21);
      CHECK_FALSE(greater_than32);
#else
      CHECK_FALSE(greater_than12);
      CHECK_FALSE(greater_than23);
      CHECK_TRUE(greater_than21);
      CHECK_TRUE(greater_than32);
#endif
    }

    //*************************************************************************
    TEST(test_greater_than_equal)
    {
      static const value_type values1[]{ Key('A'), Key('B'), Key('B'), Key('D'), Key('E'),
                                         Key('F'), Key('G'), Key('H'), Key('I'), Key('J') };

      static const value_type values2[]{ Key('A'), Key('B'), Key('C'), Key('D'), Key('E'),
                                         Key('F'), Key('G'), Key('H'), Key('I'), Key('J') };

      static const value_type values3[]{ Key('A'), Key('B'), Key('D'), Key('D'), Key('E'),
                                         Key('F'), Key('G'), Key('H'), Key('I'), Key('J') };

      static const Data data1(values1);
      static const Data data2(values2);
      static const Data data3(values3);

      static const bool greater_than_equal12 = (data1 >= data2);
      static const bool greater_than_equal23 = (data2 >= data3);
      static const bool greater_than_equal21 = (data2 >= data1);
      static const bool greater_than_equal32 = (data3 >= data2);
      static const bool greater_than_equal11 = (data1 >= data1);

#ifdef TEST_GREATER_THAN
      CHECK_TRUE(greater_than_equal12);
      CHECK_TRUE(greater_than_equal23);
      CHECK_FALSE(greater_than_equal21);
      CHECK_FALSE(greater_than_equal32);
      CHECK_TRUE(greater_than_equal11);
#else
      CHECK_FALSE(greater_than_equal12);
      CHECK_FALSE(greater_than_equal23);
      CHECK_TRUE(greater_than_equal21);
      CHECK_TRUE(greater_than_equal32);
      CHECK_TRUE(greater_than_equal11);
#endif
    }

    //*************************************************************************
    TEST(test_greater_than_equal_with_transparent_comparator)
    {
      static const value_type values1[]{ Key('A'), Key('B'), Key('B'), Key('D'), Key('E'),
                                         Key('F'), Key('G'), Key('H'), Key('I'), Key('J') };

      static const value_type values2[]{ Key('A'), Key('B'), Key('C'), Key('D'), Key('E'),
                                         Key('F'), Key('G'), Key('H'), Key('I'), Key('J') };

      static const value_type values3[]{ Key('A'), Key('B'), Key('D'), Key('D'), Key('E'),
                                         Key('F'), Key('G'), Key('H'), Key('I'), Key('J') };

      static const DataTransparentComparator data1(values1);
      static const DataTransparentComparator data2(values2);
      static const DataTransparentComparator data3(values3);

      static const bool greater_than_equal12 = (data1 >= data2);
      static const bool greater_than_equal23 = (data2 >= data3);
      static const bool greater_than_equal21 = (data2 >= data1);
      static const bool greater_than_equal32 = (data3 >= data2);
      static const bool greater_than_equal11 = (data1 >= data1);

#ifdef TEST_GREATER_THAN
      CHECK_TRUE(greater_than_equal12);
      CHECK_TRUE(greater_than_equal23);
      CHECK_FALSE(greater_than_equal21);
      CHECK_FALSE(greater_than_equal32);
      CHECK_TRUE(greater_than_equal11);
#else
      CHECK_FALSE(greater_than_equal12);
      CHECK_FALSE(greater_than_equal23);
      CHECK_TRUE(greater_than_equal21);
      CHECK_TRUE(greater_than_equal32);
      CHECK_TRUE(greater_than_equal11);
#endif
    }
  };
}
