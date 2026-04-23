# vector — Custom C++ Implementation

A header-only implementation of `std::vector<T>` and its `vector<bool>` specialization, written in modern C++23.

---

## Files

| File | Description |
|------|-------------|
| `vector.hpp` | Header-only implementation of `vector<T>` and `vector<bool>` |

---

## Features

### `vector<T>`

| Method | Description |
|--------|-------------|
| `push_back(val)` | Adds element to the end |
| `pop_back()` | Removes last element |
| `insert(pos, val)` | Inserts single value at position |
| `insert(pos, count, val)` | Inserts `count` copies at position |
| `insert(pos, {a, b, ...})` | Inserts from initializer list |
| `erase(pos)` | Removes element at position |
| `erase(first, last)` | Removes range `[first, last)` |
| `resize(count, val)` | Resizes vector, fills new elements with `val` |
| `swap(other)` | Swaps contents with another vector |
| `clear()` | Sets size to 0 |
| `size()` | Returns element count |
| `capacity()` | Returns allocated capacity |
| `empty()` | Returns true if empty |
| `print()` | Prints all elements to stdout |

### `vector<bool>` — Bit-Packed Specialization

Full template specialization that stores each `bool` as a **single bit**, using `unsigned char` as the underlying storage unit (8 bits per byte).

| Method | Description |
|--------|-------------|
| `push_back(val)` | Appends a bit |
| `operator[](i)` | Returns a `reference` proxy (read/write) or `bool` (read-only) |
| `size()` / `capacity()` | Returns bit count |
| `print()` | Prints `0`/`1` for each bit |

#### `reference` Proxy

Since individual bits have no memory address, `operator[]` returns a `reference` object instead of `bool&`:

```cpp
vector<bool> v;
v.push_back(true);
v[0] = false;        // write via proxy
bool b = v[0];       // read — implicit bool conversion
```

---

## Memory Model

### `vector<T>`
```
[ T | T | T | T | ... ]   ← unique_ptr<T[]>
  ^               ^
  size            capacity
```

Capacity doubles on every reallocation (`1 → 2 → 4 → 8 → ...`).

### `vector<bool>`
```
bit index:  0  1  2  3  4  5  6  7  | 8  9 ...
           [  byte 0               ] | [  byte 1  ...  ]
```

Initial capacity is **64 bits**. Doubles on overflow.

---

## Usage

```cpp
#include "vector.hpp"

int main()
{
    // Generic vector
    vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.insert(1, 3, 99);   // insert 3 copies of 99 at index 1
    v.print();             // 1 99 99 99 2

    // Bool specialization
    vector<bool> flags;
    flags.push_back(true);
    flags.push_back(false);
    flags.push_back(true);
    flags[1] = true;
    flags.print();         // 111
}
```

---

## Requirements

| | |
|-|-|
| **Standard** | C++23 |
| **Headers used** | `<iostream>`, `<memory>`, `<utility>`, `<cstring>`, `<print>`, `<initializer_list>` |
| **Dependencies** | None |

> `std::print` / `std::println` require C++23. Compile with `-std=c++23`.

---

## Limitations

- `vector<T>` does not yet provide iterators (`begin`/`end`), copy/move constructors, or `operator[]`
- `vector<bool>` does not provide range constructor or `erase`/`insert`
- No allocator support

---

## License

MIT
