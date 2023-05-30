#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <map>
#include <list>

static std::string format_str(const std::string& str, size_t size) {
    if (str.size() < size) {
        return str + std::string(size - str.size(), ' ');
    }
    else {
        return str.substr(str.size() - size);
    }
}

static size_t get_file_len(const std::string& file_name) {
    size_t   rt = 0;
    FILE* pfile = nullptr;
    pfile = fopen(file_name.c_str(), "rb");
    if (pfile != nullptr) {
        fseek(pfile, 0, SEEK_END);
        rt = ftell(pfile);
    } else {
        std::cout << "call fread error: " << strerror(errno) << " " << std::string(file_name) << std::endl;
    }
    return rt;
}

static bool read_file_to_buffer(const char* file_name, void* buffer, int size, const char* read_method = "rb")
{
    bool rt = true;
    if (nullptr == buffer || size <= 0 || nullptr == file_name) {
        std::cout << "call func param error\n";
        return false;
    }
    FILE* pfile = nullptr;
    pfile = fopen(file_name, read_method);
    if (nullptr == pfile) {
        std::cout << "read file not exit" << std::string(file_name);
        return false;
    }

    int ret = fread(buffer, 1, size, pfile);
    if (ret != size) {
        std::cout << "call fread error: " << strerror(errno) << " " << std::string(file_name);
        rt = false;
    }
    fclose(pfile);
    return true;
}

static bool write_file_from_buffer(const char* file_name, const void* buffer, int size, const char* write_method = "wb")
{
    bool rt = true;
    if (nullptr == buffer || size <= 0 || nullptr == file_name) {
        std::cout << "call func param error: " << std::string(file_name);
        return false;
    }

    FILE* pfile = nullptr;
    pfile = fopen(file_name, write_method);
    if (nullptr == pfile || size <= 0) {
        std::cout << "write file not exit\n";
        return false;
    }
    int ret = fwrite(buffer, 1, size, pfile);
    if (ret != size) {
        std::cout << "call fwrite error: " << strerror(errno) << " " << std::string(file_name);
        rt = false;
    }
    fclose(pfile);
    return rt;
}

class MFile {
public:
    MFile() { }
    ~MFile() { }

    static MFile& Instance() {
        static MFile inst_file;
        return inst_file;
    }


    size_t get_file_content(char** pfile_content, const std::string& file_path) {
        size_t file_len = get_file_len(file_path);
        size_t alloc_size = getFileSize(file_len);
        auto& content = *pfile_content;

        content = (char*)Alloccate(alloc_size);
        memset(content, 0, alloc_size);
        read_file_to_buffer(file_path.c_str(), content, file_len, "rb");

        return file_len;
    }

    void drop_file_content(char** pfile_content, size_t file_len) {
        auto& content = *pfile_content;
        size_t alloc_size = getFileSize(file_len);

        memset(content, 0, alloc_size);

        Deallocate((void*)content, alloc_size);

        content = nullptr;
    }

public:
    void* Alloccate(size_t size) {
        size_t real_size = fixSize(size);

        auto _buffer_list = m_buffer_pool.find(real_size);
        if (_buffer_list == m_buffer_pool.end()) {
            m_buffer_pool[real_size] = std::list<void*>();
        }

        auto& buffer_list = m_buffer_pool[real_size];
        if (!buffer_list.size()) {
            void* new_buffer = malloc(real_size);
            memset(new_buffer, 0, real_size);
            if (new_buffer != nullptr) {
                buffer_list.push_back(new_buffer);
            } else {
                std::cout << "malloc buffer error!" << std::endl;
                return nullptr;
            }
        }
        auto& rt = buffer_list.front();
        buffer_list.pop_front();
        std::cout << "allocate buffer, addr = " << rt << std::endl;
        return rt;
    }

    void Deallocate(void* buffer, size_t size) {
        size_t real_size = fixSize(size);
        auto _buffer_list = m_buffer_pool.find(real_size);
        if (_buffer_list == m_buffer_pool.end()) {
            m_buffer_pool[real_size] = std::list<void*>();
        }

        auto& buffer_list = m_buffer_pool[real_size];

        if (buffer_list.size() > 10) {
            free(buffer);
            return;
        }
        buffer_list.push_back(buffer);
        std::cout << "Deallocate buffer, addr = " << buffer << std::endl;
    }

private:
    size_t fixSize(const size_t size) {
        static const size_t base_size = 10 * 1024;

        size_t real_size = size;
        size_t spend = size % base_size;   // 8×Ö½Ú¶ÔÆë
        if (spend) {
            real_size = size + base_size - spend;
        }
        return real_size;
    }

    size_t getFileSize(size_t size) {
        return size + 1;
    }
private:
    std::map<size_t, std::list<void*>>  m_buffer_pool;

};
