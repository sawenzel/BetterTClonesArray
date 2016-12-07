#include "TClonesArray.h"
#include <type_traits>

template <typename T> class BetterTClonesArray : public TClonesArray {
    // need to impose that T is a pointer type (as is the case for TClonesArray)
    static_assert(std::is_pointer<T>::value,
                "template argument of BetterTClonesArray needs to be pointer");

  // internal iterator class
  // http://stackoverflow.com/questions/7758580/writing-your-own-stl-container/7759622#7759622
  class iterator {
  public:
    typedef T value_type;
    typedef T &reference;
    typedef T pointer; /* this is because T is itself a pointer */
    iterator(size_t index, TClonesArray *array)
        : mIndex(index), mArray(array) {}
    iterator(const iterator &other)
        : mIndex(other.mIndex), mArray(other.mArray) {}
    ~iterator(){/* nothing to be done */};

    iterator &operator=(const iterator &other) {
      mIndex = other.mIndex;
      mArray = other.mArray;
    }
    bool operator==(const iterator &other) const {
      return mArray == other.mArray && mIndex == other.mIndex;
    }
    bool operator!=(const iterator &other) const {
      return mArray != other.mArray || mIndex != other.mIndex;
    }

    // we could make this clever by actually jumping to the next VALID object
    // for the moment just going to next location
    iterator &operator++() {
      mIndex++;
      return *this;
    }

    // dereferencing the iterator:
    pointer operator*() const {
      return static_cast<T>(mArray->operator[](mIndex));
    }
    // reference operator->() const;

  private:
    // the state of the iterator
    size_t mIndex;
    TClonesArray *mArray;
  };

public:
  // implicit constructor
  BetterTClonesArray(TClonesArray &p) : mBare(&p) {}

  iterator begin() const { return iterator(0, mBare); }
  iterator end() const { return iterator(mBare->LastIndex() + 1, mBare); }

  // A type safe operator[]
  // We could make a fast version of this (without checks realocation etc.)
  // by directly accessing the internal data of mBare
  T operator[](size_t i) const { return static_cast<T>(mBare->operator[](i)); }

  // we can provide here the better and fast sorting:

private:
  TClonesArray *mBare;
};
