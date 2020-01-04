#ifndef KIZUNA_ITERATOR_HPP
#define KIZUNA_ITERATOR_HPP

#include <iterator>
#include <kizuna/operator.hpp>

namespace kizuna
{
  template <typename derived, typename T, typename Reference, typename Difference>
  struct iterator_base
  {
    using difference_type = Difference;
    using value_type = T;
    using pointer = decltype (& std::declval <Reference> ());
    using reference = Reference;

    constexpr auto operator -> () const noexcept
    {
      return & static_cast <const derived *> (this)->operator * ();
    }
  };

  template <typename derived, typename T, typename Reference, typename Difference>
  struct iterator_base <derived, const T, Reference, Difference>
  {
    using difference_type = Difference;
    using value_type = T;
    using pointer = decltype (& std::declval <Reference> ());
    using reference = Reference;

    constexpr auto operator -> () const noexcept
    {
      return & static_cast <const derived *> (this)->operator * ();
    }
  };


  // helper of creating input_iterator.
  // requires:
  //   auto operator * () const -> const T &
  //   auto operator ++ () -> derived &
  //   friend auto operator == (const derived &, const derived &) -> bool
  template <typename derived, typename T, typename Reference = const T &, typename Difference = std::ptrdiff_t>
  struct input_iterator
    : iterator_base <derived, T, Reference, Difference>
    , private incrementable <derived>
    , private equality_compareable <derived>
  {
    using iterator_category = std::input_iterator_tag;
  };


  // helper of creating output_iterator.
  // requires:
  //   auto operator * () const -> T &
  //   auto operator ++ () -> derived &
  template <typename derived, typename T, typename Reference = T &, typename Difference = std::ptrdiff_t>
  struct output_iterator
    : iterator_base <derived, T, Reference, Difference>
    , private incrementable <derived>
  {
    using iterator_category = std::output_iterator_tag;
  };


  // helper of creating forward_iterator.
  // requires:
  //   auto operator * () const -> T &
  //   auto operator ++ () -> derived &
  //   friend auto operator == (const derived &, const derived &) -> bool
  template <typename derived, typename T, typename Reference = T &, typename Difference = std::ptrdiff_t>
  struct forward_iterator
    : iterator_base <derived, T, Reference, Difference>
    , private incrementable <derived>
    , private equality_compareable <derived>
  {
    using iterator_category = std::forward_iterator_tag;
  };


  // helper of creating bidirectional_iterator.
  // requires:
  //   auto operator * () const -> T &
  //   auto operator ++ () -> derived &
  //   auto operator -- () -> derived &
  //   friend auto operator == (const derived &, const derived &) -> bool
  template <typename derived, typename T, typename Reference = T &, typename Difference = std::ptrdiff_t>
  struct bidirectional_iterator
    : iterator_base <derived, T, Reference, Difference>
    , private incrementable <derived>
    , private decrementable <derived>
    , private equality_compareable <derived>
  {
    using iterator_category = std::bidirectional_iterator_tag;
  };


  // helper of creating random_access_iterator.
  // requires:
  //   auto operator * () const -> T &
  //   auto operator += (ptrdiff_t) -> derived &
  //   friend auto operator < (const derived &, const derived &) -> bool
  //   friend auto operator - (const derived &, const derived &) -> difference_type
  template <typename derived, typename T, typename Reference = T &, typename Difference = std::ptrdiff_t>
  struct random_access_iterator
    : iterator_base <derived, T, Reference, Difference>
    , private incrementable <derived>
    , private decrementable <derived>
    , private addable <derived>
    , private substractable <derived>
    , private compareable <derived>
  {
    using difference_type = typename iterator_base <derived, T, Reference, Difference>::difference_type;
    using iterator_category = std::random_access_iterator_tag;

    constexpr auto operator [] (difference_type d) const noexcept -> decltype (auto)
    {
      return * (* static_cast <const derived *> (this) + d);
    }

    constexpr auto operator ++ () noexcept -> decltype (auto)
    {
      return * static_cast <derived *> (this) += 1;
    }

    constexpr auto operator -- () noexcept -> decltype (auto)
    {
      return * static_cast <derived *> (this) += -1;
    }

    constexpr auto operator -= (difference_type d) noexcept -> decltype (auto)
    {
      return * static_cast <derived *> (this) += - d;
    }

    friend constexpr auto operator == (const derived & left, const derived & right)
    {
      return ! (left < right || right < left);
    }
  };


  // example
  template <typename T>
  struct pointer_iterator
    : random_access_iterator <pointer_iterator <T>, T>
  {
  private:
    T * ptr;

    using super = random_access_iterator <pointer_iterator <T>, T>;

  public:
    constexpr pointer_iterator () noexcept : ptr {nullptr} {}
    explicit constexpr pointer_iterator (T * p) noexcept : ptr {p} {}

    constexpr auto operator * () const noexcept -> decltype (auto)
    {
      return * ptr;
    }

    constexpr auto operator += (typename super::difference_type d) noexcept -> decltype (auto)
    {
      ptr += d;
      return * this;
    }

    friend constexpr auto operator < (const pointer_iterator & left, const pointer_iterator & right) noexcept
    {
      return left.ptr < right.ptr;
    }

    friend constexpr auto operator - (const pointer_iterator & left, const pointer_iterator & right) noexcept
    {
      return left.ptr - right.ptr;
    }
  };
} // namespace kizuna

#endif // KIZUNA_ITERATOR_HPP
