#ifndef PREDICTIONTRIE_H
#define PREDICTIONTRIE_H

#include <unordered_map>
#include <tuple>
#include <vector>
#include <string>

using MatchedPair = std::tuple<std::string, unsigned int>;


class PredictionTrie
{
    struct PredictionTrieNode
    {
        enum class Type
        {
            Root,
            Regular,
            Leaf,
        };

        Type type = Type::Regular;
        std::unordered_map<char, PredictionTrieNode*> children;
        unsigned int count = 0;
    };

    PredictionTrieNode* _root;

    const PredictionTrieNode* find(const std::string& word) const;

    void collectAllWordsStartedWith(
        const std::string& wordPart,
        const std::unordered_map<char, PredictionTrieNode*>& letterLayer,
        std::vector<MatchedPair>& result) const;

public:
    PredictionTrie();
    ~PredictionTrie();

    PredictionTrie(const PredictionTrie& other) = delete;
    PredictionTrie(PredictionTrie&& other) = delete;

    void insert(const std::string& word, unsigned int points = 1);
    void remove(const std::string& word);
    void clear();

    bool isPresented(const std::string& word) const;

    std::vector<MatchedPair> allWordsStartedWith(const std::string& wordBegin) const;
    std::string findBestMatch(const std::string& wordBegin) const;
    std::vector<std::string> findBestMatches(const std::string& wordBegin, unsigned count) const;
};

#endif // PREDICTIONTRIE_H
