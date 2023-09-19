#include <iostream>
#include <fstream>
#include <cerrno>
#include <ranges>
#include <vector>
#include <string>
#include <string_view>
#include <iterator>
#include <algorithm>
#include <charconv>
#include <map>
#include <cstdint>

#include "Vector.hpp"

enum class Tocken : std::uint8_t
{
    None,
    Mtllib,
    Comment,
    Vertex,
    TexVertex,
    Normal,
    Surface,
    SmoothShading,
};

std::pair<Tocken, const char *> chech_element(const char *symbol, const char *end)
{
    Tocken pr{};

    switch (*symbol)
    {
    case '#':
        pr = Tocken::Comment;
        break;
    case 's':
        pr = Tocken::SmoothShading;
        break;
    case 'f':
        pr = Tocken::Surface;
        break;
    case 'v':
        symbol++;
        switch (*symbol)
        {
        case ' ':
            pr = Tocken::Vertex;
            break;
        case 't':
            pr = Tocken::TexVertex;
            break;
        case 'n':
            pr = Tocken::Normal;
            break;
        }
        break;
    }

    return std::make_pair(pr, ++symbol);
}

int main(int argc, char *args[])
{
    if (argc < 2)
    {
        std::cerr << "the file path is not specified" << std::endl;
        return -1;
    }

    std::ifstream fin(args[1], std::ios::binary | std::ios::in);
    if (!fin.is_open())
    {
        std::cerr << "file canot be open" << std::endl;
        return -1;
    }

    std::vector<char> raw_data;
    std::copy(std::istreambuf_iterator<char>(fin), {}, std::back_insert_iterator(raw_data));

    std::string_view data(raw_data.begin(), raw_data.end());
    Tocken pr;

    std::map<Tocken, std::size_t> dict{
        {Tocken::None, 0}};

    using vec3 = ta::Vector<float, 3>;
    std::vector<vec3> vertices;
    std::vector<vec3> normals;

    for (auto it = data.begin(); it < data.end(); ++it)
    {
        auto [primitive, wtail] = chech_element(it, data.end());
        it = wtail;
        if (it != data.end() && *it == '\n')
            continue;

        switch (primitive)
        {
        case Tocken::Vertex:
        {
            vec3 v;
            std::from_chars_result fc_res;
            fc_res = std::from_chars(it + 1, data.end(), v.x());
            it = fc_res.ptr;
            fc_res = std::from_chars(it + 1, data.end(), v.y());
            it = fc_res.ptr;
            fc_res = std::from_chars(it + 1, data.end(), v.z());
            it = fc_res.ptr;
            vertices.push_back(v);
        }
        break;
        case Tocken::Normal:
            /* code */
            break;
        case Tocken::Surface:
            /* code */
            break;
        case Tocken::SmoothShading:
            /* code */
            break;
        case Tocken::Comment:
        case Tocken::Mtllib:
        case Tocken::None:
            for (; it != data.end(); ++it)
                if (*it == '\n')
                    break;
            break;
        }
    }
}
