#include <string.h> // string
#include <yaml-cpp/yaml.h> // YAML

using namespace std;

// server_config is a struct that contains the port and queue_size of the server
struct server_config {
    int port;
    int queue_size;
};

// read_config reads the configuration file at the given path and returns a server_config struct
struct server_config read_config(const string& path);
