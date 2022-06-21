#ifndef EVENTS_VECTOR_H_
# define EVENTS_VECTOR_H_
# include <vector>


namespace irc {


template<class T>
class EventsVector {

private:

  typedef typename std::vector<T>              internal_vector;
  typedef typename internal_vector::iterator   iterator;
  typedef typename internal_vector::pointer    pointer;
  typedef typename internal_vector::reference  reference;
  typedef typename internal_vector::size_type  size_type;

public:

  EventsVector() {}

  EventsVector(const EventsVector& other)
  : _events(other._events) {}

  template<class InputIterator>
  EventsVector(InputIterator _first, InputIterator _last)
  : _events(_first, _last) {}

  ~EventsVector() {}


  EventsVector& operator=(const EventsVector& other) {
    if (this != &other)
      _events = other._events;
    return *this;
  }


  template<class InputIterator>
  void assign(InputIterator _first, InputIterator _last)
  { _events.assign(_first, _last); }


  iterator begin() { return _events.begin(); }
  iterator end() { return _events.end(); }


  reference operator[](size_type pos) { return _events[pos]; }


  void push_back(const reference value) { _events.push_back(value); }

  void reserve(size_type new_cap) { _events.reserve(new_cap); }


  void shrink_to_fit() {
    internal_vector _new(_events.begin(), _events.end());
    _events.swap(_new);
  }


  size_type size() const { return _events.size(); }

  void clear() { _events.clear(); }

  pointer data() { return _events.data(); }

private:

  internal_vector _events;

};


} // namespace irc


#endif // EVENTS_VECTOR_H_
