#include <globus_duroc_runtime.h>

int main(int argc, char **argv)
{

#if defined(GLOBUS_CALLBACK_GLOBAL_SPACE)
    globus_module_set_args(&argc, &argv);
#endif

    globus_module_activate(GLOBUS_DUROC_RUNTIME_MODULE);
    globus_duroc_runtime_barrier();
    globus_module_deactivate(GLOBUS_DUROC_RUNTIME_MODULE);

    printf("hello, world\n");
}
