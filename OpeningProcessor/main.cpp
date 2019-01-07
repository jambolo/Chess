#include <nlohmann/json.hpp>

#include <fstream>
#include <iostream>
#include <sstream>

using json     = nlohmann::json;

using Move = std::string;
using MoveList = std::vector<Move>;

void to_json(json & j, Move const & m)
{
    j = m; // haven't figured this out yet
}

static MoveList extractMoves(std::string const & moves)
{
    std::istringstream movestream(moves);
    return MoveList{ std::istream_iterator<std::string> { movestream }, std::istream_iterator<std::string> {} };
}

struct Opening
{
    std::string c;  // ECO code
    std::string n;  // Opening name
    std::string f;  // Final state (FEN)
    int id;         // ID
    MoveList m;     // sequence of moves
};

static void from_json(json const & j, Opening & opening)
{
    j.at("c").get_to(opening.c);
    j.at("n").get_to(opening.n);
    j.at("f").get_to(opening.f);
    j.at("id").get_to(opening.id);
    opening.m = extractMoves(j["m"].get<std::string>());
}

using NodeList = std::vector<struct Node *>;
struct Node
{
    Move move;         // Move that creates this node from the parent
    std::string code;  // ECO code that includes this line
    std::string name;  // Name of this opening (until changed by a child)
    int id;            // ID of this opening
    NodeList children; // List of follow-up moves
};

static void to_json(json & j, Node const & n)
{
    j = json{
        { "move", n.move },
        { "code", n.code },
        { "name", n.name },
        { "id", n.id },
        { "children", json::array() }
    };
    for (auto const & c : n.children)
    {
        j["children"].push_back(*c);
    }
}

Node root;
int main(int argc, char ** argv)
{
    if (argc != 2)
        exit(1);

    std::ifstream in(argv[1]);
    if (!in.is_open())
        exit(2);

    json eco;
    in >> eco;

    std::vector<Opening> openings;
    for (auto const & opening : eco)
    {
        openings.push_back(opening);
    }

    // Sort the openings by the number of moves. This ensures that the variations don't override basic openings.
    std::sort(openings.begin(), openings.end(), [] (Opening const & a, Opening const & b) { return a.m.size() < b.m.size(); });

    for (auto const & o : openings)
    {
        Node * current = &root;
        for (auto const & m : o.m)
        {
            // find m in the current node's list of children
            NodeList::iterator found = std::find_if(current->children.begin(),
                                                    current->children.end(),
                                                    [&m] (Node * a) { return a->move == m; }
                                       );

            // If found, then the found node becomes the current node. Otherwise, a new child node is created and it becomes the
            // current node.
            if (found != current->children.end())
            {
                current = *found;
            }
            else
            {
                Node * child = new Node{ m, o.c, o.n, o.id };
                current->children.push_back(child);
                current = child;
            }
        }
    }

    json out;
    for (auto const & n : root.children)
    {
        out.push_back(*n);
    }

    std::cout << out.dump(2);

    return 0;
}
