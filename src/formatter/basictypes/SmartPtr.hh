
#ifndef __SmartPtr_hh__
#define __SmartPtr_hh__

#include <cassert>

template <class P>
class SmartPtr
{
public:
  SmartPtr(P* p = 0) : ptr(p) { if (ptr) ptr->ref(); }
  SmartPtr(const SmartPtr& p) : ptr(p.ptr) { if (ptr) ptr->ref(); }
  ~SmartPtr() { if (ptr) ptr->unref(); }

  P* operator->() const { assert(ptr); return ptr; }
  SmartPtr& operator=(const SmartPtr& p)
  { 
    if (ptr == p.ptr) return *this;
    if (p.ptr) p.ptr->ref();
    if (ptr) ptr->unref();
    ptr = p.ptr;
    return *this;
  }

  operator P*() const { return ptr; }
  template <class Q> friend SmartPtr<Q> smart_cast(const SmartPtr& p) { return SmartPtr<Q>(dynamic_cast<Q*>(p.ptr)); }  
  template <class Q> friend bool is_a(const SmartPtr& p) { return dynamic_cast<Q*>(p.ptr) != 0; }
  template <class Q> operator SmartPtr<Q>() const { return SmartPtr<Q>(ptr); }

private:
  P* ptr;
};

#endif // __SmartPtr_hh__
