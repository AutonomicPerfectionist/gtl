// ---------------------------------------------------------------------------
// Copyright (c) 2023, Gregory Popovitch - greg7mdp@gmail.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      https://www.apache.org/licenses/LICENSE-2.0
//
// Derived from Folly open source library (https://github.com/facebook/folly)
// with modifications by Gregory Popovitch.
// Folly license redistributed in the "licenses" directory of this repository
// ---------------------------------------------------------------------------

/**
 * This file is supposed to be included from within
 * vector_test.cpp. Do not use otherwise.
 */

TEST(VECTOR, clause_23_3_6_1_1) {
  VECTOR v;
  EXPECT_TRUE(v.empty());
  VECTOR::allocator_type a;
  VECTOR v1(a);
  EXPECT_TRUE(v1.empty());
}

TEST(VECTOR, clause_23_3_6_1_3) {
  auto const n = random(0U, 10000U);
  VECTOR v(n);
  EXPECT_EQ(v.size(), n);
  for (const auto& i :  v) { EXPECT_EQ(i, VECTOR::value_type()); }
}

TEST(VECTOR, clause_23_3_6_1_9) {
  // Insert with iterators
  list<VECTOR::value_type> lst;
  auto const n = random(0U, 10000U);
  FOR_EACH_RANGE (i, 0U, n) {
    lst.push_back(randomObject<VECTOR::value_type>());
  }
  VECTOR v(lst.begin(), lst.end());
  EXPECT_EQ(v.size(), lst.size());
  size_t j = 0;
  for (const auto& i : lst) {
    EXPECT_EQ(v[j], i);
    j++;
  }
}

TEST(VECTOR, clause_23_3_6_1_11) {
  // assign with iterators
  list<VECTOR::value_type> lst;
  auto const n = random(0U, 10000U);
  FOR_EACH_RANGE (i, 0U, n) {
    lst.push_back(randomObject<VECTOR::value_type>());
  }
  VECTOR v;
  v.assign(lst.begin(), lst.end());
  EXPECT_EQ(v.size(), lst.size());
  size_t j = 0;
  for (const auto& i :  lst) {
    EXPECT_EQ(v[j], i);
    j++;
  }

  // aliased assign
  v.assign(v.begin(), v.begin() + v.size() / 2);
  EXPECT_EQ(v.size(), lst.size() / 2);
  j = 0;
  for (const auto& i : lst) {
    if (j == v.size()) {
      break;
    }
    EXPECT_EQ(v[j], i);
    j++;
  }
}

TEST(VECTOR, clause_23_3_6_1_12) {
  VECTOR v;
  auto const n = random(0U, 10000U);
  auto const obj = randomObject<VECTOR::value_type>();
  v.assign(n, obj);
  EXPECT_EQ(v.size(), n);
  for (const auto& i : v) { EXPECT_EQ(i, obj); }
}

TEST(VECTOR, clause_23_3_6_2_1) {
  VECTOR v;
  auto const n = random(0U, 10000U);
  v.reserve(n);
  EXPECT_GE(v.capacity(), n);
}

TEST(VECTOR, clause_23_3_6_2_7) {
  auto const n1 = random(0U, 10000U);
  auto const n2 = random(0U, 10000U);
  auto const obj1 = randomObject<VECTOR::value_type>();
  auto const obj2 = randomObject<VECTOR::value_type>();
  VECTOR v1(n1, obj1), v2(n2, obj2);
  v1.swap(v2);
  EXPECT_EQ(v1.size(), n2);
  EXPECT_EQ(v2.size(), n1);
  for (const auto& i :  v1) { EXPECT_EQ(i, obj2); }
  for (const auto& i :  v2) { EXPECT_EQ(i, obj1); }
}

TEST(VECTOR, clause_23_3_6_2_9) {
  VECTOR v;
  auto const n1 = random(0U, 10000U);
  v.resize(n1);
  for (const auto& i :  v) { EXPECT_EQ(i, VECTOR::value_type()); }
  for (const auto& i :  v) { EXPECT_EQ(i, VECTOR::value_type()); }
}

TEST(VECTOR, clause_23_3_6_2_11) {
  VECTOR v;
  auto const n1 = random(0U, 10000U);
  auto const obj1 = randomObject<VECTOR::value_type>();
  v.resize(n1, obj1);
  for (const auto& i :  v) { EXPECT_EQ(i, obj1); }
  auto const n2 = random(0U, 10000U);
  auto const obj2 = randomObject<VECTOR::value_type>();
  v.resize(n2, obj2);
  if (n1 < n2) {
    FOR_EACH_RANGE (i, n1, n2) { EXPECT_EQ(v[i], obj2); }
  }
}

TEST(VECTOR, clause_absent_element_access) {
  VECTOR v;
  auto const n1 = random(1U, 10000U);
  auto const obj1 = randomObject<VECTOR::value_type>();
  v.resize(n1, obj1);
  auto const n = random(0U, v.size() - 1);
  EXPECT_EQ(v[n], v.at(n));
  auto const obj2 = randomObject<VECTOR::value_type>();
  v[n] = obj2;
  EXPECT_EQ(v[n], v.at(n));
  EXPECT_EQ(v[n], obj2);
  auto const obj3 = randomObject<VECTOR::value_type>();
  v.at(n) = obj3;
  EXPECT_EQ(v[n], v.at(n));
  EXPECT_EQ(v[n], obj3);
}

TEST(VECTOR, clause_23_3_6_3_1) {
  VECTOR v;
  auto const n1 = random(1U, 10000U);
  auto const obj1 = randomObject<VECTOR::value_type>();
  v.resize(n1, obj1);
  EXPECT_EQ(v.data(), &v.front());
}

TEST(VECTOR, clause_23_3_6_4_1_a) {
  VECTOR v, w;
  auto const n1 = random(1U, 10000U);
  FOR_EACH_RANGE (i, 0U, n1) {
    auto const obj1 = randomObject<VECTOR::value_type>();
    v.push_back(obj1);
    w.push_back(obj1);
  }
  auto const n2 = random(0U, n1 - 1);
  auto pos = v.begin() + n2;
  auto const obj2 = randomObject<VECTOR::value_type>();

  auto r = v.insert(pos, obj2);

  EXPECT_EQ(v.size(), w.size() + 1);
  EXPECT_EQ(r - v.begin(), n2);
  EXPECT_EQ(*r, obj2);
  FOR_EACH_RANGE (i, 0U, r - v.begin()) { EXPECT_EQ(v[i], w[i]); }
  FOR_EACH_RANGE (i, std::size_t(r - v.begin() + 1), v.size()) { EXPECT_EQ(v[i], w[i - 1]); }
}

TEST(VECTOR, clause_23_3_6_4_1_c) {
  // This test only works for gtl::vector
  gtl::vector<VECTOR::value_type> v, w;
  auto const n1 = random(1U, 10000U);
  FOR_EACH_RANGE (i, 0U, n1) {
    auto const obj1 = randomObject<VECTOR::value_type>();
    v.push_back(obj1);
    w.push_back(obj1);
  }
  auto const n2 = random(0U, n1 - 1);
  auto pos = v.begin() + n2;
  auto const obj2 = randomObject<VECTOR::value_type>();
  auto const n3 = random(0U, 10000U);

  auto r = v.insert(pos, n3, obj2);

  EXPECT_EQ(v.size(), w.size() + n3);
  EXPECT_EQ(r - v.begin(), n2);
  FOR_EACH_RANGE (i, 0U, r - v.begin()) { EXPECT_EQ(v[i], w[i]); }
  FOR_EACH_RANGE (i, r - v.begin(), r - v.begin() + n3) {
    EXPECT_EQ(v[i], obj2);
  }
  FOR_EACH_RANGE (i, std::size_t(r - v.begin() + n3), v.size()) {
    EXPECT_EQ(v[i], w[i - n3]);
  }
}

TEST(VECTOR, clause_23_3_6_4_1_d) {
  VECTOR v, w;
  auto const n1 = random(0U, 10000U);
  FOR_EACH_RANGE (i, 0U, n1) {
    auto const obj1 = randomObject<VECTOR::value_type>();
    v.push_back(obj1);
    w.push_back(obj1);
  }
  EXPECT_EQ(v.size(), n1);

  auto const obj2 = randomObject<VECTOR::value_type>();
  v.push_back(obj2);
  EXPECT_EQ(v.back(), obj2);
  EXPECT_EQ(v.size(), w.size() + 1);

  FOR_EACH_RANGE (i, 0U, w.size()) { EXPECT_EQ(v[i], w[i]); }
}

TEST(VECTOR, clause_23_3_6_4_3) {
  VECTOR v, w;
  auto const n1 = random(1U, 10000U);
  FOR_EACH_RANGE (i, 0U, n1) {
    auto const obj1 = randomObject<VECTOR::value_type>();
    v.push_back(obj1);
    w.push_back(obj1);
  }
  EXPECT_EQ(v.size(), n1);

  auto const n2 = random(0U, n1 - 1);
  auto it = v.erase(v.begin() + n2);
  EXPECT_EQ(v.size() + 1, w.size());

  FOR_EACH_RANGE (i, 0U, it - v.begin()) { EXPECT_EQ(v[i], w[i]); }

  FOR_EACH_RANGE (i, std::size_t(it - v.begin()), v.size()) { EXPECT_EQ(v[i], w[i + 1]); }
}

TEST(VECTOR, clause_23_3_6_4_4) {
  VECTOR v, w;
  auto const n1 = random(1U, 10000U);
  FOR_EACH_RANGE (i, 0U, n1) {
    auto const obj1 = randomObject<VECTOR::value_type>();
    v.push_back(obj1);
    w.push_back(obj1);
  }
  EXPECT_EQ(v.size(), n1);

  auto const n2 = random(0U, n1 - 1);
  auto const n3 = random(n2, n1 - 1);
  auto it = v.erase(v.begin() + n2, v.begin() + n3);
  EXPECT_EQ(v.size() + (n3 - n2), w.size());

  FOR_EACH_RANGE (i, 0U, it - v.begin()) { EXPECT_EQ(v[i], w[i]); }

  FOR_EACH_RANGE (i, std::size_t(it - v.begin()), v.size()) {
    EXPECT_EQ(v[i], w[i + (n3 - n2)]);
  }
}

TEST(VECTOR, clause_23_3_6_4_clear) {
  VECTOR v;
  v.clear();
  EXPECT_TRUE(v.empty());
  v.resize(random(0U, 10000U));
  auto c = v.capacity();
  v.clear();
  EXPECT_TRUE(v.empty());
  EXPECT_EQ(v.capacity(), c);
}
