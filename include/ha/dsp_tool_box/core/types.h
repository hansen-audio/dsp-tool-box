// Copyright(c) 2021 Hansen Audio.

#pragma once

#include <cstdint>
#include <type_traits>

namespace ha {
namespace dtb {
//------------------------------------------------------------------------
using i32 = std::int32_t;

using real     = float const;
using mut_real = std::remove_const<real>::type;

//------------------------------------------------------------------------
} // namespace dtb
} // namespace ha