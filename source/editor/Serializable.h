#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H

#include <string>
#include <sstream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>


/**
    \class Serializable

    Simplistic serialization using boost::property_tree.
    Prodivdes a name property.
*/
class Serializable
{
public:
    typedef boost::property_tree::ptree PropertyTree;

    Serializable()
    {
        setName( getDefaultName() );
    }

    ///@name Interface
    ///@{
    virtual PropertyTree& serialize() const = 0;
    virtual void deserialize( PropertyTree& pt ) = 0;
    ///@}

    ///@name Default serialization to/from disk via XML
    ///@{
    void serializeToDisk( std::string filename )
    {
        PropertyTree pt = serialize();
        boost::property_tree::xml_writer_settings<std::string> settings(' ', 4);
        write_xml( filename, pt, std::locale("en_US.UTF-8"), settings);
    }   
    bool deserializeFromDisk( std::string filename )
    {
        PropertyTree pt;
        read_xml( filename, pt, 0, std::locale("en_US.UTF-8"));
        deserialize( pt );
        return true; // FIXME: Implement some error-checking!
    }
    ///@}

    ///@{ Access name
    std::string getName() const { return m_name; }
    void setName( const std::string& name ) { m_name = name; }
    virtual std::string getDefaultName()
    {
        static int count=0;
        std::stringstream ss;
        ss << "unnamed" << count++;
        return ss.str();
    }
    ///@}

private:
    std::string m_name;
};

#endif // SERIALIZABLE_H
