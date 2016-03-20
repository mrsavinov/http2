#pragma once
#include <string>
#include <memory>
namespace etu{
namespace base{
class Buffer;

typedef std::shared_ptr<base::Buffer> BufferPtr;

class Buffer {
public:
	typedef char Byte;
	explicit Buffer(std::size_t size) : m_data(){
		m_data.resize(size);
	}
	static BufferPtr make(std::size_t size){
		return std::make_shared<base::Buffer>(size);
	}
	Buffer() = default;
	Buffer(const Buffer& orig) = default;
	Buffer(Buffer&& orig) = default;
	Buffer& operator=(const Buffer& orig) = delete;
	Buffer& operator=(const Buffer&& orig) = delete;
	Byte* data(){return &(*m_data.begin());}
	std::size_t capacity() const {return m_data.size();}
	std::size_t size() const { return m_size; }
	std::size_t addSize(std::size_t s) { return m_size += s; }
	void resize(int addOrShrinkSize){
		long long s = size() + addOrShrinkSize;
        if (capacity() < s) {
            m_data.resize(s);
        }
        m_size = s;
	}
    void shrinkToFit() {
        m_data.shrink_to_fit();
    }
	Byte* find(const Byte* subString){
		auto pos = m_data.find(subString);
		if(std::string::npos != pos){
			return (&(*m_data.begin()))+pos;
		}
		return nullptr;
	}
	virtual ~Buffer(){};
public:
	
private:
	std::string m_data;
	std::size_t m_size;
};


}
}