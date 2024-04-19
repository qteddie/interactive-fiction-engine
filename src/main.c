#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <jansson.h>
#include <toml.h>
static void error(const char* msg, const char* msg1)
{
    fprintf(stderr, "ERROR: %s%s\n", msg, msg1?msg1:"");
    exit(1);
}


int main()
{
    toml_table_t* server = toml_table_in(root, "server");
    if (server) {
        toml_datum_t host = toml_string_in(server, "host");
        if (host.ok) {
            json_object_set_new(root, "host", json_string(host.u.s));
            free(host.u.s);
        }

        toml_array_t* port = toml_array_in(server, "port");
        if (port) {
            json_t* port_array = json_array();
            for (int i = 0; i < toml_array_nelem(port); i++) {
                toml_datum_t p = toml_int_at(port, i);
                if (p.ok) {
                    json_array_append_new(port_array, json_integer(p.u.i));
                }
            }
            json_object_set_new(root, "port", port_array);
        }
    }

    json_dump_file(root, "static/json/output.json", JSON_INDENT(4));
    return 0;

}