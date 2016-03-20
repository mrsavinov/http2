#include "ConfigReaderWriter.hpp"
#include "Config.hpp"
#include "BaseLogger.hpp"

namespace etu {
namespace base {

class LinesFromFileReader {

public:
    typedef std::vector<std::string> Lines;
    LinesFromFileReader();
    bool Init(base::File& file);
    static bool Write(base::File& file, const Lines& lines, std::string messageAtEnd);
    static bool Write(base::File& file, const std::string& lines);
    void ToStart();
    bool next(std::string& s)const;
    bool IsEnd()const;
    bool GetPrevWithoutSeek(std::string& s)const;
    void error();
    int getErrorLine()const;
    void clear();
    Lines getLines()const;

private:
    static bool getLine(base::LinesFromFileReader& file, std::string& str);
    static std::string getLineCheckMultiStringComment(base::LinesFromFileReader& file);

private:
    Lines m_lines;
    mutable int m_index;
    int errorIndex;
};

const char* startObject = "{";
const char* endObject = "}";
const char* startArray = "[";
const char* endArray = "]";
const char* keyValDelim = "=";
const char* multiLineCommentStart = "#/*#";
const char* multiLineCommentEnd = "#*/#";


LinesFromFileReader::LinesFromFileReader()
    :m_lines()
    , m_index(-1)
    , errorIndex(-1) {}

bool LinesFromFileReader::Init(base::File& file) {
    //bool result = file.Read();
    //if (result) {
    //    m_lines.swap(file.GetLines());
    //    ToStart();
    //} else {
    //    BASE_LOG_ERROR("Read file error. " << file.LastError());
    //}
    return false;
}

bool LinesFromFileReader::Write(base::File& file, const Lines& lines, std::string messageAtEnd) {
    //file.GetLines().insert(file.GetLines().begin(), lines.begin(), lines.end());
    //bool result = file.Write(messageAtEnd);
    //if (!result) {
    //    BASE_LOG_ERROR("Write file error. " << file.LastError());
    //}
    //return result;
    return false;
}

bool LinesFromFileReader::Write(base::File& file, const std::string& lines) {
    //bool result = file.WriteString(lines);
    //if (!result) {
    //    BASE_LOG_ERROR("Write file error. " << file.LastError());
    //}
    //return result;
    return false;
}

void LinesFromFileReader::ToStart() {
    errorIndex = -1;
    m_index = 0;
}

bool LinesFromFileReader::next(std::string& s)const {
    if (m_index >= 0 && size_t(m_index) < m_lines.size()) {
        s = m_lines[m_index];
        m_index++;
        return true;
    }
    return false;
}

bool LinesFromFileReader::IsEnd()const {
    return !(m_index >= 0 && size_t(m_index) < m_lines.size());
}

bool LinesFromFileReader::GetPrevWithoutSeek(std::string& s)const {
    int i = m_index - 2;
    if (i >= 0 && size_t(i) < m_lines.size()) {
        s = m_lines[i];
        return true;
    }
    return false;
}

void LinesFromFileReader::error() {
    errorIndex = m_index - 1;
}

int LinesFromFileReader::getErrorLine()const {
    return errorIndex;
}

void LinesFromFileReader::clear() {
    ToStart();
    m_lines.clear();
}

LinesFromFileReader::Lines LinesFromFileReader::getLines()const {
    return m_lines;
}

bool LinesFromFileReader::getLine(base::LinesFromFileReader& reader, std::string& str) {
    std::string tmp;
    do {
        tmp = getLineCheckMultiStringComment(reader);
    } while (!reader.IsEnd() && tmp.empty() || (!tmp.empty() && tmp[0] == '#'));
    std::string::size_type pos = tmp.find('#');
    if (pos != std::string::npos) {
        std::string::size_type spacePos = tmp.find(' ');
        if (pos != std::string::npos && spacePos < pos) {
            bool allSpace = true;
            for (std::string::size_type i = spacePos; i < pos; i++) {
                allSpace = tmp[i] == ' ';
            }
            if (allSpace) {
                pos = spacePos;
            }
        }
        str.insert(str.begin(), tmp.begin(), tmp.begin() + pos);
    } else {
        str.swap(tmp);
    }
    return !str.empty();
}

std::string LinesFromFileReader::getLineCheckMultiStringComment(base::LinesFromFileReader& file) {
    std::string tmp;
    if (file.IsEnd()) {
        return tmp;
    }
    try {
        while (tmp.empty() && !file.IsEnd()) {
            file.next(tmp);
            tmp;// = base::String::removeAllSpaces(tmp);
        }
        while (tmp.find(multiLineCommentStart) != std::string::npos) {
            while (tmp.find(multiLineCommentEnd) == std::string::npos && file.next(tmp)) {
            }
            file.next(tmp);
        }
    } catch (...) {
    }
    return tmp;
}

static const int FLAG_READ_ARRAY = 1;
static const int FLAG_READ_OBJECT = 2;
static const int FLAG_APPLY_SPACES = 4;

bool ConfigReaderWriter::readConfigRecursive(base::LinesFromFileReader& config, Config& conf, int flags) {
    std::string s;
    int index = 0;
    bool applySpaces = (flags & FLAG_APPLY_SPACES);
    std::string symbolsForRemove("\n\t");
    if (!applySpaces) {
        symbolsForRemove.push_back(' ');
    }
    while (config.next(s)) {
        std::string tmp = base::String::removeAllSpaces(s);
        if (!tmp.empty() && tmp[0] == '#') {
            continue;
        }
        std::string::size_type pos = s.find(keyValDelim);
        if (pos != std::string::npos) {
            std::string key = base::String::removeAllSymbols(s.substr(0, pos), symbolsForRemove);
            std::string value = base::String::removeAllSymbols(s.substr(pos + 1, s.size() - pos), symbolsForRemove);
            conf.Set(key, value);
        } else if (s.find(startObject) != std::string::npos) {
            std::string::size_type pos2 = s.find(startObject);
            std::string curString = base::String::removeAllSymbols(s, symbolsForRemove);
            std::string key;
            if (curString == startObject) {
                std::string sPrev;
                if (config.GetPrevWithoutSeek(sPrev)) {
                    key = sPrev;
                }
            } else {
                key = base::String::removeAllSymbols(curString, startObject);
            }
            Config subConf;
            int fl = FLAG_READ_OBJECT;
            fl |= (applySpaces) ? FLAG_APPLY_SPACES : 0;
            if (!readConfigRecursive(config, subConf, fl)) {
                break;
            }
            conf.Set(key, subConf);
        } else if (s.find(endObject) != std::string::npos) {
            return true;
        } else if (s.find(startArray) != std::string::npos) {
            std::string::size_type pos2 = s.find(startArray);
            std::string curString = base::String::removeAllSymbols(s, symbolsForRemove);
            std::string key;
            if (curString == startArray) {
                std::string sPrev;
                if (config.GetPrevWithoutSeek(sPrev)) {
                    key = sPrev;
                }
            } else {
                key = base::String::removeAllSymbols(curString, startArray);
            }
            Config subConf;
            int fl = FLAG_READ_ARRAY;
            fl |= (applySpaces) ? FLAG_APPLY_SPACES : 0;
            if (!readConfigRecursive(config, subConf, fl)) {
                break;
            }
            conf.Set(key, subConf);
        } else if (s.find(endArray) != std::string::npos) {
            return true;
        } else if (flags & FLAG_READ_ARRAY) {
            std::string arKey = base::String::ToString(index);
            std::string value = base::String::removeAllSymbols(s, symbolsForRemove);
            conf.Set(arKey, value);
        } else if (!base::String::removeAllSpaces(s).empty()) {
            break;
        }
        ++index;
    }
    bool okIfLineEnded = ((flags & FLAG_READ_ARRAY & FLAG_READ_OBJECT) == 0);
    return okIfLineEnded;
}

class Walker {
    std::string val;
    const char* nextLine = "\n";
public:
    void swap(Walker& w) {
        val.swap(w.val);
    }
    void swap(std::string& w) {
        val.swap(w);
    }
    const Walker& operator()(const std::string& s) {
        val += s;
        val += nextLine;
        return *this;
    }
    const Walker& operator +=(const std::string& s) {
        return this->operator()(s);
    }
};

bool ConfigReaderWriter::walkConfigToString(const Config& conf, Walker& saver) {
    if (conf) {
        bool needAddEnd = false;
        if (!conf.m_map.empty()) {
            saver += startObject;
            needAddEnd = true;
            for (Config::ConfMap::const_iterator it = conf.m_map.begin(); it != conf.m_map.end(); it++) {
                saver += it->first;
                saver += startObject;
                if (!it->second) {
                    walkConfigToString(*it->second, saver);
                }
                saver += endObject;
            }
        } else {
            saver += ("=");
            saver += conf.m_val;
        }
        return true;
    }
    return false;
}

bool ConfigReaderWriter::read(base::File& file, Config& conf, Config::BaseLibConfig) {
    base::LinesFromFileReader config;
    bool result = config.Init(file);
    if (result) {
        std::string s;
        size_t index = 0;
        config.next(s);
        std::string curString = base::String::removeAllSpaces(s);
        result = curString == startObject;
    }
    if (result) {
        result = readConfigRecursive(config, conf, FLAG_READ_OBJECT);
        if (!result) {
            config.error();
            int errorLine = config.getErrorLine();
            std::string line;
            if (errorLine >= 0) {
                line = config.getLines()[errorLine];
            }
            BASE_LOGF_ERROR("Read config error. Error line(%d):%s", errorLine, line.c_str());
        }
    }
    return result;
}

bool ConfigReaderWriter::read(base::File& file, Config& conf, Config::UnixConfig) {
    base::LinesFromFileReader config;

    if (config.Init(file)) {
        bool res = readConfigRecursive(config, conf, FLAG_APPLY_SPACES);
        return res;
    }
    return false;
}

bool ConfigReaderWriter::write(base::File& file, const Config& conf, Config::BaseLibConfig) {
    Walker walker;
    walker += startObject;
    walkConfigToString(conf, walker);
    walker += endObject;
    std::string content;
    walker.swap(content);
    return base::LinesFromFileReader::Write(file, content);
}

bool ConfigReaderWriter::write(base::File& file, const Config& conf, Config::UnixConfig) {
    Walker walker;
    walkConfigToString(conf, walker);
    std::string content;
    walker.swap(content);
    return base::LinesFromFileReader::Write(file, content);
}

ConfigReaderWriter* ConfigReaderWriter::create(base::File& file) {
    return new ConfigReaderWriter();
}

void ConfigReaderWriter::release(ConfigReaderWriter*& w) {
    delete w;
    w = NULL;
}

#if 0
#ifdef UTEST
class ConfigReaderWriterForTests : public ConfigReaderWriter {
public:
    virtual bool read(base::File& configFile, base::Config& config, base::Config::UnixConfig);
    virtual bool read(base::File& configFile, base::Config& config, base::Config::BaseLibConfig);
    virtual bool write(base::File& configFile, const base::Config& config, base::Config::BaseLibConfig);
    virtual bool write(base::File& configFile, const base::Config& config, base::Config::UnixConfig);

    void addConfig(std::string configFileName, base::Config& config, bool readConfigResult);
private:
    typedef ConfigReaderWriter Parent;
    struct ConfigKeeper {
        Config config;
        bool readResult;
    };
    typedef std::map<std::string, ConfigKeeper> ConfigStorage;
private:
    template<class T>
    virtual bool readInternal(base::File& configFile, base::Config& config, T t) {
        auto it = m_storage.find(configFile.FileName());
        if (it != m_storage.end()) {
            config = it->second.config;
            return it->second.readResult;
        }
        return Parent::read(configFile, config, type);
    }
    template<class T>
    virtual bool writeInternal(base::File& configFile, const base::Config& config, T t) {
        auto it = m_storage.find(configFile.FileName());
        if (it == m_storage.end()) {
            it->second.config = config;
            return true;
        }
        return Parent::write(configFile, config, t);
    }
private:
    static ConfigStorage m_storage;
};

bool ConfigReaderWriterForTests::read(base::File& configFile, base::Config& config, base::Config::UnixConfig type) {
    return readInternal(configFile, config, type);
}

bool ConfigReaderWriterForTests::read(base::File& configFile, base::Config& config, base::Config::BaseLibConfig type) {
    return readInternal(configFile, config, type);
}

bool ConfigReaderWriterForTests::write(base::File& configFile, const base::Config& config, base::Config::BaseLibConfig type) {
    return writeInternal(configFile, config, type);
}

bool ConfigReaderWriterForTests::write(base::File& configFile, const base::Config& config, base::Config::UnixConfig type) {
    return writeInternal(configFile, config, type);
}

void ConfigReaderWriterForTests::addConfig(std::string configFileName, base::Config& config, bool readConfigResult) {
    auto& c = m_storage[configFileName];
    c.config = config;
    c.readResult = readConfigResult;
}

ConfigReaderWriterForTests::ConfigStorage ConfigReaderWriterForTests::m_storage;
#endif

void ConfigReaderWriter::addConfig(std::string configFileName, base::Config& config, bool readConfigResult) {
#ifdef UTEST
    ConfigReaderWriterForTests::addConfig(configFileName, config, readConfigResult);
#endif
}

ConfigReaderWriter* ConfigReaderWriter::create(base::File& file) {
    return new
#ifdef UTEST
        ConfigReaderWriterForTests()
#else
        ConfigReaderWriter()
#endif
        ;
}

void ConfigReaderWriter::release(ConfigReaderWriter*& w) {
    delete w;
    w = NULL;
}
#endif
}
}