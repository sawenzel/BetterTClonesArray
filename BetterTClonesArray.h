#include "TClonesArray.h"
#include <type_traits>


// A smart and type-safe wrapper around TClonesArray
// In some sense, it has some similarities to smart pointers ...
template <typename T> class BetterTClonesArray /*: public TClonesArray */ {
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
      do {
        mIndex++;
      } while ( mArray->UncheckedAt(mIndex) == nullptr && mIndex < mArray->GetLast()-1 );
      return *this;
    }

    // dereferencing the iterator:
    pointer operator*() const {
      return static_cast<T>(mArray->UncheckedAt(mIndex));
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
  BetterTClonesArray(TClonesArray *ptr) : mBare(ptr) {}

  // assignment operator from a TClonesArray pointer
  BetterTClonesArray operator=(TClonesArray *ptr) { mBare = ptr; }
  BetterTClonesArray(BetterTClonesArray const &other) : mBare(other.mBare) {}

  iterator begin() const { return iterator(0, mBare); }
  iterator end() const { return iterator(mBare->GetLast() + 1, mBare); }

  // A type safe operator[] to read elements
  // We could make a fast version of this (without checks realocation etc.)
  // by directly accessing the internal data of mBare
  T operator[](size_t i) const {
    return static_cast<T>(mBare->UncheckedAt(i));
  }

  // we delete this function: It is evil !!
  T& operator[](size_t) = delete;


  // Offer only
  // ConstructedAt interface


  // Do we want an l-value operator[]

  // we can provide here the better and fast sorting:
  // void Sort();

  // access to the raw container
  TClonesArray *raw() { return mBare; }
  TClonesArray const *raw() const { return mBare; }

  // should we allow implicit castings to TClonesArray (or prefer raw())??
  operator TClonesArray*() const { return mBare; }

  // Ideas: offer reasonable push_back, emplace_back, size functionality

private:
  TClonesArray *mBare;
};
