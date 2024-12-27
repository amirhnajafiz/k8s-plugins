#include <string.h> // string
#include <yaml-cpp/yaml.h> // YAML

using namespace std;

struct server_config {
    int port;
    int queue_size;
};

struct server_config read_config(const string& path);
