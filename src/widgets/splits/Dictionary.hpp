#pragma once
// ZACHARY

#include <QString>
#include <vector>
#include <string>

namespace chatterino 
{

struct BKTreeNode
{
    QString word_;

    int distanceFromParent_{};

    std::vector<BKTreeNode> children_;

    BKTreeNode(const QString &word, int distance = -1) 
		: word_(word), distanceFromParent_(distance) {

	};
};


class Dictionary 
{
public:
    static Dictionary &instance();

    bool isMisspelled(const QString &word);

    void insertWordsFromFile(const std::string &path);
private:
    Dictionary();

    std::vector<int> checkNodeDistances(const QString &word, int tolerance, BKTreeNode &node);

    void insertWordIntoTree(const QString &word, BKTreeNode &node);

    int calculateDistance(const QString &word1, const QString &word2);

    static Dictionary instance_;

    BKTreeNode root_;

    bool hasBeenCalled_ = false;
};

} // namespace chatterino 