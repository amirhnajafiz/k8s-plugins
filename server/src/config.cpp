#include "../include/config.h"

// create a function that reads the config file into the server configuration struct
struct server_config read_config(const string& path)
{
    // load config file
    YAML::Node config = YAML::LoadFile(path);

    // create a struct for server configuration
    struct server_config s_config;
    s_config.port = config["port"].as<int>();
    s_config.queue_size = config["queue_size"].as<int>();

    return s_config;
}
