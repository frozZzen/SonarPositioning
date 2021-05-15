#pragma once

#include <type_traits>

namespace sp::tools::repo
{  
  namespace details
  {
    template <typename T> struct Tag {};
    template <typename TaggedObject> struct ObjectContainer;
    template <typename TaggedObject, typename... Args> struct ObjectRepoImpl;
    template <typename CheckedType, template <typename, typename> typename Matcher, typename Type, typename... OtherTypes> struct IsTypeInTypeList;
    template <typename Tag, typename TaggedObject> struct TagTaggedObjectMatcher;
    template <typename Object, typename TaggedObject> struct ObjectTaggedObjectMatcher;
  }

  template <typename TagT, typename ObjectT>
  struct TaggedObject
  {
    using Tag = TagT;
    using Object = ObjectT;
  };

  template <typename TaggedObject, typename... OtherTaggedObjects>
  class ObjectRepo;

  template <typename TagT, typename ObjectT, typename... OtherTags, typename... OtherObjects>
  class ObjectRepo<TaggedObject<TagT, ObjectT>, TaggedObject<OtherTags, OtherObjects>...>
    : private details::ObjectRepoImpl<TaggedObject<TagT, ObjectT>, TaggedObject<OtherTags, OtherObjects>...>
  {
  public:
    template <typename Tag, typename Object>
    void set(Object&& object_)
    {
      static_assert(details::IsTypeInTypeList<Tag, details::TagTaggedObjectMatcher, TaggedObject<TagT, ObjectT>, TaggedObject<OtherTags, OtherObjects>...>::value, "Invalid tag!");
      static_assert(details::IsTypeInTypeList<Object, details::ObjectTaggedObjectMatcher, TaggedObject<TagT, ObjectT>, TaggedObject<OtherTags, OtherObjects>...>::value, "Invalid object!");
      return setImpl(details::Tag<Tag>{}, std::forward<Object>(object_));
    }

    template <typename Tag>
    auto get()
    {
      static_assert(details::IsTypeInTypeList<std::decay_t<Tag>, details::TagTaggedObjectMatcher, TaggedObject<TagT, ObjectT>, TaggedObject<OtherTags, OtherObjects>...>::value, "Invalid tag!");
      return getImpl(details::Tag<Tag>{});
    }

  private:
    using details::ObjectContainer<TaggedObject<TagT, ObjectT>>::setImpl;
    using details::ObjectContainer<TaggedObject<OtherTags, OtherObjects>>::setImpl...;

    using details::ObjectContainer<TaggedObject<TagT, ObjectT>>::getImpl;
    using details::ObjectContainer<TaggedObject<OtherTags, OtherObjects>>::getImpl...;
  };

  namespace details
  {
    template <typename TaggedObject>
    struct ObjectContainer
    {
      template <typename Object>
      void setImpl(Tag<typename TaggedObject::Tag>, Object&& object_) { _object = std::forward<Object>(object_); }

      typename TaggedObject::Object& getImpl(Tag<typename TaggedObject::Tag>) { return _object; }

      typename TaggedObject::Object _object;
    };

    template <typename TaggedObject, typename... Args>
    struct ObjectRepoImpl : ObjectContainer<TaggedObject>, ObjectRepoImpl<Args...> {};

    template <typename TaggedObject>
    struct ObjectRepoImpl<TaggedObject> : ObjectContainer<TaggedObject> {};

    template <typename CheckedType, template <typename, typename> typename Matcher, typename Type, typename... OtherTypes>
    struct IsTypeInTypeList : std::conditional_t<Matcher<CheckedType, Type>::value, std::true_type, IsTypeInTypeList<CheckedType, Matcher, OtherTypes...>> {};

    template <typename CheckedType, template <typename, typename> typename Matcher, typename Type>
    struct IsTypeInTypeList<CheckedType, Matcher, Type> : std::conditional_t<Matcher<CheckedType, Type>::value, std::true_type, std::false_type> {};

    template <typename Tag, typename TaggedObject>
    struct TagTaggedObjectMatcher : std::conditional_t<std::is_same_v<Tag, typename TaggedObject::Tag>, std::true_type, std::false_type> {};

    template <typename Object, typename TaggedObject>
    struct ObjectTaggedObjectMatcher : std::conditional_t<std::is_convertible_v<Object, typename TaggedObject::Object>, std::true_type, std::false_type> {};
  }
}
