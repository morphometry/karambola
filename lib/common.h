#ifndef COMMON_INCLUDED
#define COMMON_INCLUDED


inline bool is_nan(double x){
    return x!=x;
}

struct noncopyable {
    public:
        noncopyable () {}
    private:
        noncopyable (const noncopyable &);
        void operator= (const noncopyable &);
};


#endif // COMMON_INCLUDED
