// vim: et:sw=4:ts=4
// read some data out of .poly files
//#define DEBUG_POLY_READER

#include "readpoly.h"
#include <stdexcept>
#include <iostream>
#include <stdarg.h>
#include <limits>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

namespace {

    static const char CARRIAGE_RETURN = '\xd';

    static
    void throw_error (const std::string &message, ...) {
        char buffer[200];
        va_list va;
        va_start (va, message);
        vsnprintf (buffer, 199, message.c_str (), va);
        throw std::runtime_error (buffer);
    }

    static
    long string_to_int (std::string value, int base = 10) {
        const char *beginptr = value.c_str ();
        char *endptr = 0;
        errno = 0;
        long ret = strtol (beginptr, &endptr, base);
        if (endptr != beginptr + value.size ())
            throw_error ("value " + value + " is not a valid integer");
        if (errno == ERANGE)
            throw_error ("value " + value + " cannot be represented in a long");
        if (errno != 0)
            throw_error ("value " + value + " is not a valid integer");
        return ret;
    }

    static
    std::string int_to_string (long x)
    {
        char buffer[40];
        snprintf (buffer, 40, "%li", x);
        return buffer;
    }

    // isdigit independant of locale
    static
    bool is_digit (int c) {
        switch (c) {
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
                return true;
            default:
                return false;
        }
    }

    static
    std::istream &extract_ws_but_no_newline (std::istream &is)
    {
        if (!is)
            // stream is in error state already, do nothing
            return is;
        while (true) {
            int c;
            c = is.peek ();
            switch (c)
            {
            case ' ':
            case '\t':
                is.get ();
                break;
            default:
                return is;
            }
        }
    }

    static
    std::istream &ignore_rest_of_line (std::istream &is)
    {
        is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return is;
    }

    static
    std::istream &whitespace_including_comments (std::istream &is)
    {
        if (!is)
            // stream is in error state already, do nothing
            return is;
        while (true) {
            int c;
            c = is.peek ();
            switch (c)
            {
            case ' ':
            case '\n':
            case '\r':
            case '\t':
                is.get ();
                break;
            case '#':
                is >> ignore_rest_of_line;
                break;
            default:
                return is;
            }
        }
    }
}

// extract label stored in the alpha component of the color attribute
long PolyFileSink::PropertyList::label_from_alpha_value () const
{
    std::string theattr = get_attr_ ("c");
    std::string::size_type x = theattr.rfind (',');
    if (x == theattr.npos)
        throw_error ("Malformed color attribute (1) " + get_attr_ ("c") + " for " + object_id);
    theattr = theattr.substr (x+1);
    if (theattr.size () == 0u)
        throw_error ("Malformed color attribute " + get_attr_ ("c") + " for " + object_id);
    theattr.erase (theattr.end () - 1);
#ifdef DEBUG_POLY_READER
    std::cerr << "[PolyFileSink::PropertyList::label_from_alpha_value] extracted "
                 "label from alpha value: "
              << string_to_int (theattr) << std::endl;
#endif
    return string_to_int (theattr);
}

bool PolyFileSink::PropertyList::is_closed () const
{
    return find_attr_ ("<") != 0;
}

// find attribute beginning with 'name', or throw an error
const std::string &PolyFileSink::PropertyList::get_attr_ (std::string name) const
{
    const std::string *p = find_attr_ (name + "(");
    if (!p)
        throw_error ("no attribute " + name + " for " + object_id);
    return *p;
}

// find attribute beginning with 'name', or return a NULL pointer
const std::string *PolyFileSink::PropertyList::find_attr_ (std::string name) const
{
    const_iterator it = begin ();
    for (; it != end (); ++it)
        if (it->substr (0u, name.size ()) == name)
            return &*it;
    return 0;
}

void PolyFileSink::reexamine_vertex (vertex_id_t, double *) const
{
    throw_error ("PolyFileSink::reexamine_vertex: you need to override the "
        "reexamine_vertex function in your PolyFileSink adapter class in order to "
        "use more advanced triangulation methods.");
}

namespace {
    // object keeping the state during the process of reading a .poly file
    struct PolyFileReader {
        PolyFileReader (std::istream &is_, PolyFileSink *sink_)
            : is (is_), sink (sink_)
        {
            num_vertices = 0;
            num_facets = 0;
            if (!is) {
                throw_error ("Stream is not readable.  Does the input file exist?");
            }
        }

        std::istream &is;
        PolyFileSink *sink;
        // mapping .poly vertex indices -> target vertex indices
        std::vector <int> vertex_map;
        size_t num_vertices, num_facets;
        typedef PolyFileSink::prop_list_t prop_list_t;

        static std::string printable_int (int c) {
            if (c == EOF)
                return "EOF";
            else
                return std::string (1, char (c));
        }

        std::string drop_dos_carriage_return (std::string &str)
        {
            if (str.size () && str[str.size () - 1] == CARRIAGE_RETURN)
                return str.substr (0, str.size () - 1);
            return str;
        }

        void extract_header (const std::string &tag) {
            std::string tmp;
            std::getline (is, tmp);
            tmp = drop_dos_carriage_return (tmp);
            if (tmp != tag) {
                throw_error ("format error in .poly file: expected %s, got %s",
                    tag.c_str (), tmp.c_str ());
            }
        }

        int lookup_vertex (int v_id) {
            if ((int)vertex_map.size () > v_id && v_id >= 0) {
                int ret = vertex_map[v_id];
                if (ret >= 0)
                    return ret;
            }
            throw_error ("file refers to vertex %i which I do not know (vertex_map size = %i)",
                         v_id, (int)vertex_map.size ());
            abort ();  // silence warning
        }

        int extract_identifier ()
        {
            assert (is);
            int number;
            is >> number >> std::ws;
            int colon = is.get ();
            if (colon != ':')
                throw_error ("Expected colon, got %s.", printable_int (colon).c_str ());
            is >> std::ws;
            assert (number >= 0);
            return number;
        }

        // extract a .poly property form istream and store it in *out
        // return true if terminated by whitespace
        // return false if terminated by end of line
        // handles parentheses in properties.
        // multiple spaces in sequence between attributes are not handled correctly
        bool extract_property (std::string *out)
        {
            int paren = 0;
            assert (out);
            *out = "";
            for (;;)
            {
                int x = is.get ();
                if (!is)
                    throw_error ("unexpected end of file");
                switch (x)
                {
                case '(':
                    paren++;
                    break;
                case ')':
                    paren--;
                    break;
                case ' ':
                case '\t':
                    if (!paren)
                        return true;
                    break;
                case '\n':
                    if (paren)
                        throw_error ("unbalanced parentheses in .poly");
                    return false;
                case CARRIAGE_RETURN:
                    continue;
                default:
                    ;
                }
                out->push_back (char (x));
            }
        }

        void extract_all_properties (prop_list_t *out)
        {
            std::string prop;
            while (extract_property (&prop))
                out->push_back (prop);
            out->push_back (prop);
#ifdef DEBUG_POLY_READER
            prop_list_t::iterator it;
            for (it = out->begin (); it != out->end (); ++it)
                std::cerr << "[" << *it << "] ";
            std::cerr << "\n";
#endif
        }

        void read_vertex () {
            const int number = extract_identifier ();

            // coords
            double x, y, z;
            is >> x >> y >> z >> extract_ws_but_no_newline;
            if (!is)
                throw_error ("Cannot read coordinates for vertex " + int_to_string (number) + ".");

            prop_list_t properties;
            extract_all_properties (&properties);
            properties.object_id = "vertex " + int_to_string (number);
            properties.object_number = number;

            int vert_id = sink->insert_vertex (x, y, z, properties);

            // saved for later reference
            if ((int)vertex_map.size () <= number) {
                vertex_map.resize (number+1, -1);
            }
            vertex_map[number] = vert_id;
            ++num_vertices;
        }

        void read_facet () {

            const int number = extract_identifier ();

            PolyFileSink::vertex_id_list_t vl;
            vl.reserve (10);
            while (is_digit (is.peek ())) {
                int v;
                is >> v >> extract_ws_but_no_newline;
                v = lookup_vertex (v);
                vl.push_back (v);
            }

            prop_list_t properties;
            extract_all_properties (&properties);
            properties.object_id = "facet " + int_to_string (number);
            properties.object_number = number;

            sink->insert_facet (vl, properties);
            ++num_facets;
        }

        void read_file () {
            extract_header ("POINTS");
            while (is_digit (is.peek ()))
                read_vertex ();
#ifdef DEBUG_POLY_READER
            std::cerr << "[PolyFileReader::read_file] read " << num_vertices << " vertices\n";
#endif
            extract_header ("POLYS");
            while (is_digit (is.peek ()))
                read_facet ();
#ifdef DEBUG_POLY_READER
            std::cerr << "[PolyFileReader::read_file] read " << num_facets << " facets\n";
#endif
            extract_header ("END");
            // rest of file is ignored.
        }
    };

}

void parse_poly_file (PolyFileSink *sink, std::istream &is) {
    PolyFileReader r (is, sink);
    r.read_file ();
}

namespace {
    // object keeping the state during the process of reading a .off file
    struct OffFileReader {
        OffFileReader (std::istream &is_, PolyFileSink *sink_)
            : is (is_), sink (sink_)
        {
            num_vertices = 0;
            num_facets = 0;
            expected_num_vertices = 0;
            expected_num_facets = 0;
            if (!is) {
                throw_error ("Stream is not readable.  Does the input file exist?");
            }
        }

        std::istream &is;
        PolyFileSink *sink;
        std::vector <int> vertex_map;
        size_t num_vertices, num_facets;
        size_t expected_num_vertices, expected_num_facets;
        typedef PolyFileSink::prop_list_t prop_list_t;

        static std::string printable_int (int c) {
            if (c == EOF)
                return "EOF";
            else
                return std::string (1, char (c));
        }

        std::string drop_dos_carriage_return (std::string &str)
        {
            if (str.size () && str[str.size () - 1] == CARRIAGE_RETURN)
                return str.substr (0, str.size () - 1);
            return str;
        }

        void extract_header (const std::string &tag) {
            std::string tmp;
            std::getline (is, tmp);
            tmp = drop_dos_carriage_return (tmp);
            if (tmp != tag) {
                throw_error ("format error in .off file: expected %s, got %s",
                    tag.c_str (), tmp.c_str ());
            }
        }

        void read_vertex () {
            double x, y, z;
            is >> x >> y >> z >> whitespace_including_comments;
            if (!is)
                throw_error ("Cannot read coordinates for vertex " + int_to_string (num_vertices) + ".");

            prop_list_t properties;
            properties.object_id = "vertex " + int_to_string (num_vertices);
            properties.object_number = num_vertices;

            int vert_id = sink->insert_vertex (x, y, z, properties);

            // saved for later reference
            if (vertex_map.size () <= num_vertices) {
                vertex_map.resize (num_vertices + 1, -1);
            }
            vertex_map[num_vertices] = vert_id;
            ++num_vertices;
        }

        int lookup_vertex (int v_id) {
            if ((int)vertex_map.size () > v_id && v_id >= 0) {
                int ret = vertex_map[v_id];
                if (ret >= 0)
                    return ret;
            }
            throw_error ("file refers to vertex %i which I do not know (vertex_map size = %i)",
                         v_id, (int)vertex_map.size ());
            abort ();  // silence warning
        }

        void read_facet () {
            PolyFileSink::vertex_id_list_t vl;
            vl.reserve (10);

            size_t num_vertices_in_this_facet;

            is >> num_vertices_in_this_facet >> extract_ws_but_no_newline;

            for (size_t i = 0; i != num_vertices_in_this_facet; ++i)
            {
                int v;
                is >> v >> extract_ws_but_no_newline;
                v = lookup_vertex (v);
                vl.push_back (v);
            }

            if (!is)
                throw_error ("Cannot read vertex ids for facet " + int_to_string (num_facets) + ".");

            prop_list_t properties;
            properties.object_id = "facet " + int_to_string (num_facets);
            properties.object_number = num_facets;

            // ignore facet color
            if (is.peek () != '\n' && is.peek () != '#')
            {
                double r, g, b;
                is >> r >> g >> b >> extract_ws_but_no_newline;
            }

            // extract facet label from alpha value
            if (is.peek () != '\n' && is.peek () != '#')
            {
                size_t a;
                is >> a >> extract_ws_but_no_newline;
                properties.push_back("c(0, 0, 0, " + int_to_string(a) + ")");
            }

            sink->insert_facet (vl, properties);
            ++num_facets;
        }

        void read_file () {
            extract_header ("OFF");
            is >> whitespace_including_comments;
            size_t expected_num_edges; // ignored
            is >> expected_num_vertices >> expected_num_facets >> expected_num_edges >> whitespace_including_comments;
#ifdef DEBUG_POLY_READER
            std::cerr << "[OffFileReader::read_file] about to read " << expected_num_vertices << " vertices and "
                << expected_num_facets << " facets\n";
#endif


            for (size_t i = 0; i != expected_num_vertices; ++i)
            {
                read_vertex ();
                is >> whitespace_including_comments;
            }

#ifdef DEBUG_POLY_READER
            std::cerr << "[OffFileReader::read_file] read " << num_vertices << " vertices\n";
#endif

            for (size_t i = 0; i != expected_num_facets; ++i)
            {
                read_facet ();
                is >> whitespace_including_comments;
            }

#ifdef DEBUG_POLY_READER
            std::cerr << "[OffFileReader::read_file] read " << num_facets << " facets\n";
#endif
            // rest of file is ignored.
        }
    };
}

void parse_off_file (PolyFileSink *sink, std::istream &is) {
    OffFileReader r (is, sink);
    r.read_file ();
}
