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
    void run_ui(void);

    void process_backend(void);


//lo_method lo_server_thread_add_method(lo_server_thread st, const char *path,
//                               const char *typespec, lo_method_handler h,
//                               void *user_data);

    const char *mkPath(const char *fmt, size_t idx);

    void UI_RM(const char *path, const char *type);

    void UI_FN(const char *path, const char *type, 
                      lo_method_handler handle, void *arg);
    
    void UI_NFN(const char *path, size_t idx, const char *type, 
                      lo_method_handler handle, void *arg);

    void UI_RE(const char *path);
    void UI_NRE(const char *path, size_t idx);

    void UI_UP(const char *path, char val);
    void UI_NUP(const char *path, size_t idx, char val);
    
    void UI_FN(const char *path, const char *type, 
                      lo_method_handler handle, void *arg);
    
    void UI_NFN(const char *path, size_t idx, const char *type, 
                      lo_method_handler handle, void *arg);

    extern lo_server        back_server;
    extern lo_server_thread ui_server;
    extern lo_address       backend, ui;
};

#endif
