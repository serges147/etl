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

#include <map>
#include <array>
#include <algorithm>
#include <utility>
#include <iterator>
#include <string>
#include <vector>

#include "etl/const_map.h"
#include "etl/span.h"

#include "data.h"

namespace
{
  static constexpr size_t Max_Size = 10UL;

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

  #define TEST_GREATER_THAN
  #ifdef TEST_GREATER_THAN
    using Data                       = etl::const_map_ext<Key,  int, etl::greater<Key>>;
    using IData                      = etl::iconst_map<Key, int, etl::greater<Key>>;
    using DataTransparentComparator  = etl::const_map_ext<Key,  int, etl::greater<>>;
    using IDataTransparentComparator = etl::iconst_map<Key, int, etl::greater<>>;
  #else
    using Data                       = etl::const_map_ext<Key,  int, etl::less<Key>>;
    using IData                      = etl::iconst_map<Key, int, etl::less<Key>>;
    using DataTransparentComparator  = etl::const_map_ext<Key,  int, etl::less<>>;
    using IDataTransparentComparator = etl::iconst_map<Key, int, etl::less<>>;
  #endif

  using value_type     = Data::value_type;
  using key_type       = Data::key_type;
  using mapped_type    = Data::mapped_type;
  using const_iterator = Data::const_iterator;
  using span_type      = etl::span<const value_type, Max_Size>;

  SUITE(test_const_map_ext_constexpr)
  {
    //*************************************************************************
    TEST(test_default_constructor)
    {
      static constexpr Data data;

      static constexpr bool   is_valid      = data.is_valid();
      static constexpr size_t size          = data.size();
      static constexpr bool   empty         = data.empty();
      static constexpr bool   full          = data.full();
      static constexpr size_t capacity      = data.capacity();
      static constexpr size_t max_size      = data.max_size();
      static constexpr const_iterator begin = data.begin();
      static constexpr const_iterator end   = data.end();

      CHECK_TRUE(is_valid);
      CHECK_TRUE(size == 0UL);
      CHECK_TRUE(empty);
      CHECK_FALSE(full);
      CHECK_TRUE(capacity == 0);
      CHECK_TRUE(max_size == 0);
      CHECK_TRUE(begin == end);
    }

    //*************************************************************************
    TEST(test_constructor_min_size_from_span)
    {
      static constexpr value_type values[]{value_type{Key('A'), 0}};
      static constexpr etl::span<const value_type, 1> span(values);
      static constexpr Data data{ span };

      static constexpr bool   is_valid      = data.is_valid();
      static constexpr size_t size          = data.size();
      static constexpr bool   empty         = data.empty();
      static constexpr bool   full          = data.full();
      static constexpr size_t capacity      = data.capacity();
      static constexpr size_t max_size      = data.max_size();
      static constexpr const_iterator begin = data.begin();
      static constexpr const_iterator end   = data.end();

      CHECK_TRUE(is_valid);
      CHECK_TRUE(size == 1U);
      CHECK_FALSE(empty);
      CHECK_TRUE(full);
      CHECK_TRUE(capacity == span.size());
      CHECK_TRUE(max_size == span.size());
      CHECK_FALSE(begin == end);
    }

    //*************************************************************************
    TEST(test_constructor_max_size_from_span)
    {
#ifdef TEST_GREATER_THAN
      static constexpr value_type values[]{ value_type{Key('J'), 9 }, value_type{Key('I'), 8 }, value_type{Key('H'), 7 }, value_type{Key('G'), 6 }, value_type{Key('F'), 5 },
                                        value_type{Key('E'), 4 }, value_type{Key('D'), 3 }, value_type{Key('C'), 2 }, value_type{Key('B'), 1 }, value_type{Key('A'), 0 } };
#else
      static constexpr value_type values[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                        value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };
#endif

      static constexpr span_type span(values);
      static constexpr Data data{ span };

      static constexpr bool   is_valid      = data.is_valid();
      static constexpr size_t size          = data.size();
      static constexpr bool   empty         = data.empty();
      static constexpr bool   full          = data.full();
      static constexpr size_t capacity      = data.capacity();
      static constexpr size_t max_size      = data.max_size();
      static constexpr const_iterator begin = data.begin();
      static constexpr const_iterator end   = data.end();

      CHECK_TRUE(is_valid);
      CHECK_TRUE(size == Max_Size);
      CHECK_FALSE(empty);
      CHECK_TRUE(full);
      CHECK_TRUE(capacity == Max_Size);
      CHECK_TRUE(max_size == Max_Size);
      CHECK_FALSE(begin == end);
    }

    //*************************************************************************
    TEST(test_constructor_max_size_from_array)
    {
#ifdef TEST_GREATER_THAN
      static constexpr value_type values[]{ value_type{Key('J'), 9 }, value_type{Key('I'), 8 }, value_type{Key('H'), 7 }, value_type{Key('G'), 6 }, value_type{Key('F'), 5 },
                                        value_type{Key('E'), 4 }, value_type{Key('D'), 3 }, value_type{Key('C'), 2 }, value_type{Key('B'), 1 }, value_type{Key('A'), 0 } };
#else
      static constexpr value_type values[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                        value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };
#endif

      static constexpr Data data{ values };

      static constexpr bool   is_valid      = data.is_valid();
      static constexpr size_t size          = data.size();
      static constexpr bool   empty         = data.empty();
      static constexpr bool   full          = data.full();
      static constexpr size_t capacity      = data.capacity();
      static constexpr size_t max_size      = data.max_size();
      static constexpr const_iterator begin = data.begin();
      static constexpr const_iterator end   = data.end();

      CHECK_TRUE(is_valid);
      CHECK_TRUE(size == Max_Size);
      CHECK_FALSE(empty);
      CHECK_TRUE(full);
      CHECK_TRUE(capacity == Max_Size);
      CHECK_TRUE(max_size == Max_Size);
      CHECK_FALSE(begin == end);
    }

    //*************************************************************************
    TEST(test_constructor_max_size_status_from_iconst_map)
    {
#ifdef TEST_GREATER_THAN
      static constexpr value_type values[]{ value_type{Key('J'), 9 }, value_type{Key('I'), 8 }, value_type{Key('H'), 7 }, value_type{Key('G'), 6 }, value_type{Key('F'), 5 },
                                            value_type{Key('E'), 4 }, value_type{Key('D'), 3 }, value_type{Key('C'), 2 }, value_type{Key('B'), 1 }, value_type{Key('A'), 0 } };
#else
      static constexpr value_type values[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                            value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };
#endif

      static constexpr Data data{ values };

      static constexpr const IData& idata = data;

      static constexpr bool   is_valid      = idata.is_valid();
      static constexpr size_t size          = idata.size();
      static constexpr bool   empty         = idata.empty();
      static constexpr bool   full          = idata.full();
      static constexpr size_t capacity      = idata.capacity();
      static constexpr size_t max_size      = idata.max_size();
      static constexpr const_iterator begin = idata.begin();
      static constexpr const_iterator end   = idata.end();

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
      static constexpr value_type values[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                            value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };

      static constexpr span_type span(values);
      static constexpr etl::const_map_ext data{span};

      etl::const_map<Key, int, 10U> check{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                           value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };

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
      static constexpr value_type values[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                            value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };
      
      static constexpr etl::const_map_ext data{values};
      
      etl::const_map<Key, int, 10U> check{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                           value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };

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
    TEST(test_begin)
    {
#ifdef TEST_GREATER_THAN
      static constexpr value_type values[]{ value_type{Key('J'), 9 }, value_type{Key('I'), 8 }, value_type{Key('H'), 7 }, value_type{Key('G'), 6 }, value_type{Key('F'), 5 },
                                            value_type{Key('E'), 4 }, value_type{Key('D'), 3 }, value_type{Key('C'), 2 }, value_type{Key('B'), 1 }, value_type{Key('A'), 0 } };
#else
      static constexpr value_type values[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                            value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };
#endif

      static constexpr Data data{ values };

      CHECK_TRUE(data.is_valid());
      static constexpr auto value = *data.begin();

#ifdef TEST_GREATER_THAN
      CHECK_TRUE((value_type{ Key('J'), 9 }) == value);
#else
      CHECK_TRUE((value_type{ Key('A'), 0 }) == value);
#endif
    }

    //*************************************************************************
    TEST(test_end)
    {
      static constexpr value_type values[]{ value_type{Key('J'), 9 }, value_type{Key('I'), 8 }, value_type{Key('H'), 7 }, value_type{Key('G'), 6 }, value_type{Key('F'), 5 },
                                            value_type{Key('E'), 4 }, value_type{Key('D'), 3 }, value_type{Key('C'), 2 }, value_type{Key('B'), 1 }, value_type{Key('A'), 0 } };

      static constexpr Data data{ values };

      static constexpr const_iterator end_itr = data.end();

      CHECK_TRUE(end_itr == (data.begin() + data.size()));
    }

    //*************************************************************************
    TEST(test_index)
    {
#ifdef TEST_GREATER_THAN
      static constexpr value_type values[]{ value_type{Key('J'), 9 }, value_type{Key('I'), 8 }, value_type{Key('H'), 7 }, value_type{Key('G'), 6 }, value_type{Key('F'), 5 },
                                            value_type{Key('E'), 4 }, value_type{Key('D'), 3 }, value_type{Key('C'), 2 }, value_type{Key('B'), 1 }, value_type{Key('A'), 0 } };
#else
      static constexpr value_type values[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                            value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };
#endif

      static constexpr Data data{ values };

      static constexpr mapped_type atA = data[Key('A')];
      static constexpr mapped_type atB = data[Key('B')];
      static constexpr mapped_type atC = data[Key('C')];
      static constexpr mapped_type atD = data[Key('D')];
      static constexpr mapped_type atE = data[Key('E')];
      static constexpr mapped_type atF = data[Key('F')];
      static constexpr mapped_type atG = data[Key('G')];
      static constexpr mapped_type atH = data[Key('H')];
      static constexpr mapped_type atI = data[Key('I')];
      static constexpr mapped_type atJ = data[Key('J')];

      CHECK_TRUE(data.is_valid());
      CHECK(atA == 0);
      CHECK(atB == 1);
      CHECK(atC == 2);
      CHECK(atD == 3);
      CHECK(atE == 4);
      CHECK(atF == 5);
      CHECK(atG == 6);
      CHECK(atH == 7);
      CHECK(atI == 8);
      CHECK(atJ == 9);
    }

    //*************************************************************************
    TEST(test_index_using_transparent_comparator)
    {
#ifdef TEST_GREATER_THAN
      static constexpr value_type values[]{ value_type{Key('J'), 9 }, value_type{Key('I'), 8 }, value_type{Key('H'), 7 }, value_type{Key('G'), 6 }, value_type{Key('F'), 5 },
                                            value_type{Key('E'), 4 }, value_type{Key('D'), 3 }, value_type{Key('C'), 2 }, value_type{Key('B'), 1 }, value_type{Key('A'), 0 } };
#else
      static constexpr value_type values[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                            value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };
#endif

      static constexpr DataTransparentComparator data{ values };

      static constexpr mapped_type atA = data['A'];
      static constexpr mapped_type atB = data['B'];
      static constexpr mapped_type atC = data['C'];
      static constexpr mapped_type atD = data['D'];
      static constexpr mapped_type atE = data['E'];
      static constexpr mapped_type atF = data['F'];
      static constexpr mapped_type atG = data['G'];
      static constexpr mapped_type atH = data['H'];
      static constexpr mapped_type atI = data['I'];
      static constexpr mapped_type atJ = data['J'];

      CHECK_TRUE(data.is_valid());
      CHECK(atA == 0);
      CHECK(atB == 1);
      CHECK(atC == 2);
      CHECK(atD == 3);
      CHECK(atE == 4);
      CHECK(atF == 5);
      CHECK(atG == 6);
      CHECK(atH == 7);
      CHECK(atI == 8);
      CHECK(atJ == 9);
    }

    //*************************************************************************
    TEST(test_at)
    {
#ifdef TEST_GREATER_THAN
      static constexpr value_type values[]{ value_type{Key('J'), 9 }, value_type{Key('I'), 8 }, value_type{Key('H'), 7 }, value_type{Key('G'), 6 }, value_type{Key('F'), 5 },
                                            value_type{Key('E'), 4 }, value_type{Key('D'), 3 }, value_type{Key('C'), 2 }, value_type{Key('B'), 1 }, value_type{Key('A'), 0 } };
#else
      static constexpr value_type values[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                            value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };
#endif

      static constexpr Data data{ values };

      static constexpr mapped_type atA = data.at(Key('A'));
      static constexpr mapped_type atB = data.at(Key('B'));
      static constexpr mapped_type atC = data.at(Key('C'));
      static constexpr mapped_type atD = data.at(Key('D'));
      static constexpr mapped_type atE = data.at(Key('E'));
      static constexpr mapped_type atF = data.at(Key('F'));
      static constexpr mapped_type atG = data.at(Key('G'));
      static constexpr mapped_type atH = data.at(Key('H'));
      static constexpr mapped_type atI = data.at(Key('I'));
      static constexpr mapped_type atJ = data.at(Key('J'));

      CHECK_TRUE(data.is_valid());
      CHECK(atA == 0);
      CHECK(atB == 1);
      CHECK(atC == 2);
      CHECK(atD == 3);
      CHECK(atE == 4);
      CHECK(atF == 5);
      CHECK(atG == 6);
      CHECK(atH == 7);
      CHECK(atI == 8);
      CHECK(atJ == 9);
    }

    //*************************************************************************
    TEST(test_at_using_transparent_comparator)
    {
#ifdef TEST_GREATER_THAN
      static constexpr value_type values[]{ value_type{Key('J'), 9 }, value_type{Key('I'), 8 }, value_type{Key('H'), 7 }, value_type{Key('G'), 6 }, value_type{Key('F'), 5 },
                                            value_type{Key('E'), 4 }, value_type{Key('D'), 3 }, value_type{Key('C'), 2 }, value_type{Key('B'), 1 }, value_type{Key('A'), 0 } };
#else
      static constexpr value_type values[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                            value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };
#endif

      static constexpr DataTransparentComparator data{ values };

      static constexpr mapped_type atA = data.at('A');
      static constexpr mapped_type atB = data.at('B');
      static constexpr mapped_type atC = data.at('C');
      static constexpr mapped_type atD = data.at('D');
      static constexpr mapped_type atE = data.at('E');
      static constexpr mapped_type atF = data.at('F');
      static constexpr mapped_type atG = data.at('G');
      static constexpr mapped_type atH = data.at('H');
      static constexpr mapped_type atI = data.at('I');
      static constexpr mapped_type atJ = data.at('J');

      CHECK_TRUE(data.is_valid());
      CHECK(atA == 0);
      CHECK(atB == 1);
      CHECK(atC == 2);
      CHECK(atD == 3);
      CHECK(atE == 4);
      CHECK(atF == 5);
      CHECK(atG == 6);
      CHECK(atH == 7);
      CHECK(atI == 8);
      CHECK(atJ == 9);
    }

    //*************************************************************************
    TEST(test_equal_range)
    {
#ifdef TEST_GREATER_THAN
      static constexpr value_type values[]{ value_type{Key('J'), 9 }, value_type{Key('I'), 8 }, value_type{Key('H'), 7 }, value_type{Key('G'), 6 }, value_type{Key('F'), 5 },
                                            value_type{Key('E'), 4 }, value_type{Key('D'), 3 }, value_type{Key('C'), 2 }, value_type{Key('B'), 1 }, value_type{Key('A'), 0 } };
#else
      static constexpr value_type values[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                            value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };
#endif

      static constexpr Data data{ values };

      static constexpr ETL_OR_STD::pair<const_iterator, const_iterator> resultA = data.equal_range(Key('A'));
      static constexpr ETL_OR_STD::pair<const_iterator, const_iterator> resultB = data.equal_range(Key('B'));
      static constexpr ETL_OR_STD::pair<const_iterator, const_iterator> resultC = data.equal_range(Key('C'));
      static constexpr ETL_OR_STD::pair<const_iterator, const_iterator> resultD = data.equal_range(Key('D'));
      static constexpr ETL_OR_STD::pair<const_iterator, const_iterator> resultE = data.equal_range(Key('E'));
      static constexpr ETL_OR_STD::pair<const_iterator, const_iterator> resultF = data.equal_range(Key('F'));
      static constexpr ETL_OR_STD::pair<const_iterator, const_iterator> resultG = data.equal_range(Key('G'));
      static constexpr ETL_OR_STD::pair<const_iterator, const_iterator> resultH = data.equal_range(Key('H'));
      static constexpr ETL_OR_STD::pair<const_iterator, const_iterator> resultI = data.equal_range(Key('I'));
      static constexpr ETL_OR_STD::pair<const_iterator, const_iterator> resultJ = data.equal_range(Key('J'));

#ifdef TEST_GREATER_THAN
      CHECK_EQUAL(9, (std::distance(data.begin(), resultA.first)));
      CHECK_EQUAL(8, (std::distance(data.begin(), resultB.first)));
      CHECK_EQUAL(7, (std::distance(data.begin(), resultC.first)));
      CHECK_EQUAL(6, (std::distance(data.begin(), resultD.first)));
      CHECK_EQUAL(5, (std::distance(data.begin(), resultE.first)));
      CHECK_EQUAL(4, (std::distance(data.begin(), resultF.first)));
      CHECK_EQUAL(3, (std::distance(data.begin(), resultG.first)));
      CHECK_EQUAL(2, (std::distance(data.begin(), resultH.first)));
      CHECK_EQUAL(1, (std::distance(data.begin(), resultI.first)));
      CHECK_EQUAL(0, (std::distance(data.begin(), resultJ.first)));

      CHECK_EQUAL(10, (std::distance(data.begin(), resultA.second)));
      CHECK_EQUAL(9, (std::distance(data.begin(), resultB.second)));
      CHECK_EQUAL(8, (std::distance(data.begin(), resultC.second)));
      CHECK_EQUAL(7, (std::distance(data.begin(), resultD.second)));
      CHECK_EQUAL(6, (std::distance(data.begin(), resultE.second)));
      CHECK_EQUAL(5, (std::distance(data.begin(), resultF.second)));
      CHECK_EQUAL(4, (std::distance(data.begin(), resultG.second)));
      CHECK_EQUAL(3, (std::distance(data.begin(), resultH.second)));
      CHECK_EQUAL(2, (std::distance(data.begin(), resultI.second)));
      CHECK_EQUAL(1, (std::distance(data.begin(), resultJ.second)));
#else
      CHECK_EQUAL(0, (std::distance(data.begin(), resultA.first)));
      CHECK_EQUAL(1, (std::distance(data.begin(), resultB.first)));
      CHECK_EQUAL(2, (std::distance(data.begin(), resultC.first)));
      CHECK_EQUAL(3, (std::distance(data.begin(), resultD.first)));
      CHECK_EQUAL(4, (std::distance(data.begin(), resultE.first)));
      CHECK_EQUAL(5, (std::distance(data.begin(), resultF.first)));
      CHECK_EQUAL(6, (std::distance(data.begin(), resultG.first)));
      CHECK_EQUAL(7, (std::distance(data.begin(), resultH.first)));
      CHECK_EQUAL(8, (std::distance(data.begin(), resultI.first)));
      CHECK_EQUAL(9, (std::distance(data.begin(), resultJ.first)));

      CHECK_EQUAL(1, (std::distance(data.begin(), resultA.second)));
      CHECK_EQUAL(2, (std::distance(data.begin(), resultB.second)));
      CHECK_EQUAL(3, (std::distance(data.begin(), resultC.second)));
      CHECK_EQUAL(4, (std::distance(data.begin(), resultD.second)));
      CHECK_EQUAL(5, (std::distance(data.begin(), resultE.second)));
      CHECK_EQUAL(6, (std::distance(data.begin(), resultF.second)));
      CHECK_EQUAL(7, (std::distance(data.begin(), resultG.second)));
      CHECK_EQUAL(8, (std::distance(data.begin(), resultH.second)));
      CHECK_EQUAL(9, (std::distance(data.begin(), resultI.second)));
      CHECK_EQUAL(10, (std::distance(data.begin(), resultJ.second)));
#endif
    }

    //*************************************************************************
    TEST(test_equal_range_using_transparent_comparator)
    {
#ifdef TEST_GREATER_THAN
      static constexpr value_type values[]{ value_type{Key('J'), 9 }, value_type{Key('I'), 8 }, value_type{Key('H'), 7 }, value_type{Key('G'), 6 }, value_type{Key('F'), 5 },
                                            value_type{Key('E'), 4 }, value_type{Key('D'), 3 }, value_type{Key('C'), 2 }, value_type{Key('B'), 1 }, value_type{Key('A'), 0 } };
#else
      static constexpr value_type values[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                            value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };
#endif

      static constexpr DataTransparentComparator data{ values };

      static constexpr ETL_OR_STD::pair<const_iterator, const_iterator> resultA = data.equal_range('A');
      static constexpr ETL_OR_STD::pair<const_iterator, const_iterator> resultB = data.equal_range('B');
      static constexpr ETL_OR_STD::pair<const_iterator, const_iterator> resultC = data.equal_range('C');
      static constexpr ETL_OR_STD::pair<const_iterator, const_iterator> resultD = data.equal_range('D');
      static constexpr ETL_OR_STD::pair<const_iterator, const_iterator> resultE = data.equal_range('E');
      static constexpr ETL_OR_STD::pair<const_iterator, const_iterator> resultF = data.equal_range('F');
      static constexpr ETL_OR_STD::pair<const_iterator, const_iterator> resultG = data.equal_range('G');
      static constexpr ETL_OR_STD::pair<const_iterator, const_iterator> resultH = data.equal_range('H');
      static constexpr ETL_OR_STD::pair<const_iterator, const_iterator> resultI = data.equal_range('I');
      static constexpr ETL_OR_STD::pair<const_iterator, const_iterator> resultJ = data.equal_range('J');

#ifdef TEST_GREATER_THAN
      CHECK_EQUAL(9, (std::distance(data.begin(), resultA.first)));
      CHECK_EQUAL(8, (std::distance(data.begin(), resultB.first)));
      CHECK_EQUAL(7, (std::distance(data.begin(), resultC.first)));
      CHECK_EQUAL(6, (std::distance(data.begin(), resultD.first)));
      CHECK_EQUAL(5, (std::distance(data.begin(), resultE.first)));
      CHECK_EQUAL(4, (std::distance(data.begin(), resultF.first)));
      CHECK_EQUAL(3, (std::distance(data.begin(), resultG.first)));
      CHECK_EQUAL(2, (std::distance(data.begin(), resultH.first)));
      CHECK_EQUAL(1, (std::distance(data.begin(), resultI.first)));
      CHECK_EQUAL(0, (std::distance(data.begin(), resultJ.first)));

      CHECK_EQUAL(10, (std::distance(data.begin(), resultA.second)));
      CHECK_EQUAL(9, (std::distance(data.begin(), resultB.second)));
      CHECK_EQUAL(8, (std::distance(data.begin(), resultC.second)));
      CHECK_EQUAL(7, (std::distance(data.begin(), resultD.second)));
      CHECK_EQUAL(6, (std::distance(data.begin(), resultE.second)));
      CHECK_EQUAL(5, (std::distance(data.begin(), resultF.second)));
      CHECK_EQUAL(4, (std::distance(data.begin(), resultG.second)));
      CHECK_EQUAL(3, (std::distance(data.begin(), resultH.second)));
      CHECK_EQUAL(2, (std::distance(data.begin(), resultI.second)));
      CHECK_EQUAL(1, (std::distance(data.begin(), resultJ.second)));
#else
      CHECK_EQUAL(0, (std::distance(data.begin(), resultA.first)));
      CHECK_EQUAL(1, (std::distance(data.begin(), resultB.first)));
      CHECK_EQUAL(2, (std::distance(data.begin(), resultC.first)));
      CHECK_EQUAL(3, (std::distance(data.begin(), resultD.first)));
      CHECK_EQUAL(4, (std::distance(data.begin(), resultE.first)));
      CHECK_EQUAL(5, (std::distance(data.begin(), resultF.first)));
      CHECK_EQUAL(6, (std::distance(data.begin(), resultG.first)));
      CHECK_EQUAL(7, (std::distance(data.begin(), resultH.first)));
      CHECK_EQUAL(8, (std::distance(data.begin(), resultI.first)));
      CHECK_EQUAL(9, (std::distance(data.begin(), resultJ.first)));

      CHECK_EQUAL(1, (std::distance(data.begin(), resultA.second)));
      CHECK_EQUAL(2, (std::distance(data.begin(), resultB.second)));
      CHECK_EQUAL(3, (std::distance(data.begin(), resultC.second)));
      CHECK_EQUAL(4, (std::distance(data.begin(), resultD.second)));
      CHECK_EQUAL(5, (std::distance(data.begin(), resultE.second)));
      CHECK_EQUAL(6, (std::distance(data.begin(), resultF.second)));
      CHECK_EQUAL(7, (std::distance(data.begin(), resultG.second)));
      CHECK_EQUAL(8, (std::distance(data.begin(), resultH.second)));
      CHECK_EQUAL(9, (std::distance(data.begin(), resultI.second)));
      CHECK_EQUAL(10, (std::distance(data.begin(), resultJ.second)));
#endif
    }

    //*************************************************************************
    TEST(test_lower_bound)
    {
#ifdef TEST_GREATER_THAN
      static constexpr value_type values[]{ value_type{Key('K'), 10 }, value_type{Key('J'), 9 }, value_type{Key('I'), 8 }, value_type{Key('H'), 7 }, value_type{Key('G'), 6 },
                                            value_type{Key('F'), 5 }, value_type{Key('E'), 4 }, value_type{Key('C'), 2 }, value_type{Key('B'), 1 }, value_type{Key('A'), 0 } };
#else
      static constexpr value_type values[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('E'), 4 }, value_type{Key('F'), 5 }, 
                                            value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 }, value_type{Key('K'), 10 } };
#endif

      static constexpr Data data{ values };

      static constexpr const_iterator resultA = data.lower_bound(Key('A'));
      static constexpr const_iterator resultB = data.lower_bound(Key('B'));
      static constexpr const_iterator resultC = data.lower_bound(Key('C'));
      static constexpr const_iterator resultD = data.lower_bound(Key('D'));
      static constexpr const_iterator resultE = data.lower_bound(Key('E'));
      static constexpr const_iterator resultF = data.lower_bound(Key('F'));
      static constexpr const_iterator resultG = data.lower_bound(Key('G'));
      static constexpr const_iterator resultH = data.lower_bound(Key('H'));
      static constexpr const_iterator resultI = data.lower_bound(Key('I'));
      static constexpr const_iterator resultJ = data.lower_bound(Key('J'));
      static constexpr const_iterator resultK = data.lower_bound(Key('K'));

#ifdef TEST_GREATER_THAN
      CHECK_EQUAL(9, (std::distance(data.begin(), resultA)));
      CHECK_EQUAL(8, (std::distance(data.begin(), resultB)));
      CHECK_EQUAL(7, (std::distance(data.begin(), resultC)));
      CHECK_EQUAL(7, (std::distance(data.begin(), resultD)));
      CHECK_EQUAL(6, (std::distance(data.begin(), resultE)));
      CHECK_EQUAL(5, (std::distance(data.begin(), resultF)));
      CHECK_EQUAL(4, (std::distance(data.begin(), resultG)));
      CHECK_EQUAL(3, (std::distance(data.begin(), resultH)));
      CHECK_EQUAL(2, (std::distance(data.begin(), resultI)));
      CHECK_EQUAL(1, (std::distance(data.begin(), resultJ)));
      CHECK_EQUAL(0, (std::distance(data.begin(), resultK)));
#else
      CHECK_EQUAL(0, (std::distance(data.begin(), resultA)));
      CHECK_EQUAL(1, (std::distance(data.begin(), resultB)));
      CHECK_EQUAL(2, (std::distance(data.begin(), resultC)));
      CHECK_EQUAL(3, (std::distance(data.begin(), resultD)));
      CHECK_EQUAL(3, (std::distance(data.begin(), resultE)));
      CHECK_EQUAL(4, (std::distance(data.begin(), resultF)));
      CHECK_EQUAL(5, (std::distance(data.begin(), resultG)));
      CHECK_EQUAL(6, (std::distance(data.begin(), resultH)));
      CHECK_EQUAL(7, (std::distance(data.begin(), resultI)));
      CHECK_EQUAL(8, (std::distance(data.begin(), resultJ)));
      CHECK_EQUAL(9, (std::distance(data.begin(), resultK)));
#endif
    }

    //*************************************************************************
    TEST(test_lower_bound_using_transparent_comparator)
    {
#ifdef TEST_GREATER_THAN
      static constexpr value_type values[]{ value_type{Key('K'), 10 }, value_type{Key('J'), 9 }, value_type{Key('I'), 8 }, value_type{Key('H'), 7 }, value_type{Key('G'), 6 },
                                            value_type{Key('F'), 5 }, value_type{Key('E'), 4 }, value_type{Key('C'), 2 }, value_type{Key('B'), 1 }, value_type{Key('A'), 0 } };
#else
      static constexpr value_type values[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('E'), 4 }, value_type{Key('F'), 5 }, 
                                            value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 }, value_type{Key('K'), 10 } };
#endif

      static constexpr DataTransparentComparator data{ values };

      static constexpr const_iterator resultA = data.lower_bound('A');
      static constexpr const_iterator resultB = data.lower_bound('B');
      static constexpr const_iterator resultC = data.lower_bound('C');
      static constexpr const_iterator resultD = data.lower_bound('D');
      static constexpr const_iterator resultE = data.lower_bound('E');
      static constexpr const_iterator resultF = data.lower_bound('F');
      static constexpr const_iterator resultG = data.lower_bound('G');
      static constexpr const_iterator resultH = data.lower_bound('H');
      static constexpr const_iterator resultI = data.lower_bound('I');
      static constexpr const_iterator resultJ = data.lower_bound('J');
      static constexpr const_iterator resultK = data.lower_bound('K');

#ifdef TEST_GREATER_THAN
      CHECK_EQUAL(9, (std::distance(data.begin(), resultA)));
      CHECK_EQUAL(8, (std::distance(data.begin(), resultB)));
      CHECK_EQUAL(7, (std::distance(data.begin(), resultC)));
      CHECK_EQUAL(7, (std::distance(data.begin(), resultD)));
      CHECK_EQUAL(6, (std::distance(data.begin(), resultE)));
      CHECK_EQUAL(5, (std::distance(data.begin(), resultF)));
      CHECK_EQUAL(4, (std::distance(data.begin(), resultG)));
      CHECK_EQUAL(3, (std::distance(data.begin(), resultH)));
      CHECK_EQUAL(2, (std::distance(data.begin(), resultI)));
      CHECK_EQUAL(1, (std::distance(data.begin(), resultJ)));
      CHECK_EQUAL(0, (std::distance(data.begin(), resultK)));
#else
      CHECK_EQUAL(0, (std::distance(data.begin(), resultA)));
      CHECK_EQUAL(1, (std::distance(data.begin(), resultB)));
      CHECK_EQUAL(2, (std::distance(data.begin(), resultC)));
      CHECK_EQUAL(3, (std::distance(data.begin(), resultD)));
      CHECK_EQUAL(3, (std::distance(data.begin(), resultE)));
      CHECK_EQUAL(4, (std::distance(data.begin(), resultF)));
      CHECK_EQUAL(5, (std::distance(data.begin(), resultG)));
      CHECK_EQUAL(6, (std::distance(data.begin(), resultH)));
      CHECK_EQUAL(7, (std::distance(data.begin(), resultI)));
      CHECK_EQUAL(8, (std::distance(data.begin(), resultJ)));
      CHECK_EQUAL(9, (std::distance(data.begin(), resultK)));
#endif
    }

    //*************************************************************************
    TEST(test_upper_bound)
    {
#ifdef TEST_GREATER_THAN
      static constexpr value_type values[]{ value_type{Key('K'), 10 }, value_type{Key('J'), 9 }, value_type{Key('I'), 8 }, value_type{Key('H'), 7 }, value_type{Key('G'), 6 },
                                            value_type{Key('F'), 5 }, value_type{Key('E'), 4 }, value_type{Key('C'), 2 }, value_type{Key('B'), 1 }, value_type{Key('A'), 0 } };
#else
      static constexpr value_type values[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('E'), 4 }, value_type{Key('F'), 5 }, 
                                            value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 }, value_type{Key('K'), 10 } };
#endif

      static constexpr Data data{ values };

      static constexpr const_iterator resultA = data.upper_bound(Key('A'));
      static constexpr const_iterator resultB = data.upper_bound(Key('B'));
      static constexpr const_iterator resultC = data.upper_bound(Key('C'));
      static constexpr const_iterator resultD = data.upper_bound(Key('D'));
      static constexpr const_iterator resultE = data.upper_bound(Key('E'));
      static constexpr const_iterator resultF = data.upper_bound(Key('F'));
      static constexpr const_iterator resultG = data.upper_bound(Key('G'));
      static constexpr const_iterator resultH = data.upper_bound(Key('H'));
      static constexpr const_iterator resultI = data.upper_bound(Key('I'));
      static constexpr const_iterator resultJ = data.upper_bound(Key('J'));
      static constexpr const_iterator resultK = data.upper_bound(Key('K'));

#ifdef TEST_GREATER_THAN
      CHECK_EQUAL(10, (std::distance(data.begin(), resultA)));
      CHECK_EQUAL(9, (std::distance(data.begin(), resultB)));
      CHECK_EQUAL(8, (std::distance(data.begin(), resultC)));
      CHECK_EQUAL(7, (std::distance(data.begin(), resultD)));
      CHECK_EQUAL(7, (std::distance(data.begin(), resultE)));
      CHECK_EQUAL(6, (std::distance(data.begin(), resultF)));
      CHECK_EQUAL(5, (std::distance(data.begin(), resultG)));
      CHECK_EQUAL(4, (std::distance(data.begin(), resultH)));
      CHECK_EQUAL(3, (std::distance(data.begin(), resultI)));
      CHECK_EQUAL(2, (std::distance(data.begin(), resultJ)));
      CHECK_EQUAL(1, (std::distance(data.begin(), resultK)));
#else
      CHECK_EQUAL(1, (std::distance(data.begin(), resultA)));
      CHECK_EQUAL(2, (std::distance(data.begin(), resultB)));
      CHECK_EQUAL(3, (std::distance(data.begin(), resultC)));
      CHECK_EQUAL(3, (std::distance(data.begin(), resultD)));
      CHECK_EQUAL(4, (std::distance(data.begin(), resultE)));
      CHECK_EQUAL(5, (std::distance(data.begin(), resultF)));
      CHECK_EQUAL(6, (std::distance(data.begin(), resultG)));
      CHECK_EQUAL(7, (std::distance(data.begin(), resultH)));
      CHECK_EQUAL(8, (std::distance(data.begin(), resultI)));
      CHECK_EQUAL(9, (std::distance(data.begin(), resultJ)));
      CHECK_EQUAL(10, (std::distance(data.begin(), resultK)));
#endif
    }

    //*************************************************************************
    TEST(test_upper_bound_using_transparent_comparator)
    {
#ifdef TEST_GREATER_THAN
      static constexpr value_type values[]{ value_type{Key('K'), 10 }, value_type{Key('J'), 9 }, value_type{Key('I'), 8 }, value_type{Key('H'), 7 }, value_type{Key('G'), 6 },
                                            value_type{Key('F'), 5 }, value_type{Key('E'), 4 }, value_type{Key('C'), 2 }, value_type{Key('B'), 1 }, value_type{Key('A'), 0 } };
#else
      static constexpr value_type values[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('E'), 4 }, value_type{Key('F'), 5 }, 
                                            value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 }, value_type{Key('K'), 10 } };
#endif

      static constexpr DataTransparentComparator data{ values };

      static constexpr const_iterator resultA = data.upper_bound('A');
      static constexpr const_iterator resultB = data.upper_bound('B');
      static constexpr const_iterator resultC = data.upper_bound('C');
      static constexpr const_iterator resultD = data.upper_bound('D');
      static constexpr const_iterator resultE = data.upper_bound('E');
      static constexpr const_iterator resultF = data.upper_bound('F');
      static constexpr const_iterator resultG = data.upper_bound('G');
      static constexpr const_iterator resultH = data.upper_bound('H');
      static constexpr const_iterator resultI = data.upper_bound('I');
      static constexpr const_iterator resultJ = data.upper_bound('J');
      static constexpr const_iterator resultK = data.upper_bound('K');

#ifdef TEST_GREATER_THAN
      CHECK_EQUAL(10, (std::distance(data.begin(), resultA)));
      CHECK_EQUAL(9, (std::distance(data.begin(), resultB)));
      CHECK_EQUAL(8, (std::distance(data.begin(), resultC)));
      CHECK_EQUAL(7, (std::distance(data.begin(), resultD)));
      CHECK_EQUAL(7, (std::distance(data.begin(), resultE)));
      CHECK_EQUAL(6, (std::distance(data.begin(), resultF)));
      CHECK_EQUAL(5, (std::distance(data.begin(), resultG)));
      CHECK_EQUAL(4, (std::distance(data.begin(), resultH)));
      CHECK_EQUAL(3, (std::distance(data.begin(), resultI)));
      CHECK_EQUAL(2, (std::distance(data.begin(), resultJ)));
      CHECK_EQUAL(1, (std::distance(data.begin(), resultK)));
#else
      CHECK_EQUAL(1, (std::distance(data.begin(), resultA)));
      CHECK_EQUAL(2, (std::distance(data.begin(), resultB)));
      CHECK_EQUAL(3, (std::distance(data.begin(), resultC)));
      CHECK_EQUAL(3, (std::distance(data.begin(), resultD)));
      CHECK_EQUAL(4, (std::distance(data.begin(), resultE)));
      CHECK_EQUAL(5, (std::distance(data.begin(), resultF)));
      CHECK_EQUAL(6, (std::distance(data.begin(), resultG)));
      CHECK_EQUAL(7, (std::distance(data.begin(), resultH)));
      CHECK_EQUAL(8, (std::distance(data.begin(), resultI)));
      CHECK_EQUAL(9, (std::distance(data.begin(), resultJ)));
      CHECK_EQUAL(10, (std::distance(data.begin(), resultK)));
#endif
    }

    //*************************************************************************
    TEST(test_count)
    {
#ifdef TEST_GREATER_THAN
      static constexpr value_type values[]{ value_type{Key('J'), 9 }, value_type{Key('I'), 8 }, value_type{Key('H'), 7 }, value_type{Key('G'), 6 }, value_type{Key('F'), 5 },
                                            value_type{Key('E'), 4 }, value_type{Key('D'), 3 }, value_type{Key('C'), 2 }, value_type{Key('B'), 1 }, value_type{Key('A'), 0 } };
#else
      static constexpr value_type values[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                            value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };
#endif

      static constexpr Data data{ values };

      static constexpr size_t countA = data.count(Key('A'));
      static constexpr size_t countB = data.count(Key('B'));
      static constexpr size_t countC = data.count(Key('C'));
      static constexpr size_t countD = data.count(Key('D'));
      static constexpr size_t countE = data.count(Key('E'));
      static constexpr size_t countF = data.count(Key('F'));
      static constexpr size_t countG = data.count(Key('G'));
      static constexpr size_t countH = data.count(Key('H'));
      static constexpr size_t countI = data.count(Key('I'));
      static constexpr size_t countJ = data.count(Key('J'));
      static constexpr size_t countK = data.count(Key('K'));

      CHECK_EQUAL(1, countA);
      CHECK_EQUAL(1, countB);
      CHECK_EQUAL(1, countC);
      CHECK_EQUAL(1, countD);
      CHECK_EQUAL(1, countE);
      CHECK_EQUAL(1, countF);
      CHECK_EQUAL(1, countG);
      CHECK_EQUAL(1, countH);
      CHECK_EQUAL(1, countI);
      CHECK_EQUAL(1, countJ);
      CHECK_EQUAL(0, countK);
    }

    //*************************************************************************
    TEST(test_count_using_transparent_comparator)
    {
#ifdef TEST_GREATER_THAN
      static constexpr value_type values[]{ value_type{Key('J'), 9 }, value_type{Key('I'), 8 }, value_type{Key('H'), 7 }, value_type{Key('G'), 6 }, value_type{Key('F'), 5 },
                                            value_type{Key('E'), 4 }, value_type{Key('D'), 3 }, value_type{Key('C'), 2 }, value_type{Key('B'), 1 }, value_type{Key('A'), 0 } };
#else
      static constexpr value_type values[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                            value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };
#endif

      static constexpr DataTransparentComparator data{ values };

      static constexpr size_t countA = data.count('A');
      static constexpr size_t countB = data.count('B');
      static constexpr size_t countC = data.count('C');
      static constexpr size_t countD = data.count('D');
      static constexpr size_t countE = data.count('E');
      static constexpr size_t countF = data.count('F');
      static constexpr size_t countG = data.count('G');
      static constexpr size_t countH = data.count('H');
      static constexpr size_t countI = data.count('I');
      static constexpr size_t countJ = data.count('J');
      static constexpr size_t countK = data.count('K');

      CHECK_EQUAL(1, countA);
      CHECK_EQUAL(1, countB);
      CHECK_EQUAL(1, countC);
      CHECK_EQUAL(1, countD);
      CHECK_EQUAL(1, countE);
      CHECK_EQUAL(1, countF);
      CHECK_EQUAL(1, countG);
      CHECK_EQUAL(1, countH);
      CHECK_EQUAL(1, countI);
      CHECK_EQUAL(1, countJ);
      CHECK_EQUAL(0, countK);
    }

    //*************************************************************************
    TEST(test_const_iterator)
    {
#ifdef TEST_GREATER_THAN
      static constexpr value_type values[]{ value_type{Key('J'), 9 }, value_type{Key('I'), 8 }, value_type{Key('H'), 7 }, value_type{Key('G'), 6 }, value_type{Key('F'), 5 },
                                            value_type{Key('E'), 4 }, value_type{Key('D'), 3 }, value_type{Key('C'), 2 }, value_type{Key('B'), 1 }, value_type{Key('A'), 0 } };
#else
      static constexpr value_type values[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                            value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };
#endif

      static constexpr Data data{ values };

      const_iterator itr = data.begin();

#ifdef TEST_GREATER_THAN
      CHECK_TRUE((value_type{Key('J'), 9 }) == *itr++);
      CHECK_TRUE((value_type{Key('I'), 8 }) == *itr++);
      CHECK_TRUE((value_type{Key('H'), 7 }) == *itr++);
      CHECK_TRUE((value_type{Key('G'), 6 }) == *itr++);
      CHECK_TRUE((value_type{Key('F'), 5 }) == *itr++);
      CHECK_TRUE((value_type{Key('E'), 4 }) == *itr++);
      CHECK_TRUE((value_type{Key('D'), 3 }) == *itr++);
      CHECK_TRUE((value_type{Key('C'), 2 }) == *itr++);
      CHECK_TRUE((value_type{Key('B'), 1 }) == *itr++);
      CHECK_TRUE((value_type{Key('A'), 0 }) == *itr++);
      CHECK_TRUE(itr == data.end());
#else
      CHECK_TRUE((value_type{Key('A'), 0 }) == *itr++);
      CHECK_TRUE((value_type{Key('B'), 1 }) == *itr++);
      CHECK_TRUE((value_type{Key('C'), 2 }) == *itr++);
      CHECK_TRUE((value_type{Key('D'), 3 }) == *itr++);
      CHECK_TRUE((value_type{Key('E'), 4 }) == *itr++);
      CHECK_TRUE((value_type{Key('F'), 5 }) == *itr++);
      CHECK_TRUE((value_type{Key('G'), 6 }) == *itr++);
      CHECK_TRUE((value_type{Key('H'), 7 }) == *itr++);
      CHECK_TRUE((value_type{Key('I'), 8 }) == *itr++);
      CHECK_TRUE((value_type{Key('J'), 9 }) == *itr++);
      CHECK_TRUE(itr == data.end());
#endif      
    }

    //*************************************************************************
    TEST(test_find)
    {
#ifdef TEST_GREATER_THAN
      static constexpr value_type values[]{ value_type{Key('J'), 9 }, value_type{Key('I'), 8 }, value_type{Key('H'), 7 }, value_type{Key('G'), 6 }, value_type{Key('F'), 5 },
                                            value_type{Key('E'), 4 }, value_type{Key('D'), 3 }, value_type{Key('C'), 2 }, value_type{Key('B'), 1 }, value_type{Key('A'), 0 } };
#else
      static constexpr value_type values[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                            value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };
#endif

      static constexpr Data data{ values };

      static constexpr const_iterator resultA = data.find(Key('A'));
      static constexpr const_iterator resultB = data.find(Key('B'));
      static constexpr const_iterator resultC = data.find(Key('C'));
      static constexpr const_iterator resultD = data.find(Key('D'));
      static constexpr const_iterator resultE = data.find(Key('E'));
      static constexpr const_iterator resultF = data.find(Key('F'));
      static constexpr const_iterator resultG = data.find(Key('G'));
      static constexpr const_iterator resultH = data.find(Key('H'));
      static constexpr const_iterator resultI = data.find(Key('I'));
      static constexpr const_iterator resultJ = data.find(Key('J'));

#ifdef TEST_GREATER_THAN
      CHECK_EQUAL(9, (std::distance(data.begin(), resultA)));
      CHECK_EQUAL(8, (std::distance(data.begin(), resultB)));
      CHECK_EQUAL(7, (std::distance(data.begin(), resultC)));
      CHECK_EQUAL(6, (std::distance(data.begin(), resultD)));
      CHECK_EQUAL(5, (std::distance(data.begin(), resultE)));
      CHECK_EQUAL(4, (std::distance(data.begin(), resultF)));
      CHECK_EQUAL(3, (std::distance(data.begin(), resultG)));
      CHECK_EQUAL(2, (std::distance(data.begin(), resultH)));
      CHECK_EQUAL(1, (std::distance(data.begin(), resultI)));
      CHECK_EQUAL(0, (std::distance(data.begin(), resultJ)));
#else
      CHECK_EQUAL(0, (std::distance(data.begin(), resultA)));
      CHECK_EQUAL(1, (std::distance(data.begin(), resultB)));
      CHECK_EQUAL(2, (std::distance(data.begin(), resultC)));
      CHECK_EQUAL(3, (std::distance(data.begin(), resultD)));
      CHECK_EQUAL(4, (std::distance(data.begin(), resultE)));
      CHECK_EQUAL(5, (std::distance(data.begin(), resultF)));
      CHECK_EQUAL(6, (std::distance(data.begin(), resultG)));
      CHECK_EQUAL(7, (std::distance(data.begin(), resultH)));
      CHECK_EQUAL(8, (std::distance(data.begin(), resultI)));
      CHECK_EQUAL(9, (std::distance(data.begin(), resultJ)));
#endif
    }

    //*************************************************************************
    TEST(test_find_using_transparent_comparator)
    {
#ifdef TEST_GREATER_THAN
      static constexpr value_type values[]{ value_type{Key('J'), 9 }, value_type{Key('I'), 8 }, value_type{Key('H'), 7 }, value_type{Key('G'), 6 }, value_type{Key('F'), 5 },
                                            value_type{Key('E'), 4 }, value_type{Key('D'), 3 }, value_type{Key('C'), 2 }, value_type{Key('B'), 1 }, value_type{Key('A'), 0 } };
#else
      static constexpr value_type values[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                            value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };
#endif

      static constexpr DataTransparentComparator data{ values };

      static constexpr const_iterator resultA = data.find('A');
      static constexpr const_iterator resultB = data.find('B');
      static constexpr const_iterator resultC = data.find('C');
      static constexpr const_iterator resultD = data.find('D');
      static constexpr const_iterator resultE = data.find('E');
      static constexpr const_iterator resultF = data.find('F');
      static constexpr const_iterator resultG = data.find('G');
      static constexpr const_iterator resultH = data.find('H');
      static constexpr const_iterator resultI = data.find('I');
      static constexpr const_iterator resultJ = data.find('J');

#ifdef TEST_GREATER_THAN
      CHECK_EQUAL(9, (std::distance(data.begin(), resultA)));
      CHECK_EQUAL(8, (std::distance(data.begin(), resultB)));
      CHECK_EQUAL(7, (std::distance(data.begin(), resultC)));
      CHECK_EQUAL(6, (std::distance(data.begin(), resultD)));
      CHECK_EQUAL(5, (std::distance(data.begin(), resultE)));
      CHECK_EQUAL(4, (std::distance(data.begin(), resultF)));
      CHECK_EQUAL(3, (std::distance(data.begin(), resultG)));
      CHECK_EQUAL(2, (std::distance(data.begin(), resultH)));
      CHECK_EQUAL(1, (std::distance(data.begin(), resultI)));
      CHECK_EQUAL(0, (std::distance(data.begin(), resultJ)));
#else
      CHECK_EQUAL(0, (std::distance(data.begin(), resultA))); 
      CHECK_EQUAL(1, (std::distance(data.begin(), resultB)));
      CHECK_EQUAL(2, (std::distance(data.begin(), resultC)));
      CHECK_EQUAL(3, (std::distance(data.begin(), resultD)));
      CHECK_EQUAL(4, (std::distance(data.begin(), resultE)));
      CHECK_EQUAL(5, (std::distance(data.begin(), resultF)));
      CHECK_EQUAL(6, (std::distance(data.begin(), resultG)));
      CHECK_EQUAL(7, (std::distance(data.begin(), resultH)));
      CHECK_EQUAL(8, (std::distance(data.begin(), resultI)));
      CHECK_EQUAL(9, (std::distance(data.begin(), resultJ)));
#endif
    }

    //*************************************************************************
    TEST(test_key_compare)
    {
      static constexpr Data data;

      static constexpr Data::key_compare compare = data.key_comp();

      static constexpr Data::key_type a(Key('A'));
      static constexpr Data::key_type b(Key('B'));

#ifdef TEST_GREATER_THAN
      CHECK(!compare(a, b));
      CHECK(compare(b, a));
#else
      CHECK(compare(a, b));
      CHECK(!compare(b, a));
#endif
    }

    //*************************************************************************
    TEST(test_key_compare_using_transparent_comparator)
    {
      static constexpr DataTransparentComparator data;
      static constexpr DataTransparentComparator::key_compare compare = data.key_comp();

      static constexpr DataTransparentComparator::key_type a(Key('A'));
      static constexpr DataTransparentComparator::key_type b(Key('B'));

#ifdef TEST_GREATER_THAN
      CHECK(!compare(a, b));
      CHECK(compare(b, a));
#else
      CHECK(compare(a, b));
      CHECK(!compare(b, a));
#endif
   }

    //*************************************************************************
    TEST(test_value_compare)
    {
      static constexpr Data data;
      static constexpr Data::value_compare compare = data.value_comp();

      static constexpr Data::value_type a(Key('A'), 0);
      static constexpr Data::value_type b(Key('B'), 1);

#ifdef TEST_GREATER_THAN
      CHECK(!compare(a, b));
      CHECK(compare(b, a));
#else
      CHECK(compare(a, b));
      CHECK(!compare(b, a));
#endif
    }

    //*************************************************************************
    TEST(test_contains)
    {
#ifdef TEST_GREATER_THAN
      static constexpr value_type values[]{ value_type{Key('J'), 9 }, value_type{Key('I'), 8 }, value_type{Key('H'), 7 }, value_type{Key('G'), 6 }, value_type{Key('F'), 5 },
                                            value_type{Key('E'), 4 }, value_type{Key('D'), 3 }, value_type{Key('C'), 2 }, value_type{Key('B'), 1 }, value_type{Key('A'), 0 } };
#else
      static constexpr value_type values[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                            value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };
#endif

      static constexpr Data data{ values };

      static constexpr bool containsA = data.contains(Key('A'));
      static constexpr bool containsB = data.contains(Key('B'));
      static constexpr bool containsC = data.contains(Key('C'));
      static constexpr bool containsD = data.contains(Key('D'));
      static constexpr bool containsE = data.contains(Key('E'));
      static constexpr bool containsF = data.contains(Key('F'));
      static constexpr bool containsG = data.contains(Key('G'));
      static constexpr bool containsH = data.contains(Key('H'));
      static constexpr bool containsI = data.contains(Key('I'));
      static constexpr bool containsJ = data.contains(Key('J'));
      static constexpr bool containsK = data.contains(Key('K'));

      CHECK_TRUE(containsA);
      CHECK_TRUE(containsB);
      CHECK_TRUE(containsC);
      CHECK_TRUE(containsD);
      CHECK_TRUE(containsE);
      CHECK_TRUE(containsF);
      CHECK_TRUE(containsG);
      CHECK_TRUE(containsH);
      CHECK_TRUE(containsI);
      CHECK_TRUE(containsJ);
      CHECK_FALSE(containsK);
    }

    //*************************************************************************
    TEST(test_contains_with_transparent_comparator)
    {
#ifdef TEST_GREATER_THAN
      static constexpr value_type values[]{ value_type{Key('J'), 9 }, value_type{Key('I'), 8 }, value_type{Key('H'), 7 }, value_type{Key('G'), 6 }, value_type{Key('F'), 5 },
                                            value_type{Key('E'), 4 }, value_type{Key('D'), 3 }, value_type{Key('C'), 2 }, value_type{Key('B'), 1 }, value_type{Key('A'), 0 } };
#else
      static constexpr value_type values[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                            value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };
#endif

      static constexpr DataTransparentComparator data{ values };

      static constexpr bool containsA = data.contains('A');
      static constexpr bool containsB = data.contains('B');
      static constexpr bool containsC = data.contains('C');
      static constexpr bool containsD = data.contains('D');
      static constexpr bool containsE = data.contains('E');
      static constexpr bool containsF = data.contains('F');
      static constexpr bool containsG = data.contains('G');
      static constexpr bool containsH = data.contains('H');
      static constexpr bool containsI = data.contains('I');
      static constexpr bool containsJ = data.contains('J');
      static constexpr bool containsK = data.contains('K');

      CHECK_TRUE(containsA);
      CHECK_TRUE(containsB);
      CHECK_TRUE(containsC);
      CHECK_TRUE(containsD);
      CHECK_TRUE(containsE);
      CHECK_TRUE(containsF);
      CHECK_TRUE(containsG);
      CHECK_TRUE(containsH);
      CHECK_TRUE(containsI);
      CHECK_TRUE(containsJ);
      CHECK_FALSE(containsK);
    }

    //*************************************************************************
    TEST(test_key_comp)
    {
      static constexpr Data data;
      static constexpr Data::key_compare compare = data.key_comp();

      static constexpr bool compareAA = compare(Key{ 'A' }, Key{ 'A' });
      static constexpr bool compareBA = compare(Key{ 'B' }, Key{ 'A' });
      static constexpr bool compareAB = compare(Key{ 'A' }, Key{ 'B' });

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
    TEST(test_key_comp_transparent_comparator)
    {
      static constexpr DataTransparentComparator data;
      static constexpr DataTransparentComparator::key_compare compare = data.key_comp();

      static constexpr bool compareAA = compare('A', 'A');
      static constexpr bool compareBA = compare('B', 'A');
      static constexpr bool compareAB = compare('A', 'B');

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
    TEST(test_value_comp)
    {
      static constexpr Data data;
      static constexpr Data::value_compare compare = data.value_comp();

      static constexpr bool compareAA1 = compare(value_type{ Key{ 'A' }, 0 }, value_type{ Key{ 'A' }, 0 });
      static constexpr bool compareAA2 = compare(value_type{ Key{ 'A' }, 0 }, Key{ 'A' });
      static constexpr bool compareAA3 = compare(Key{ 'A' }, value_type{ Key{ 'A' }, 0 });

      static constexpr bool compareBA1 = compare(value_type{ Key{ 'B' }, 0 }, value_type{ Key{ 'A' }, 0 });
      static constexpr bool compareBA2 = compare(value_type{ Key{ 'B' }, 0 }, Key{ 'A' });
      static constexpr bool compareBA3 = compare(Key{ 'B' }, value_type{ Key{ 'A' }, 0 });
          
      static constexpr bool compareAB1 = compare(value_type{ Key{ 'A' }, 0 }, value_type{ Key{ 'B' }, 0 });
      static constexpr bool compareAB2 = compare(value_type{ Key{ 'A' }, 0 }, Key{ 'B' });
      static constexpr bool compareAB3 = compare(Key{ 'A' }, value_type{ Key{ 'B' }, 0 });;

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
    TEST(test_value_comp_transparent_comparator)
    {
      static constexpr DataTransparentComparator data;
      static constexpr DataTransparentComparator::value_compare compare = data.value_comp();

      static constexpr bool compareAA1 = compare(value_type{ 'A', 0 }, value_type{ 'A', 0 });
      static constexpr bool compareAA2 = compare(value_type{ 'A', 0 }, 'A');
      static constexpr bool compareAA3 = compare('A', value_type{ 'A', 0 });

      static constexpr bool compareBA1 = compare(value_type{ 'B', 0 }, value_type{ 'A', 0 });
      static constexpr bool compareBA2 = compare(value_type{ 'B', 0 }, 'A');
      static constexpr bool compareBA3 = compare('B', value_type{ 'A', 0 });

      static constexpr bool compareAB1 = compare(value_type{ 'A', 0 }, value_type{ 'B', 0 });
      static constexpr bool compareAB2 = compare(value_type{ 'A', 0 }, 'B');
      static constexpr bool compareAB3 = compare('A', value_type{ 'B', 0 });;

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
      static constexpr value_type values1[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                             value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };

      static constexpr value_type values2[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                             value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };

      static constexpr value_type values3[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                             value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 6 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };

      static constexpr value_type values4[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                             value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 6 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 }, value_type{Key('K'), 10 } };

      static constexpr Data data1{values1};
      static constexpr Data data2{values2};
      static constexpr Data data3{values3};
      static constexpr Data data4{values4};

      static constexpr bool equal12 = (data1 == data2);
      static constexpr bool equal13 = (data1 == data3);
      static constexpr bool equal14 = (data1 == data4);

      CHECK_TRUE(equal12);
      CHECK_FALSE(equal13);
      CHECK_FALSE(equal14);
    }

    //*************************************************************************
    TEST(test_equal_with_transparent_comparator)
    {
      static constexpr value_type values1[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                             value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };

      static constexpr value_type values2[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                             value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };

      static constexpr value_type values3[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                             value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 6 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };

      static constexpr value_type values4[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                             value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 6 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 }, value_type{Key('K'), 10 } };

      static constexpr DataTransparentComparator data1{values1};
      static constexpr DataTransparentComparator data2{values2};
      static constexpr DataTransparentComparator data3{values3};
      static constexpr DataTransparentComparator data4{values4};

      static constexpr bool equal12 = (data1 == data2);
      static constexpr bool equal13 = (data1 == data3);
      static constexpr bool equal14 = (data1 == data4);

      CHECK_TRUE(equal12);
      CHECK_FALSE(equal13);
      CHECK_FALSE(equal14);
    }

    //*************************************************************************
    TEST(test_not_equal)
    {
      static constexpr value_type values1[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                             value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };

      static constexpr value_type values2[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                             value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };

      static constexpr value_type values3[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                             value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 6 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };

      static constexpr value_type values4[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                             value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 6 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 }, value_type{Key('K'), 10 } };

      static constexpr Data data1{values1};
      static constexpr Data data2{values2};
      static constexpr Data data3{values3};
      static constexpr Data data4{values4};

      static constexpr bool not_equal12 = (data1 != data2);
      static constexpr bool not_equal13 = (data1 != data3);
      static constexpr bool not_equal14 = (data1 != data4);

      CHECK_FALSE(not_equal12);
      CHECK_TRUE(not_equal13);
      CHECK_TRUE(not_equal14);
    }

    //*************************************************************************
    TEST(test_not_equal_with_transparent_comparator)
    {
      static constexpr value_type values1[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                             value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };

      static constexpr value_type values2[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                             value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };

      static constexpr value_type values3[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                             value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 6 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };

      static constexpr value_type values4[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                             value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 6 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 }, value_type{Key('K'), 10 } };

      static constexpr DataTransparentComparator data1{values1};
      static constexpr DataTransparentComparator data2{values2};
      static constexpr DataTransparentComparator data3{values3};
      static constexpr DataTransparentComparator data4{values4};

      static constexpr bool not_equal12 = (data1 != data2);
      static constexpr bool not_equal13 = (data1 != data3);
      static constexpr bool not_equal14 = (data1 != data4);

      CHECK_FALSE(not_equal12);
      CHECK_TRUE(not_equal13);
      CHECK_TRUE(not_equal14);
    }

    //*************************************************************************
    TEST(test_less_than)
    {
      static constexpr value_type values1[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('B'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                             value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };

      static constexpr value_type values2[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                             value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };

      static constexpr value_type values3[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('D'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                             value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };

      static constexpr Data data1{values1};
      static constexpr Data data2{values2};
      static constexpr Data data3{values3};

      static constexpr bool less_than12 = (data1 < data2);
      static constexpr bool less_than23 = (data2 < data3);
      static constexpr bool less_than21 = (data2 < data1);
      static constexpr bool less_than32 = (data3 < data2);

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
      static constexpr value_type values1[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('B'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                             value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };

      static constexpr value_type values2[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                             value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };

      static constexpr value_type values3[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('D'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                             value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };

      static constexpr DataTransparentComparator data1{values1};
      static constexpr DataTransparentComparator data2{values2};
      static constexpr DataTransparentComparator data3{values3};

      static constexpr bool less_than12 = (data1 < data2);
      static constexpr bool less_than23 = (data2 < data3);
      static constexpr bool less_than21 = (data2 < data1);
      static constexpr bool less_than32 = (data3 < data2);

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
      static constexpr value_type values1[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('B'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                             value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };

      static constexpr value_type values2[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                             value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };

      static constexpr value_type values3[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('D'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                             value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };

      static constexpr Data data1{values1};
      static constexpr Data data2{values2};
      static constexpr Data data3{values3};

      static constexpr bool less_than_equal12 = (data1 <= data2);
      static constexpr bool less_than_equal23 = (data2 <= data3);
      static constexpr bool less_than_equal21 = (data2 <= data1);
      static constexpr bool less_than_equal32 = (data3 <= data2);
      static constexpr bool less_than_equal11 = (data1 <= data1);

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
      static constexpr value_type values1[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('B'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                             value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };

      static constexpr value_type values2[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                             value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };

      static constexpr value_type values3[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('D'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                             value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };

      static constexpr DataTransparentComparator data1{values1};
      static constexpr DataTransparentComparator data2{values2};
      static constexpr DataTransparentComparator data3{values3};

      static constexpr bool less_than_equal12 = (data1 <= data2);
      static constexpr bool less_than_equal23 = (data2 <= data3);
      static constexpr bool less_than_equal21 = (data2 <= data1);
      static constexpr bool less_than_equal32 = (data3 <= data2);
      static constexpr bool less_than_equal11 = (data1 <= data1);

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
      static constexpr value_type values1[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('B'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                             value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };

      static constexpr value_type values2[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                             value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };

      static constexpr value_type values3[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('D'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                             value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };

      static constexpr Data data1{values1};
      static constexpr Data data2{values2};
      static constexpr Data data3{values3};

      static constexpr bool greater_than12 = (data1 > data2);
      static constexpr bool greater_than23 = (data2 > data3);
      static constexpr bool greater_than21 = (data2 > data1);
      static constexpr bool greater_than32 = (data3 > data2);

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
      static constexpr value_type values1[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('B'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                             value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };

      static constexpr value_type values2[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                             value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };

      static constexpr value_type values3[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('D'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                             value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };

      static constexpr DataTransparentComparator data1{values1};
      static constexpr DataTransparentComparator data2{values2};
      static constexpr DataTransparentComparator data3{values3};

      static constexpr bool greater_than12 = (data1 > data2);
      static constexpr bool greater_than23 = (data2 > data3);
      static constexpr bool greater_than21 = (data2 > data1);
      static constexpr bool greater_than32 = (data3 > data2);

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
      static constexpr value_type values1[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('B'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                             value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };

      static constexpr value_type values2[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                             value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };

      static constexpr value_type values3[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('D'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                             value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };

      static constexpr Data data1{values1};
      static constexpr Data data2{values2};
      static constexpr Data data3{values3};

      static constexpr bool greater_than_equal12 = (data1 >= data2);
      static constexpr bool greater_than_equal23 = (data2 >= data3);
      static constexpr bool greater_than_equal21 = (data2 >= data1);
      static constexpr bool greater_than_equal32 = (data3 >= data2);
      static constexpr bool greater_than_equal11 = (data1 >= data1);

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
      static constexpr value_type values1[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('B'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                             value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };

      static constexpr value_type values2[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('C'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                             value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };

      static constexpr value_type values3[]{ value_type{Key('A'), 0 }, value_type{Key('B'), 1 }, value_type{Key('D'), 2 }, value_type{Key('D'), 3 }, value_type{Key('E'), 4 },
                                             value_type{Key('F'), 5 }, value_type{Key('G'), 6 }, value_type{Key('H'), 7 }, value_type{Key('I'), 8 }, value_type{Key('J'), 9 } };

      static constexpr DataTransparentComparator data1{values1};
      static constexpr DataTransparentComparator data2{values2};
      static constexpr DataTransparentComparator data3{values3};

      static constexpr bool greater_than_equal12 = (data1 >= data2);
      static constexpr bool greater_than_equal23 = (data2 >= data3);
      static constexpr bool greater_than_equal21 = (data2 >= data1);
      static constexpr bool greater_than_equal32 = (data3 >= data2);
      static constexpr bool greater_than_equal11 = (data1 >= data1);

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
