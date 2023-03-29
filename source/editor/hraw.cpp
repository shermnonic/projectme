#include "hraw.h"

#include <cstdint>
#include <fstream>
#include <iostream>

using std::ifstream;
using std::ofstream;
using std::cerr;
using std::endl;


bool read_hraw( const char* filename, float*& data, unsigned* size )
{   
    data = nullptr;
    
    // Open file
    ifstream f( filename, ifstream::binary );
    if( !f.is_open() )
    {
        cerr << "Error in read_hraw():"
                "Could not open file \"" << filename << "\"!" << endl;
        return false;
    }
    
    // Read header <dims size[0] ... size[dims]>
    uint32_t dims;
    f.read( (char*)&dims, sizeof(uint32_t) );
    
    if( dims<=0 || dims > 3 )
    {
        f.close();
        cerr << "Error in read_hraw():"
                "Invalid dimensionality (dims=" << dims << ")!" << endl;
        return false;
    }
    
    uint32_t size_[3] = { 0, 1, 1 };
    f.read( (char*)size_, dims*sizeof(uint32_t) );
    
    size[0] = size_[0];
    size[1] = size_[1];
    size[2] = size_[2]; 
    
    // Read data
    unsigned n = size[0]*size[1]*size[2];
    data = new float[ n ];
    f.read( reinterpret_cast<char*>(data), n*sizeof(float) );
    
    if( !f )
    {
        cerr << "Error in read_hraw():"
                "Only " << f.gcount()/sizeof(float) << " / " << n/sizeof(float) 
                << " floats could be read from \"" << filename << "\"!" << endl;
    }
    
    f.close();
    return true;
}

void write_hraw( const char* filename, const float* data, 
                 unsigned sizeX, unsigned sizeY, unsigned sizeZ )
{
    unsigned size[3];
    size[0] = sizeX;
    size[1] = sizeY;
    size[2] = sizeZ;
    write_hraw( filename, data, size );
}


void write_hraw( const char* filename, const float* data, const unsigned* size )
{
    // Open file
    ofstream f( filename, ofstream::binary );
    if( !f.is_open() )
    {
        cerr << "Error in write_hraw():"
                "Could not open file \"" << filename << "\"!" << endl;
        return;
    }
    
    // Write header
    uint32_t dims = (size[0]>1?1:0) + (size[1]>1?1:0) + (size[2]>1?1:0);
    uint32_t size_[3] = { size[0], size[1], size[2] };
    
    f.write( reinterpret_cast<char*>(&dims), sizeof(uint32_t)*3 );
    f.write( reinterpret_cast<char*>(size_), sizeof(uint32_t)*3 );
    
    // Write data
    unsigned n = size[0]*size[1]*size[2];
    f.write( reinterpret_cast<const char*>(data), sizeof(float)*n );
    
    f.close();
}
