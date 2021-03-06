#include "jsfeature.hpp"
//#include "feature_export.hpp"
#include "crossfilter.hpp"
#include "utils.hpp"
#include "feature_cast_extern.hpp"

template<typename Key, typename Value, typename DimType, bool is_group_all, typename I>
static  napi_value size_(napi_env env, js_function & jsf, jsfeature * obj) {
  auto & feature = cast_feature<Key,Value, DimType, is_group_all, I>(obj->ptr);
  auto v = feature.size();
  return convert_from<int64_t>(env,v);
}

template<typename Value>
static  napi_value size_(napi_env env,  jsfeature * obj) {
  auto & feature = cast_feature_filter<Value>(obj->ptr);
  auto v = feature.size();
  return convert_from<uint64_t>(env,v);
}

napi_value  jsfeature::size(napi_env env, napi_callback_info info) {
  js_function jsf = extract_function(env, info, 0);
  jsfeature* obj = get_object<jsfeature>(env, jsf.jsthis);
  //  CALL_DISPATCH3(obj->key_type, obj->value_type, obj->dim_type,obj->is_group_all, size_, env, jsf, obj);
  if(obj->dim_type != is_cross) {
    CALL_DISPATCH_F(obj->key_type, obj->value_type, obj->dim_type,obj->is_group_all, obj->is_iterable,size_, env, jsf, obj);
  } else {
    switch(obj->value_type) {
      case is_int64:
        return size_<int64_t>(env, obj);
      case is_int32:
        return size_<int32_t>(env, obj);
      case is_bool:
        return size_<bool>(env, obj);
      case is_double:
        return size_<double>(env,  obj);
      case is_uint64:
        return size_<uint64_t>(env, obj);
    }
  }

  return nullptr;
}
