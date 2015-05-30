#ifndef PRETTYPRINTER_H_INCLUDED
#define PRETTYPRINTER_H_INCLUDED

/*
    Originally made by Stephan T. Lavavej
    Can be found here https://onedrive.live.com/?cid=E66E02DC83EFB165&id=e66e02dc83efb165!293
    Partially changed to fit the rest of lib
*/

#include <iterator>
#include <map>
#include <ostream>
#include <set>
#include <string>
#include <tuple>
#include <utility>
#include <vector>
#include <forward_list>

class PrettyPrinter
{
private:
    template <typename T> struct is_container_helper
    {
        template <typename U> static std::true_type f(typename U::const_iterator*);
        template <typename U> static std::false_type f(...);

        typedef decltype(f<T>(0)) type;
    };

    template <typename T>
    struct is_container : public is_container_helper<T>::type { };

    template <typename T, size_t N>
    struct is_container<T[N]> : public std::true_type { };

    template <typename Ch, typename Tr, typename Al>
    struct is_container<std::basic_string<Ch, Tr, Al>> : public std::false_type { }; //so strings are not considered as array of chars

public:
    struct DefaultFormatter
    {
        template <typename T> void    prefix(std::ostream& os, const T&) const { os << "["; }
        template <typename T> void separator(std::ostream& os, const T&) const { os << ", "; }
        template <typename T> void    suffix(std::ostream& os, const T&) const { os << "]"; }

        template <typename A, typename B>
        void    prefix(std::ostream& os, const std::pair<A, B>&) const { os << "("; }
        template <typename A, typename B>
        void separator(std::ostream& os, const std::pair<A, B>&) const { os << ", "; }
        template <typename A, typename B>
        void    suffix(std::ostream& os, const std::pair<A, B>&) const { os << ")"; }

        template <typename... Types>
        void    prefix(std::ostream& os, const std::tuple<Types...>&) const { os << "("; }
        template <typename... Types>
        void separator(std::ostream& os, const std::tuple<Types...>&) const { os << ", "; }
        template <typename... Types>
        void    suffix(std::ostream& os, const std::tuple<Types...>&) const { os << ")"; }

        template <typename K, typename C, typename A>
        void    prefix(std::ostream& os, const std::set<K, C, A>&) const { os << "{"; }
        template <typename K, typename C, typename A>
        void separator(std::ostream& os, const std::set<K, C, A>&) const { os << ", "; }
        template <typename K, typename C, typename A>
        void    suffix(std::ostream& os, const std::set<K, C, A>&) const { os << "}"; }

        template <typename T> void element(std::ostream& os, const T& t) const
        {
            os << t;
        }

        template <typename Ch, typename Tr, typename Al>
        void element(std::ostream& os, const std::basic_string<Ch, Tr, Al>& s) const
        {
            os << "\"" << s << "\"";
        }
    };

    template <typename T>
    static void print(std::ostream& os, const T& t)
    {
        print(os, t, DefaultFormatter());
    }


    template <typename T, typename Fmt>
    static void print(std::ostream& os, const T& t, const Fmt& fmt)
    {
        print_container_helper(os, t, typename is_container<T>::type(), fmt);
    }

    template <typename A, typename B, typename Fmt>
    static void print(std::ostream& os, const std::pair<A, B>& p, const Fmt& fmt)
    {
        fmt.prefix(os, p);
        print(os, p.first, fmt);
        fmt.separator(os, p);
        print(os, p.second, fmt);
        fmt.suffix(os, p);
    }

    template <typename... Types, typename Fmt>
    static void print(std::ostream& os, const std::tuple<Types...>& t, const Fmt& fmt)
    {
        const size_t N = std::tuple_size<std::tuple<Types...>>::value;
        fmt.prefix(os, t);
        print_tuple_helper(os, t, fmt, std::integral_constant<size_t, N>());
        fmt.suffix(os, t);
    }

    template <typename T>
    static void printLine(std::ostream& os, const T& t)
    {
        print(os, t);
        os << '\n';
    }

    template <typename T, typename Fmt>
    static void printLine(std::ostream& os, const T& t, const Fmt& fmt)
    {
        print(os, t, fmt);
        os << '\n';
    }
private:
    template <typename Tuple, typename Fmt, size_t I>
    static void print_tuple_helper(std::ostream& os, const Tuple& t, const Fmt& fmt, std::integral_constant<size_t, I>)
    {
        const size_t N = std::tuple_size<Tuple>::value;
        print(os, std::get < N - I > (t), fmt);
        fmt.separator(os, t);
        print_tuple_helper(os, t, fmt, std::integral_constant < size_t, I - 1 > ());
    }

    template <typename Tuple, typename Fmt>
    static void print_tuple_helper(std::ostream& os, const Tuple& t, const Fmt& fmt, std::integral_constant<size_t, 1>)
    {
        const size_t N = std::tuple_size<Tuple>::value;
        print(os, std::get < N - 1 > (t), fmt);
    }

    template <typename Tuple, typename Fmt>
    static void print_tuple_helper(std::ostream&, const Tuple&, const Fmt&, std::integral_constant<size_t, 0>) { }


    template <typename C, typename Fmt>
    static void print_container_helper(std::ostream& os, const C& c, std::true_type, const Fmt& fmt)
    {
        fmt.prefix(os, c);

        auto i = std::begin(c);
        auto e = std::end(c);

        if(i != e)
        {
            for(;;)
            {
                print(os, *i, fmt);

                if(++i != e) fmt.separator(os, c);
                else break;
            }
        }

        fmt.suffix(os, c);
    }

    template <typename T, typename Fmt>
    static void print_container_helper(std::ostream& os, const T& t, std::false_type, const Fmt& fmt)
    {
        fmt.element(os, t);
    }
#ifdef PrettyPrinterTest
    /// Test written originally by Stephan T. Lavavej
    /// Shows examples how to use printing and custom formatters

    struct TESTSpecialFormatter
    {
        template <typename T> void    prefix(std::ostream& os, const T& t) const
        {
            PrettyPrinter::DefaultFormatter().prefix(os, t);
        }
        template <typename T> void separator(std::ostream& os, const T& t) const
        {
            PrettyPrinter::DefaultFormatter().separator(os, t);
        }
        template <typename T> void    suffix(std::ostream& os, const T& t) const
        {
            PrettyPrinter::DefaultFormatter().suffix(os, t);
        }
        template <typename T> void   element(std::ostream& os, const T& t) const
        {
            PrettyPrinter::DefaultFormatter().element(os, t);
        }

        template <typename K, typename C, typename A>
        void prefix(std::ostream& os, const std::set<K, C, A>& s) const
        {
            os << "[" << s.size() << "]{";
        }

        template <typename T, typename A>
        void    prefix(std::ostream& os, const std::forward_list<T, A>&) const { os << "<"; }
        template <typename T, typename A>
        void separator(std::ostream& os, const std::forward_list<T, A>&) const { os << "->"; }
        template <typename T, typename A>
        void    suffix(std::ostream& os, const std::forward_list<T, A>&) const { os << ">"; }

        template <typename Ch, typename Tr, typename Al>
        void element(std::ostream& os, const std::basic_string<Ch, Tr, Al>& s) const
        {
            os << s;
        }
    };
public:
    static void test()
    {
        std::cout << "Empty vector: ";
        PrettyPrinter::printLine(std::cout, std::vector<int>());

        std::cout << "Empty    set: ";
        PrettyPrinter::printLine(std::cout, std::set<int>());

        std::cout << "Empty  tuple: ";
        PrettyPrinter::printLine(std::cout, std::tuple<>());

        std::cout << "One-element vector: ";
        PrettyPrinter::printLine(std::cout, std::vector<int>(1, 1701));

        {
            std::cout << "One-element    set: ";
            std::set<int> s;
            s.insert(1729);
            PrettyPrinter::printLine(std::cout, s);
        }

        {
            std::cout << "One-element  array: ";
            const int a[] = { 2048 };
            PrettyPrinter::printLine(std::cout, a);
        }

        std::cout << "One-element  tuple: ";
        PrettyPrinter::printLine(std::cout, std::tuple<int>(4096));

        {
            std::cout << "Multi-element vector: ";
            std::vector<int> v;
            v.push_back(11);
            v.push_back(22);
            v.push_back(33);
            PrettyPrinter::printLine(std::cout, v);
        }

        {
            std::cout << "Multi-element    set: ";
            std::set<int> s;
            s.insert(111);
            s.insert(777);
            s.insert(222);
            s.insert(999);
            PrettyPrinter::printLine(std::cout, s);
        }

        {
            std::cout << "Multi-element  array: ";
            const int a[] = { 100, 200, 300, 400, 500 };
            PrettyPrinter::printLine(std::cout, a);
        }

        std::cout << "  Two-element   pair: ";
        PrettyPrinter::printLine(std::cout, std::make_pair(123, 456));

        std::cout << "Multi-element  tuple: ";
        PrettyPrinter::printLine(std::cout, std::make_tuple(10, 20, 30, 40));

        std::cout << "          Empty string: ";
        PrettyPrinter::printLine(std::cout, std::string(""));

        std::cout << "  One-character string: ";
        PrettyPrinter::printLine(std::cout, std::string("x"));

        std::cout << "Multi-character string: ";
        PrettyPrinter::printLine(std::cout, std::string("meow"));

        std::cout << "--" << std::endl;

        {
            std::cout << "vector<string>: ";
            std::vector<std::string> v;
            v.push_back("cute");
            v.push_back("fluffy");
            v.push_back("kittens");
            PrettyPrinter::printLine(std::cout, v);
        }

        {
            std::cout << "vector<vector<int>>: ";
            std::vector<std::vector<int>> v;
            for(int i = 0; i < 3; ++i)
            {
                std::vector<int> temp;

                for(int j = 0; j < 4; ++j)
                {
                    temp.push_back((i + 1) * 10 + j);
                }

                v.push_back(temp);
            }
            PrettyPrinter::printLine(std::cout, v);

            std::cout << "map<string, vector<int>>: ";
            std::map<std::string, std::vector<int>> m;
            m["abc"] = v[0];
            m["def"] = v[1];
            m["ghi"] = v[2];
            PrettyPrinter::printLine(std::cout, m);
        }

        {
            std::cout << "Multi-dimensional array: ";
            const int aa[3][5] =
            {
                { 71, 72, 73, 74, 75 },
                { 81, 82, 83, 84, 85 },
                { 91, 92, 93, 94, 95 }
            };
            PrettyPrinter::printLine(std::cout, aa);
        }

        {
            std::cout << "vector<tuple<int, string, int>>: ";
            std::vector<std::tuple<int, std::string, int>> v;
            v.push_back(std::make_tuple(1, "ten", 100));
            v.push_back(std::make_tuple(2, "twenty", 200));
            v.push_back(std::make_tuple(3, "thirty", 300));
            PrettyPrinter::printLine(std::cout, v);
        }

        std::cout << std::endl << "*** TESTSpecialFormatter: ***" << std::endl;

        {
            std::vector<std::set<std::string>> v(3);
            v[0].insert("the");
            v[0].insert("wrath");
            v[0].insert("of");
            v[0].insert("khan");
            v[1].insert("the");
            v[1].insert("voyage");
            v[1].insert("home");
            v[2].insert("the");
            v[2].insert("undiscovered");
            v[2].insert("country");
            PrettyPrinter::printLine(std::cout, v, TESTSpecialFormatter());
        }

        {
            std::set<std::pair<int, int>> s;
            s.insert(std::make_pair(11, 22));
            s.insert(std::make_pair(33, 44));
            s.insert(std::make_pair(55, 66));
            PrettyPrinter::printLine(std::cout, s, TESTSpecialFormatter());
        }

        {
            std::forward_list<int> fl;
            fl.push_front(123);
            fl.push_front(456);
            fl.push_front(789);
            PrettyPrinter::printLine(std::cout, fl, TESTSpecialFormatter());
        }
    }
#endif // PrettyPrinterTest
};

#endif // PRETTYPRINTER_H_INCLUDED
