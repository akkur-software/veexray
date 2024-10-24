#include <stdio.h>
#include <sources/Veex.Core/Uuid4.h>

using Veex::Core::Uuid4;

int main()
{
    auto uuid1 = Uuid4();
    auto uuid2 = Uuid4(uuid1);

    auto uuidStr = uuid2.ToString();
    auto uuid3 = Uuid4::Parse(uuidStr);

    ::printf("UUID4 (1): %s\n", uuid1.ToString().data());
    ::printf("UUID4 (2): %s\n", uuid2.ToString().data());
    ::printf("UUID4 (3): %s\n", uuid3.ToString().data());

    Uuid4* uuid4 = nullptr;
    auto isEquals = &uuid3 == uuid4;

    return ::getchar();
}