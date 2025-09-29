#include <engine.hpp>
#include <stdio.h>

int main(int argc, char** argv)
{
    printf("Entered main\n");

    engine *app = new engine();

    app->run();

    return 0;
}
