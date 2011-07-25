#include "osc.h"
#include <cstdio>

namespace osc
{
    lo_server        back_server;
    lo_server_thread ui_server;
    lo_address       backend, ui;
};

void osc::error(int num, const char *msg, const char *path)
{
    printf("liblo server error %d in path %s: %s\n", num, path, msg);
}

/* catch any incoming messages and display them. returning 1 means that the
 * message has not been fully handled and the server should try other methods */
int osc::generic_handler(const char *path, const char *types, lo_arg **argv,
        int argc, void *data, void *user_data)
{
    printf("%s[%s := ", user_data, path);
    for (int i=0; i<argc; i++) {
        printf("arg %d '%c' ", i, types[i]);
        lo_arg_pp((lo_type)types[i], argv[i]);
    }
    printf("]\n");
    fflush(stdout);

    return 1;
}

void osc::init_backend(void)
{
    back_server = lo_server_new("7770", error);
    lo_server_add_method(back_server, NULL, NULL, generic_handler, (void*)"BACKEND");
    backend = lo_address_new(NULL, "7770");
}

void osc::process_backend(void)
{
    while(lo_server_recv_noblock(back_server,0));
}

void osc::init_ui(void)
{
    ui_server = lo_server_thread_new("7771", error);
    lo_server_thread_add_method(ui_server, NULL, NULL, generic_handler, (void*)"FRONTEND");
    ui        = lo_address_new(NULL, "7771");
    lo_server_thread_start(ui_server);
}

