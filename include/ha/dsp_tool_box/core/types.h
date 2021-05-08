// Copyright(c) 2021 Hansen Audio.

#pragma once

#include <cstdint>
#include <type_traits>

namespace ha {
namespace dtb {
//------------------------------------------------------------------------
using i32     = std::int32_t const;
using mut_i32 = std::remove_const<i32>::type;

using real     = float const;
using mut_real = std::remove_const<real>::type;

//------------------------------------------------------------------------
} // namespace dtb
} // namespace ha