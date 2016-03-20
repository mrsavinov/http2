#pragma once

#include <vector>
#include <map>

namespace etu {
namespace base {

class LinesFromFileReader;
class ConfigWalker;
class ConfigReaderWriter;
class File;

class Config {
    typedef std::map < std::string, Config* > ConfMap;
    typedef std::vector<std::string> ConfigOrderArray;

public:
    class UnixConfig {};
    class BaseLibConfig {};
    typedef const Config& Handler;
    static bool ReadConfig(base::File& file, Config& conf, BaseLibConfig);
    static bool ReadConfig(base::File& file, Config& conf, UnixConfig);
    static bool WriteConfig(base::File& file, const Config& conf, BaseLibConfig);
    static bool WriteConfig(base::File& file, const Config& conf, UnixConfig);

    Config();
    Config(std::string val);
    Config(const Config& conf);
    ~Config();

    const Config& operator [] (const std::string& key) const;
    const Config& operator [] (const char* key) const;
    const Config& operator [] (int key) const;
    Config& operator= (const Config& copy);

    const Config& Next()const;
    void ToStart();
    std::string ToString() const;

    operator std::string() const;
    std::string operator()()const;
    operator bool()const;
    operator int()const;

    template<class T>
    void Set(const std::string& key, const T& value) {
        Remove(key);
        Config* conf = new Config(value);
        conf->SetName(key);
        m_map[key] = conf;
        m_orderArray.push_back(key);
    }

    bool empty() const;
    bool HasKey(const std::string& key)const;
    const char* c_str()const;
    void Clear();
    int IsObject();
    int size() const;
    std::string GetName()const { return (this) ? m_name : ""; }

private:
    friend class Walker;
    friend class ConfigReaderWriter;
    void checkVal();
    bool Remove(const std::string& key);
    Config* Isset(const std::string& key) const;
    bool is()const;
    void SetName(std::string name)const;

private:
    ConfMap m_map;
    ConfigOrderArray m_orderArray;
    std::string m_val;
    mutable std::string m_name;
    bool m_valIsEmpty;
    mutable ConfigOrderArray::iterator currentIt;
};

}
}
