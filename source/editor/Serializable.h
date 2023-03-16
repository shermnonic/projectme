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

    /// Return filepath relative to current working directory.
    /// The latter should be already set to the directory of the associated
    /// project file beforehand.
    /// @TODO: Refactor from string filenames to filesystem::path
    static std::string getRelativePath(std::string filename);

    Serializable();

    ///@name Interface
    ///@{
    virtual PropertyTree& serialize() const = 0;
    virtual void deserialize( PropertyTree& pt ) = 0;
    ///@}

    ///@name Default serialization to/from disk via XML
    ///@{
    void serializeToDisk( std::string filename ) const;
    bool deserializeFromDisk( std::string filename );
    ///@}

    ///@{ Access name
    std::string getName() const;
    void setName( const std::string& name );
    virtual std::string getDefaultName();
    ///@}

private:
    std::string m_name;
};

#endif // SERIALIZABLE_H
