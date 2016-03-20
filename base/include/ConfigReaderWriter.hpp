#pragma once
#include <map>
#include <string>
#include "Config.hpp"

namespace etu {
namespace base {

class File;
class Walker;

class ConfigReaderWriter {
public:
    static void addConfig(std::string configFileName, base::Config& config, bool readConfigResult);
    static ConfigReaderWriter* create(base::File& file);
    static void release(ConfigReaderWriter*&);
    virtual bool read(base::File& configFile, base::Config& config, base::Config::UnixConfig);
    virtual bool read(base::File& configFile, base::Config& config, base::Config::BaseLibConfig);
    virtual bool write(base::File& configFile, const base::Config& config, base::Config::BaseLibConfig);
    virtual bool write(base::File& configFile, const base::Config& config, base::Config::UnixConfig);

    virtual ~ConfigReaderWriter() {}
private:
    bool readConfigRecursive(base::LinesFromFileReader& config, Config& conf, int flags);
    bool walkConfigToString(const Config& conf, Walker& saver);
};
}
}
