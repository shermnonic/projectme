#include "Serializable.h"
#include <filesystem>

namespace detail
{
    void set_cwd_to_directory_of_file(std::string filename)
    {
        auto filepath = std::filesystem::absolute(std::filesystem::path(filename)).parent_path();
        std::filesystem::current_path(filepath);
    }
}

std::string Serializable::getRelativePath(std::string filename)
{
    return std::filesystem::relative(std::filesystem::path(filename)).string();
}


Serializable::Serializable()
{
    setName(getDefaultName());
}

void Serializable::serializeToDisk(std::string filename) const
{
    detail::set_cwd_to_directory_of_file(filename);
    PropertyTree pt = serialize();
    boost::property_tree::xml_writer_settings<std::string> settings(' ',4);
    write_xml(filename,pt,std::locale("en_US.UTF-8"),settings);
}

bool Serializable::deserializeFromDisk(std::string filename)
{
    detail::set_cwd_to_directory_of_file(filename);
    PropertyTree pt;
    read_xml(filename,pt,0,std::locale("en_US.UTF-8"));
    deserialize(pt);
    return true; // FIXME: Implement some error-checking!
}


std::string Serializable::getName() const 
{ 
    return m_name; 
}

void Serializable::setName(const std::string& name) 
{ 
    m_name = name; 
}

std::string Serializable::getDefaultName()
{
    static int count=0;
    std::stringstream ss;
    ss << "unnamed" << count++;
    return ss.str();
}
