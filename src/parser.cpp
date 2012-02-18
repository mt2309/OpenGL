#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <string>
// #include <iostream>
// #include <fstream>
// #include <istream>
#include "cgRender.h"
#include "parser.h"


    using namespace std;
    using namespace boost;
    
    parser::parser(string s)
    {
        vtk = parse_vtk_file(s);
    }
    
    vtk_file* parser::parse_vtk_file(string s)
    {

        
        // Allocate memory for vtk_file struct and vectors
        
        vtk_file* input = (vtk_file*)malloc(sizeof(struct vtk_file));
        vector<float>* points = new vector<float>;
        vector<int>* polygons = new vector<int>;
        vector<float>* texture = new vector<float>;

        
        // Set up tokenizer
        char_separator<char> sep(" \n");
        tokenizer<char_separator<char> > tok(s,sep);
        tokenizer<char_separator<char> >::iterator beg=tok.begin();		

        // Move to start of point data
        while ((*beg) != "POINTS")
        {
            beg++;
        }


        beg++;


        // Get number of points
        int point_count = lexical_cast<int>(*beg++);

        beg++;

        // While the token is not polygons, add points to vector
        cout << "Parsing point data" << endl;
        while(*beg != "POLYGONS")
        {
            points->push_back(lexical_cast<float>(*beg++));
        }
        cout << points->size() << endl;

        beg++;
        
        // Get polygon data
        int polygon_no = lexical_cast<int>(*beg);
        beg++;
        int polygon_size = lexical_cast<int>(*beg);
        beg++;

        // While the token is not point_data, add polygons to vector
        cout << "Parsing polygon data" << endl;
        while (*beg != "POINT_DATA")
        {
            polygons->push_back(lexical_cast<int>(*beg++));
        }

        // Move iterator into position
        beg++;
        beg++;
        beg++;
        beg++;
        beg++;
        beg++;
        
        //While the token is not at the end, add texture data to vector
        cout << "Parsing texture data" << endl;
        while (beg != tok.end())
        {
            texture->push_back(lexical_cast<float>(*beg++));
        }
        
        // Set up vtk struct
        input->point_count = point_count;
        input->polygon_size = polygon_size;
        input->polygon_no = polygon_no;

        input->points = points;
        input->polygons = polygons;
        input->point_data = texture;
        
        cout << "Parsing complete" << endl;

        return input;
    }