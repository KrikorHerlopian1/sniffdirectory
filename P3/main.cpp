#include "tools.hpp"
#include "Sniff.hpp"

int main(int argc, char* argv[])
{
    banner();
    Sniff *s = new Sniff(argc, argv);
    s->run();
    delete s;

    return 0;
}
