#include "tools.hpp"
#include "Sniff.hpp"

int main(int argc, char* argv[])
{
    banner();
    Sniff *s = new Sniff(argc, argv);
    s->run();
    bye();
    delete s;
    return 0;
}
