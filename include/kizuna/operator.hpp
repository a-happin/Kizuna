#ifndef KIZUNA_OPERATOR_HPP
#define KIZUNA_OPERATOR_HPP

#include <utility>
#include <type_traits>

namespace kizuna
{

  // ADL firewall
  namespace equality_compareable_
  {
    template <typename derived>
    struct equality_compareable
    {
      // requires:
      //   friend auto operator == (const derived &, const derived &)

      friend constexpr auto operator != (const derived & x, const derived & y) noexcept
      {
        return ! (x == y);
      }
    };

  } // namespace equality_compareable_
  using equality_compareable_::equality_compareable;


  // ADL firewall
  namespace compareable_
  {
    template <typename derived>
    struct compareable : equality_compareable <derived>
    {
      // requires:
      //   friend auto operator < (const derived &, const derived &)

      friend constexpr auto operator > (const derived & x, const derived & y) noexcept
      {
        return y < x;
      }
      friend constexpr auto operator >= (const derived & x, const derived & y) noexcept
      {
        return not (x < y);
      }
      friend constexpr auto operator <= (const derived & x, const derived & y) noexcept
      {
        return not (y < x);
      }
    };
  } // namespace compareable_
  using compareable_::compareable;


  // ADL filewall
  namespace incrementable_
  {
    template <typename derived>
    struct incrementable
    {
      // requires:
      //   auto operator ++ ()

      // メンバ関数にするべきだが、friend関数だと記述量が減り、private継承できる
      friend constexpr auto operator ++ (derived & self, int)
      {
        auto tmp = self;
        ++ self;
        return tmp;
      }
    };
  } // namespace incrementable_
  using incrementable_::incrementable;


  // ADL filewall
  namespace decrementable_
  {
    template <typename derived>
    struct decrementable
    {
      // requires:
      //   auto operator -- ()

      // メンバ関数にするべきだが、friend関数だと記述量が減り、private継承できる
      friend constexpr auto operator -- (derived & self, int)
      {
        auto tmp = self;
        -- self;
        return tmp;
      }
    };
  } // namespace decrementable_
  using decrementable_::decrementable;


  // ADL firewall
  namespace addable_
  {
    template <typename derived>
    struct addable
    {
      // requires:
      //   auto operator += (U)

      template <typename U, typename = std::void_t <decltype (std::declval <derived> () += std::declval <U &&> ())>>
      friend constexpr auto operator + (const derived & x, U && y)
      {
        auto tmp = x;
        tmp += std::forward <U> (y);
        return tmp;
      }
    };
  } // namespace addable_
  using addable_::addable;


  // ADL firewall
  namespace substractable_
  {
    template <typename derived>
    struct substractable
    {
      // requires:
      //   auto operator -= (U)

      template <typename U, typename = std::void_t <decltype (std::declval <derived> () -= std::declval <U &&> ())>>
      friend constexpr auto operator - (const derived & x, U && y)
      {
        auto tmp = x;
        tmp -= std::forward <U> (y);
        return tmp;
      }
    };
  } // namespace substractable_
  using substractable_::substractable;


  // ADL firewall
  namespace multipliable_
  {
    template <typename derived>
    struct multipliable
    {
      // requires:
      //   auto operator *= (U)

      template <typename U, typename = std::void_t <decltype (std::declval <derived> () *= std::declval <U &&> ())>>
      friend constexpr auto operator * (const derived & x, U && y)
      {
        auto tmp = x;
        tmp *= std::forward <U> (y);
        return tmp;
      }
    };
  } // namespace multipliable_
  using multipliable_::multipliable;


  // ADL firewall
  namespace dividable_
  {
    template <typename derived>
    struct dividable
    {
      // requires:
      //   auto operator /= (U)

      template <typename U, typename = std::void_t <decltype (std::declval <derived> () /= std::declval <U &&> ())>>
      friend constexpr auto operator / (const derived & x, U && y)
      {
        auto tmp = x;
        tmp /= std::forward <U> (y);
        return tmp;
      }
    };
  } // namespace dividable_
  using dividable_::dividable;


  // ADL firewall
  namespace modable_
  {
    template <typename derived>
    struct modable
    {
      // requires:
      //   auto operator %= (U)

      template <typename U, typename = std::void_t <decltype (std::declval <derived> () %= std::declval <U &&> ())>>
      friend constexpr auto operator % (const derived & x, U && y)
      {
        auto tmp = x;
        tmp %= std::forward <U> (y);
        return tmp;
      }
    };
  } // namespace modable_
  using modable_::modable;


  // ADL firewall
  namespace orable_
  {
    template <typename derived>
    struct orable
    {
      // requires:
      //   auto operator |= (U)

      template <typename U, typename = std::void_t <decltype (std::declval <derived> () |= std::declval <U &&> ())>>
      friend constexpr auto operator | (const derived & x, U && y)
      {
        auto tmp = x;
        tmp |= std::forward <U> (y);
        return tmp;
      }
    };
  } // namespace orable_
  using orable_::orable;


  // ADL firewall
  namespace andable_
  {
    template <typename derived>
    struct andable
    {
      // requires:
      //   auto operator &= (U)

      template <typename U, typename = std::void_t <decltype (std::declval <derived> () &= std::declval <U &&> ())>>
      friend constexpr auto operator & (const derived & x, U && y)
      {
        auto tmp = x;
        tmp &= std::forward <U> (y);
        return tmp;
      }
    };
  } // namespace andable_
  using andable_::andable;


  // ADL firewall
  namespace xorable_
  {
    template <typename derived>
    struct xorable
    {
      // requires:
      //   auto operator ^= (U)

      template <typename U, typename = std::void_t <decltype (std::declval <derived> () ^= std::declval <U &&> ())>>
      friend constexpr auto operator ^ (const derived & x, U && y)
      {
        auto tmp = x;
        tmp ^= std::forward <U> (y);
        return tmp;
      }
    };
  } // namespace xorable_
  using xorable_::xorable;


  // ADL firewall
  namespace left_shiftable_
  {
    template <typename derived>
    struct left_shiftable
    {
      // requires:
      //   auto operator <<= (U)

      template <typename U, typename = std::void_t <decltype (std::declval <derived> () <<= std::declval <U &&> ())>>
      friend constexpr auto operator << (const derived & x, U && y)
      {
        auto tmp = x;
        tmp <<= std::forward <U> (y);
        return tmp;
      }
    };
  } // namespace left_shiftable_
  using left_shiftable_::left_shiftable;


  // ADL firewall
  namespace right_shiftable_
  {
    template <typename derived>
    struct right_shiftable
    {
      // requires:
      //   auto operator >>= (U)

      template <typename U, typename = std::void_t <decltype (std::declval <derived> () >>= std::declval <U &&> ())>>
      friend constexpr auto operator >> (const derived & x, U && y)
      {
        auto tmp = x;
        tmp >>= std::forward <U> (y);
        return tmp;
      }
    };
  } // namespace right_shiftable_
  using right_shiftable_::right_shiftable;


  // ADL firewall
  namespace notable_
  {
    template <typename derived>
    struct notable
    {
      // requires:
      //   auto operator bool ()

      friend constexpr auto operator ! (const derived & x) noexcept
      {
        return ! static_cast <bool> (x);
      }
    };
  } // namespace notable_
  using notable_::notable;


} // namespace kizuna

#endif // KIZUNA_OPERATOR_HPP
