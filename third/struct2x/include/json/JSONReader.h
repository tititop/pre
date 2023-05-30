#ifndef __JSON_READER_H__
#define __JSON_READER_H__
#include <string>
#include <map>
#include "../struct2x.h"


struct cJSON;
namespace struct2x
{

class EXPORTAPI JSONReader
{
    cJSON* _root;
    cJSON* _cur;
    JSONReader(const JSONReader&);
    JSONReader& operator=(const JSONReader&);
public:
    JSONReader();
    ~JSONReader();

    template<typename T>
    JSONReader& operator&(serializeItem<T> value)
    {
        return convert(value.name, value.value);
    }

    template<typename T>
    JSONReader& operator << (const T& value)
    {
        const typename internal::TypeTraits<T>::Type& v = value;
        if (internal::TypeTraits<T>::isVector()) {
            operator << (v);
        } else {
            if (typename internal::TypeTraits<T>::Type* pValue = const_cast<typename internal::TypeTraits<T>::Type*>(&v)) {
                internal::serializeWrapper(*this, *pValue);
            }
        }
        return *this;
    }

    template<typename K, typename V>
    JSONReader& operator <<(const std::map<K, V>& value)
    {
        for (typename std::map<K, V>::const_iterator it = value.begin(); it != value.end(); ++it) {
            const V& item = it->second;
            std::string key = internal::STOT::type<K>::tostr(it->first);
            convert(key, item);
        }
        return *this;
    }

    // 自定义数据结构
    template<typename T>
    JSONReader& convert(const std::string& sz, const T& value)
    {
        cJSON* curItem = cur();
        createObject(sz);
        internal::serializeWrapper(*this, *const_cast<T*>(&value));
        cur(curItem);
        return *this;
    }

    // 自定义数据结构+数组容器
    template<typename T>
    JSONReader& convert(const std::string& sz, const std::vector<T>& value)
    {
        cJSON* curItem = cur();
        createArray(sz);
        int size = (int)value.size();
        for (int i = 0; i < size; ++i) {
            cJSON* lastItem = cur();
            if (internal::TypeTraits<T>::isVector()) {
                addArrayToArray();
            } else {
                addItemToArray();
            }
            const typename internal::TypeTraits<T>::Type& item = value.at(i);
            this->operator<<(item);
            cur(lastItem);
        }
        cur(curItem);
        return *this;
    }

    // 自定义数据结构+红黑树容器
    template<typename K, typename V>
    JSONReader& convert(const std::string& sz, const std::map<K, V>& value)
    {
        cJSON* curItem = cur();
        createObject(sz);
        for (typename std::map<K, V>::const_iterator it = value.begin(); it != value.end(); ++it) {
            cJSON* lastItem = cur();
            const V& item = it->second;
            std::string key = internal::STOT::type<K>::tostr(it->first);
            convert(key, item);
            cur(lastItem);
        }
        cur(curItem);
        return *this;
    }

    bool toString(std::string& str, bool bUnformatted = false);
    JSONReader& convert(const std::string& sz, int value);
    JSONReader& convert(const std::string& sz, float value);
    JSONReader& convert(const std::string& sz, double value);
    JSONReader& convert(const std::string& sz, unsigned int value);
    JSONReader& convert(const std::string& sz, const char* value);
    JSONReader& convert(const std::string& sz, const std::string& value);
    JSONReader& convert(const std::string& sz, bool value);
    JSONReader& convert(const std::string& sz, const std::vector<int>& value);
    JSONReader& convert(const std::string& sz, const std::vector<float>& value);
    JSONReader& convert(const std::string& sz, const std::vector<double>& value);
    JSONReader& convert(const std::string& sz, const std::vector<std::string>& value);

private:
    JSONReader& operator<<(const std::vector<int>& value);
    JSONReader& operator<<(const std::vector<float>& value);
    JSONReader& operator<<(const std::vector<double>& value);
    JSONReader& operator<<(const std::vector<std::string>& value);

    void createObject(const std::string& sz);
    void createArray(const std::string& sz);
    void addItemToArray();
    void addArrayToArray();
    void cur(cJSON* item)
    {
        _cur = item;
    }
    cJSON* cur()
    {
        return _cur;
    }
};

}

#endif