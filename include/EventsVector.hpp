#ifndef EVENTS_VECTOR_H_
# define EVENTS_VECTOR_H_
# include <vector>


namespace irc {


template<class T>
class EventsVector {

public:

  EventsVector() {}

  EventsVector(const EventsVector& other)
  : _events(other._events) {}

  template<class InputIt>
  EventsVector(InputIt _first, InputIt _last)
  : _events(_first, _last) {}

  ~EventsVector() {}


  EventsVector& operator=(const EventsVector& other) {
    if (this != &other)
      _events = other._events;
    return *this;
  }


  template<class InputIt>
  void assign(InputIt _first, InputIt _last)
  { _events.assign(_first, _last); }


  typename std::vector<T>::iterator
  begin() { return _events.begin(); }

  typename std::vector<T>::iterator
  end() { return _events.end(); }


  T& operator[](size_t pos) { return _events[pos]; }


  void push_back(const T& value)
  { _events.push_back(value); }

  void reserve(size_t new_cap)
  { _events.reserve(new_cap); }


  void shrink_to_fit() {
    std::vector<T> _new(_events.begin(), _events.end());
    _events.swap(_new);
  }


  size_t size() const
  { return _events.size(); }


  T* data()
  { return _events.data(); }

private:

  std::vector<T> _events;

};


} // namespace irc


#endif // EVENTS_VECTOR_H_
