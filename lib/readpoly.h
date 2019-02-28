#ifndef READ_POLY_H_INCLUDED 
#define READ_POLY_H_INCLUDED 
    
#include <string>
#include <vector>
#include <istream>

class PolyFileSink {
public:
    typedef long vertex_id_t;
    typedef std::vector <vertex_id_t> vertex_id_list_t;
    struct PropertyList;
    typedef PropertyList prop_list_t;
    typedef std::string string_t;

    struct PropertyList : public std::vector <std::string>
    {
        bool is_closed () const;
        long label_from_alpha_value () const;

        bool is_attribute_present (string_t name) const;

        string_t object_id;

    private:
        const string_t &get_attr_ (string_t name) const;
        const string_t *find_attr_ (string_t name) const;
    };

    virtual ~PolyFileSink () {}
    virtual
     vertex_id_t insert_vertex (double, double, double,
                                const prop_list_t &) = 0;
    virtual void insert_facet  (const vertex_id_list_t &,
                                const prop_list_t &) = 0;
    virtual void reexamine_vertex (vertex_id_t, double *x) const;
};

class TriangulatingPolyFileSink : public PolyFileSink
{
public:
    TriangulatingPolyFileSink (PolyFileSink *sink)
    {
        sink_ = sink;
    }

    virtual vertex_id_t insert_vertex (double x, double y, double z,
                                       const prop_list_t &list)
    {
        return sink_->insert_vertex (x, y, z, list);
    }

    virtual void insert_facet (const vertex_id_list_t &vl,
                               const prop_list_t &list)
    {
        vertex_id_list_t temp (3);
        if (vl.size() > 3u)
        {
            temp.at (0) = vl.at (0);
            for (unsigned i = 1; i < (vl.size()-1); i++) {
                temp.at (1) = vl.at (i);
                temp.at (2) = vl.at (i+1);
                sink_->insert_facet (temp, list);
            }
        }
        else
        {
            temp.at (0) = vl.at (0);
            temp.at (1) = vl.at (1);
            temp.at (2) = vl.at (2);
            sink_->insert_facet (temp, list);
        }
    }

private:
    PolyFileSink *sink_;
};

class TriangleSink {
public:
    typedef PolyFileSink::prop_list_t prop_list_t;

    virtual ~TriangleSink () {}
    virtual void insert_triangle (const double *v0, const double *v1,
        const double *v2) = 0;
};

void parse_poly_file (PolyFileSink *sink, std::istream &);

// return an adapter class instance.
// both sinks have to be delete'd by the user.
PolyFileSink *poly_file_sink_from_triangle_sink (TriangleSink *);

#endif // READ_POLY_H_INCLUDED
