#pragma once

#include <type_traits>

template <typename T>
struct EnableBitMasksOperators : public std::false_type {};

template <typename Enum, typename = std::enable_if_t<EnableBitMasksOperators<Enum>::value>>
Enum operator|(Enum lhs, Enum rhs)
{
    using UnderlyingType = typename std::underlying_type_t<Enum>;

    return static_cast<Enum>(
        static_cast<UnderlyingType>(lhs) |
        static_cast<UnderlyingType>(rhs));
}

template <typename Enum, typename = std::enable_if_t<EnableBitMasksOperators<Enum>::value>>
Enum operator|=(Enum lhs, Enum rhs)
{
    return lhs | rhs;
}

template <typename Enum, typename = std::enable_if_t<EnableBitMasksOperators<Enum>::value>>
Enum operator&(Enum lhs, Enum rhs)
{
    using UnderlyingType = typename std::underlying_type_t<Enum>;

    return static_cast<Enum>(
        static_cast<UnderlyingType>(lhs) &
        static_cast<UnderlyingType>(rhs));
}

template <typename Enum, typename = std::enable_if_t<EnableBitMasksOperators<Enum>::value>>
Enum operator&=(Enum lhs, Enum rhs)
{
    return lhs & rhs;
}

template <typename Enum, typename = std::enable_if_t<EnableBitMasksOperators<Enum>::value>>
Enum operator^(Enum lhs, Enum rhs)
{
    using UnderlyingType = typename std::underlying_type_t<Enum>;

    return static_cast<Enum>(
        static_cast<UnderlyingType>(lhs) ^
        static_cast<UnderlyingType>(rhs));
}

template <typename Enum, typename = std::enable_if_t<EnableBitMasksOperators<Enum>::value>>
Enum operator~(Enum rhs)
{
    using UnderlyingType = typename std::underlying_type_t<Enum>;

    return static_cast<Enum>(~static_cast<UnderlyingType>(rhs));
}

template <typename Enum, typename = std::enable_if_t<std::is_enum_v<Enum>>>
constexpr bool check(Enum flags, Enum e)
{
    return (flags & e) == e;
}

#define ENABLE_BITMASK_OPERATORS(TYPE) \
    template<> struct EnableBitMasksOperators<TYPE> : public std::true_type {}