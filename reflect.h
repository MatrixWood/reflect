#ifndef __REFLECT_H__
#define __REFLECT_H__

#include <iostream>
#include <map>
#include <string>
#include <type_traits>

namespace canftin_reflect {
  struct default_reflect_tag {};

  template <class T,
            typename REFLECT_TAG = default_reflect_tag>
  class base_factory {
  public:
    virtual T* create() const = 0;
  };

  template <class Base, class T,
            typename REFLECT_TAG = default_reflect_tag>
  class factory : public base_factory<Base> {
  public:
    virtual T* create() const {
      return new T();
    }
  };

  template <class Base,
            typename REFLECT_TAG = default_reflect_tag>
  std::map<std::string, base_factory<Base, REFLECT_TAG>* >&
  __global_factory_map() {
    static std::map<std::string,
                    base_factory<Base, REFLECT_TAG>* > __the_map;
    return __the_map;
  }

  template <class Base,
            class T,
            typename class_name_wp,
            typename REFLECT_TAG = default_reflect_tag>
  class __reflect_regist_class {
  public:
    __reflect_regist_class() {
      __global_factory_map<Base, REFLECT_TAG>().insert(
        std::make_pair<std::string, factory<Base, T, REFLECT_TAG>* >(
          std::string(class_name_wp::get()), new factory<Base, T, REFLECT_TAG>()
        )
      );
    }

    void* get() { return nullptr; }
  };

  template <class Base,
            class T,
            typename class_name_wp,
            typename REFLECT_TAG = default_reflect_tag>
  class __reflect_regist_class_caller {
    static __reflect_regist_class<Base,
                                  T,
                                  class_name_wp,
                                  REFLECT_TAG> __register;

  public:
    __reflect_regist_class_caller() {
      __register.get();
    }
  };

  template <class Base,
            class T,
            typename class_name_wp,
            typename REFLECT_TAG>
  __reflect_regist_class<Base,
                         T,
                         class_name_wp,
                         REFLECT_TAG>
  __reflect_regist_class_caller<Base,
                                T,
                                class_name_wp,
                                REFLECT_TAG>::__register;

#define __string_wrapper(s)     \
  struct __string_wrapper_##s { \
    static char const *         \
    get() {                     \
      return #s;                \
    }                           \
  };

#define re_class_2(classname, Base)                    \
__string_wrapper(classname)                            \
static canftin_reflect::__reflect_regist_class_caller< \
  Base,                                                \
  class classname,                                     \
  __string_wrapper_##classname>                        \
  _registor_caller_##classname;                        \
class classname : public Base

#define re_class_3(classname, Base, tag)               \
__string_wrapper(classname)                            \
static canftin_reflect::__reflect_regist_class_caller< \
  Base,                                                \
  class classname,                                     \
  __string_wrapper_##classname, tag>                   \
  _registor_caller_##classname;                        \
class classname : public Base

#define __re_class_get_macro(_1, _2, _3, NAME, ...) NAME

#define re_class(...) __re_class_get_macro( \
  __VA_ARGS__, re_class_3, re_class_2)(__VA_ARGS__)

#define reflect_1(T) (canftin_reflect::__global_factory_map<T>())

#define reflect_2(T, tag) (canftin_reflect::__global_factory_map<T, tag>())

#define __REFLECT_GET_MACRO(_1, _2, NAME, ...) NAME

#define reflect(...) __REFLECT_GET_MACRO( \
  __VA_ARGS__, reflect_2, reflect_1)(__VA_ARGS__)
}


#endif