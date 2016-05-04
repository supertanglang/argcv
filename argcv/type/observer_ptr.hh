// based on: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n3840.pdf
// example: http://stackoverflow.com/questions/31862412/use-of-observer-ptr
// 20.8.x, class template observer_ptr
#ifndef ARGCV_TYPE_OBSERVER_PTR_HH
#define ARGCV_TYPE_OBSERVER_PTR_HH

namespace argcv {
namespace type {
// based on: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n3840.pdf
// example: http://stackoverflow.com/questions/31862412/use-of-observer-ptr
// 20.8.x, class template observer_ptr
template <class W>
class observer_ptr;

template <class W, size_t N>
class observer_ptr<E[N]>;

template <class W>
void swap(const observer_ptr<W>&, const observer_ptr<W>&) noexcept;

template <class W>
observer_ptr<W> make_observer(W*) noexcept;

// (in)equality operators
template <class W1, class W2>
bool operator==(const observer_ptr<W1>&, const observer_ptr<W2>&);

template <class W1, class W2>
bool operator!=(const observer_ptr<W1>&, const observer_ptr<W2>&);

template <class W>
bool operator==(const observer_ptr<W>&, nullptr_t) noexcept;

template <class W>
bool operator!=(const observer_ptr<W>&, nullptr_t) noexcept;

template <class W>
bool operator==(nullptr_t, const observer_ptr<W>&) noexcept;

template <class W>
bool operator!=(nullptr_t, const observer_ptr<W>&) noexcept;

// ordering operators
template <class W1, class W2>
bool operator<(const observer_ptr<W1>&, const observer_ptr<W2>&);

template <class W1, class W2>
bool operator>(const observer_ptr<W1>&, const observer_ptr<W2>&);

template <class W1, class W2>
bool operator<=(const observer_ptr<W1>&, const observer_ptr<W2>&);

template <class W1, class W2>
bool operator>=(const observer_ptr<W1>&, const observer_ptr<W2>&);

// arithmetic operators
template <class W, size_t N>
observer_ptr<W> operator+(ptrdiff_t, const observer_ptr<W>[N]&);

// hash support
template <class T>
struct hash<observer_ptr<T>>;
}
}  // namespace argcv::type::

#endif  // ARGCV_TYPE_OBSERVER_PTR_HH