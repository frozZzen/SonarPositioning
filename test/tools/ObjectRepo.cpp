#include "../pch.h"

#include <tools/ObjectRepo.h>

#include <functional>

using namespace sp::tools::repo;

TEST(FactoryRepo, SetUpAndUseFactories)
{
  struct Interface1 { int _value = 11; };
  struct Interface2 { int _value = 22; };
  struct Interface3 { double _value = 33.3; };
  struct Interface4 { char _value = '!'; };

  using Factory1 = std::function<Interface1(int, int)>;
  using Factory2 = std::function<Interface2(int)>;
  using Factory3 = std::function<Interface3(double)>;
  using Factory4 = std::function<Interface4()>;

  using Repo = ObjectRepo<
    TaggedObject<Interface1, Factory1>,
    TaggedObject<Interface2, Factory2>,
    TaggedObject<Interface3, Factory3>,
    TaggedObject<Interface4, Factory4>
  >;

  Repo repo;

  repo.set<Interface1>([](int i1_, int i2_) { return Interface1{i1_ + i2_}; });
  repo.set<Interface2>([](int i_) { return Interface2{i_}; });
  repo.set<Interface3>([](double d_) { return Interface3{d_}; });
  repo.set<Interface4>([]() { return Interface4{'?'}; });

  const auto& factory1 = repo.get<Interface1>();
  const auto& factory2 = repo.get<Interface2>();
  const auto& factory3 = repo.get<Interface3>();
  const auto& factory4 = repo.get<Interface4>();

  auto obj1 = factory1(123, 456);
  auto obj2 = factory2(123456);
  auto obj3 = factory3(654.321);
  auto obj4 = factory4();

  EXPECT_EQ(obj1._value, 579);
  EXPECT_EQ(obj2._value, 123456);
  EXPECT_EQ(obj3._value, 654.321);
  EXPECT_EQ(obj4._value, '?');
}
