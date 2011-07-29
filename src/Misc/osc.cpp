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
        int argc, void *, void *user_data)
{
    //return 1;
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
}

void osc::run_ui(void)
{
    lo_server_thread_start(ui_server);
}


const char *osc::mkPath(const char *fmt, size_t idx)
{
    const size_t buffLen = 200;
    static char buf[buffLen + 1];
    snprintf(buf, buffLen, fmt, idx);
    return buf;
}

void osc::UI_RM(const char *path, const char *type)
{
    lo_server_thread_del_method(osc::ui_server, path, type);
}

void osc::UI_FN(const char *path, const char *type, 
                lo_method_handler handle, void *arg)
{
    //printf("UI registered %s\n", path);
    lo_server_thread_add_method(osc::ui_server, path, type, handle, arg);
}
    
void osc::UI_NFN(const char *path, size_t idx, const char *type, 
                 lo_method_handler handle, void *arg)
{
    osc::UI_FN(mkPath(path,idx), type, handle, arg);
}

void osc::UI_RE(const char *path)
{
    printf("sending request out to %s\n", path);
    lo_send(backend, path, "N");
}

void osc::UI_NRE(const char *path, size_t idx)
{
    osc::UI_RE(osc::mkPath(path, idx));
}

void osc::UI_UP(const char *path, char val)
{
    //printf("UI sent update\n");
    lo_send(osc::backend, path, "c", val);
}

void osc::UI_NUP(const char *path, size_t idx, char val)
{
    osc::UI_UP(osc::mkPath(path, idx), val);
}
