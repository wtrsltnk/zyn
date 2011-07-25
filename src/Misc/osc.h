#ifndef OSC_H
#define OSC_H
#include <lo/lo.h>

namespace osc
{

    void error(int num, const char *msg, const char *path);
    int generic_handler(const char *path, const char *types, lo_arg **argv,
            int argc, void *data, void *user_data);
    void init_backend(void);
    void init_ui(void);

    void process_backend(void);

    extern lo_server        back_server;
    extern lo_server_thread ui_server;
    extern lo_address       backend, ui;
};

#endif
