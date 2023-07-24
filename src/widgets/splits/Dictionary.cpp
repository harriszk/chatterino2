#include "Dictionary.hpp"
#include "common/QLogging.hpp"
#include <queue>
#include <fstream>

namespace chatterino 
{

Dictionary Dictionary::instance_;

Dictionary::Dictionary()
    : root_("")
{
    this->insertWordsFromFile("resources/dictionaries/usa_english_words.txt");
}

Dictionary &Dictionary::instance()
{
    return instance_;
}

bool Dictionary::isMisspelled(const QString &word)
{
    std::vector<int> numbers = this->checkNodeDistances(word, 2, this->root_);
    
    if (std::find(numbers.begin(), numbers.end(), 0) != numbers.end()) {
        return false;
    }
    
    return true;
}

std::vector<int> Dictionary::checkNodeDistances(const QString &word, int tolerance, BKTreeNode &node)
{
    // Go through all children with distance between [distance_to_root - tolerance, distance_to_root + tolerance]
    // If distance is less than or equal to tolerance then add it to the nodes vector.
    std::vector<int> distances;

    int distanceToNode = calculateDistance(word, node.word_);

    if (distanceToNode <= tolerance)
    {
        distances.push_back(distanceToNode);
    }

    int minimum = distanceToNode - tolerance;
    int maximum = distanceToNode + tolerance;

    for (BKTreeNode &child : node.children_)
    {
        if (child.distanceFromParent_ >= minimum && child.distanceFromParent_ <= maximum)
        {
            std::vector<int> childDistances = this->checkNodeDistances(word, tolerance, child);
            distances.insert(distances.end(), childDistances.begin(), childDistances.end());
        }
    }

    return distances;
}


void Dictionary::insertWordsFromFile(const std::string &path)
{
    if(hasBeenCalled_) 
    {
        return;
    }

    hasBeenCalled_ = true;

    qCDebug(chatterinoWidget) << "Attempting to insert words into dictionary...";
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cerr << "Error opening file: " << path << std::endl;
        return;
    }

    std::string line;
    QString word;
    while (std::getline(file, line))
    {
        word = QString::fromStdString(line);

        // Add the word to the dictionary (assuming it is not empty)
        if (word.isEmpty())
        {
            continue;
        }

        if(this->root_.word_.isEmpty())
        {
            this->root_.word_ = word;
            continue;
        }

        this->insertWordIntoTree(word, this->root_);
    }

    file.close();
    
    qCDebug(chatterinoWidget) << "Finished loading words into dictionary...";
}

void Dictionary::insertWordIntoTree(const QString &word, BKTreeNode &node)
{
    int distance = calculateDistance(word, node.word_);
    bool shouldInsert = true;

    for (BKTreeNode &child : node.children_) { 
        if (child.distanceFromParent_ == distance) {
            shouldInsert = false;
            this->insertWordIntoTree(word, child);
            break;
        }
    }

    if (shouldInsert) {
        BKTreeNode newNode(word, distance);
        node.children_.push_back(newNode);
    }
}

// Implementation of the Levenshtein distance metric using two row implementation.
int Dictionary::calculateDistance(const QString &word1, const QString &word2)
{
    int m = word1.length();
    int n = word2.length();

    std::vector<int> v0(n + 1);
    std::vector<int> v1(n + 1);

    for (int i = 0; i <= n; ++i) 
    {
        v0[i] = i;
    }

    for (int i = 0; i < m; ++i) 
    {
        v1[0] = i + 1;

        for (int j = 0; j < n; ++j) 
        {
            int deletionCost = v0[j + 1] + 1;
            int insertionCost = v1[j] + 1;
            int substitutionCost = (word1[i] == word2[j]) ? v0[j] : v0[j] + 1;

            v1[j + 1] = std::min({deletionCost, insertionCost, substitutionCost});
        }

        v0.swap(v1);
    }

    return v0[n];
}

} // namespace chatterino 