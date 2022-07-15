#ifndef EVENTS_VECTOR_H_
# define EVENTS_VECTOR_H_
# include <sys/types.h>
# include <sys/event.h>
# include <sys/time.h>
# include <new>

namespace irc {



class EventsVector {

private:

  typedef struct kevent  value_type;
  typedef value_type*    pointer;
  typedef value_type&    reference;
  typedef size_t         size_type;

public:

  EventsVector()
  : _events(NULL), _size(0), _capacity(0) {}

  EventsVector(size_type size)
  : _events(new value_type[size]), _size(size), _capacity(size) {}

  EventsVector(const EventsVector& other)
  : _events(new value_type[other._size]),
    _size(other._size), _capacity(other._size) {
    for (size_type i = 0; i < _size; ++i)
      _events[i] = other._events[i];
  }

  ~EventsVector() { delete [] _events; }

  EventsVector& operator=(const EventsVector& other) {
    if (this != &other) {
      delete [] _events;
      _events = new value_type[other._size];
      _size = other._size;
      _capacity = other._size;
      for (size_type i = 0; i < _size; ++i)
        _events[i] = other._events[i];
    }
    return *this;
  }


  void clear() {
    delete [] _events;
    _events = NULL;
    _size = 0;
    _capacity = 0;
  }

  void reserve(size_type new_capacity) {
    pointer tmp = new value_type[new_capacity];
    _capacity = new_capacity;
    for (size_type i = 0; i < _size; ++i)
      tmp[i] = _events[i];
    delete [] _events;
    _events = tmp;
  }

  reference operator[](size_type pos) { return _events[pos]; }
  size_type size() const { return _size; }
  size_type capacity() const { return _capacity; }
  pointer data() { return _events; }

private:

  pointer   _events;
  size_type _size;
  size_type _capacity;

};


} // namespace irc


#endif // EVENTS_VECTOR_H_
