#include <string>
#include <iostream>
#include <algorithm>

#include "Config.hpp"
#include "BaseLogger.hpp"
#include "ConfigReaderWriter.hpp"

using namespace etu::base;


Config::Config() : m_valIsEmpty(false) {}

Config::Config(std::string val) : m_val(val), m_valIsEmpty(false) {
	checkVal();
}

Config::Config(const Config& conf) : m_valIsEmpty(false){
    operator=(conf);
}

Config::~Config(){
	Clear();
}

void Config::Clear(){
    if (this){
		for (ConfMap::iterator it = m_map.begin(); it != m_map.end(); it++) {
			delete it->second;
		}
		m_map.clear();
		m_orderArray.clear();
		m_name.empty();
		m_valIsEmpty = false;
    }
}

const Config& Config::operator [] (const std::string& key) const {
	return *Isset(key);
}

const Config& Config::operator [] (const char* key) const {
	return operator[](std::string(key));
}

const Config& Config::operator [] (int key) const {
	Config* conf = NULL;
	if (this){
		std::string strKey = std::to_string(key);
		conf = Isset(strKey);
		if (!conf) {
			if (key >= 0 && size_t(key) < m_orderArray.size()) {
				ConfigOrderArray::const_iterator it = m_orderArray.begin();
				std::advance(it, key);
				ConfMap::const_iterator itFind = m_map.find(*it);
				if (itFind != m_map.end()) {
					conf = itFind->second;
				}
			}
		}
	}
	return *conf;
}

const Config& Config::Next()const{
	Config* conf = NULL;
	if (this){
		if (currentIt != m_orderArray.end()){
			ConfMap::const_iterator itFind = m_map.find(*currentIt);
			if (itFind != m_map.end()){
				conf = itFind->second;
			}
			currentIt++;
		}
	}
	return *conf;
}

Config& Config::operator= (const Config& copy) {
    if(this != NULL) {
        if (&copy != NULL) {
            for (ConfMap::const_iterator it = copy.m_map.begin(); it != copy.m_map.end(); it++){
                Config* conf = new Config(*it->second);
                conf->SetName(it->first);
                m_map[it->first] = conf;
            }
            m_orderArray.insert(m_orderArray.end(), copy.m_orderArray.begin(), copy.m_orderArray.end());
            m_name = copy.m_name;
            m_valIsEmpty = copy.m_valIsEmpty;
            m_val = copy.m_val;
        }
        else {
            m_valIsEmpty = true;
        }
    }
    return *this;
}

void Config::ToStart() {
	if (this) {
		currentIt = m_orderArray.begin();
	}
}

std::string Config::ToString() const {
    if (this){
		return m_val;
    }
    return "";
}

Config::operator std::string() const {
    return ToString();
}

std::string Config::operator()() const {
	if (this) {
		return m_val;
	}
	return "";
}

Config::operator int()const{
	if (this) {
		return std::atoi(m_val.c_str());
	}
	return 0;
}

Config::operator bool()const {
	return NULL != this;
}


bool Config::HasKey(const std::string& key)const {
	if (this) {
		return m_map.find(key) != m_map.end();
	}
	return false;
}

bool Config::empty() const {
    if(this) {
		return m_map.empty() && m_val.empty();
    }
    return true;
}

const char* Config::c_str()const{
    if(this) {
		return m_val.c_str();
    }
    return NULL;
}

int Config::IsObject(){
    if(this) {
		return m_val.empty() && m_valIsEmpty;
    }
    return false;
}

int Config::size() const {
    if(this) {
		return m_map.size();
    }
    return 0;
}

void Config::checkVal(){
    if(this) {
		m_valIsEmpty = m_val.empty();
    }
}

bool Config::Remove(const std::string& key){
    if(this) {
		ConfMap::iterator it = m_map.find(key);
		if (it != m_map.end()) {
			delete it->second;
			m_map.erase(it);
			return true;
		}
    }
    return false;
}

Config* Config::Isset(const std::string& key) const {
	Config* conf = NULL;
	if (this) {
		ConfMap::const_iterator it = m_map.find(key);
		if (it != m_map.end()){
			conf = it->second;
		}
	}
	return conf;
}

bool Config::is()const {
	return NULL != this;
}

void Config::SetName(std::string name)const {
    if(this) {
		m_name = name;
    }
}

bool Config::ReadConfig(base::File& file, Config& conf, BaseLibConfig t) {
    ConfigReaderWriter* rw = base::ConfigReaderWriter::create(file);
    bool res = rw->read(file, conf, t);
    base::ConfigReaderWriter::release(rw);
    return res;
}

bool Config::ReadConfig(base::File& file, Config& conf, UnixConfig t) {
    ConfigReaderWriter* rw = base::ConfigReaderWriter::create(file);
    bool res = rw->read(file, conf, t);
    base::ConfigReaderWriter::release(rw);
    return res;
}

bool Config::WriteConfig(base::File& file, const Config& conf, BaseLibConfig t) {
    ConfigReaderWriter* rw = base::ConfigReaderWriter::create(file);
    bool res = rw->write(file, conf, t);
    base::ConfigReaderWriter::release(rw);
    return res;
}

bool Config::WriteConfig(base::File& file, const Config& conf, UnixConfig t) {
    ConfigReaderWriter* rw = base::ConfigReaderWriter::create(file);
    bool res = rw->write(file, conf, t);
    base::ConfigReaderWriter::release(rw);
    return res;
}

