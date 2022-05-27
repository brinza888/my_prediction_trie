#include "PredictionTrie.h"

#include <algorithm>
#include <tuple>
#include <queue>

PredictionTrie::PredictionTrie()
{
    _root = new PredictionTrie::PredictionTrieNode;
    _root->count = 0u;
    _root->type = PredictionTrie::PredictionTrieNode::Type::Root;
}

PredictionTrie::~PredictionTrie()
{
    clear();
    delete _root;
}

void PredictionTrie::insert(const std::string& word, unsigned int points)
{
    auto* current = _root;
    for (auto letter : word)
    {
        auto foundIt = current->children.find(letter);
        if (foundIt == current->children.end())
        {
            auto [it, _] = current->children.emplace(letter, new PredictionTrieNode);
            foundIt = it;
        }

        current = foundIt->second;
    }

    current->type = PredictionTrie::PredictionTrieNode::Type::Leaf;
    current->count += points;
}

void PredictionTrie::remove(const std::string& word)
{
    PredictionTrieNode* node = find(word);
    if (node != nullptr) {
        node->type = PredictionTrieNode::Type::Regular;
    }
}

void PredictionTrie::clear() {
    PredictionTrieNode* current;
    std::queue<PredictionTrieNode*> deleteQueue;
    for (auto&& [letter, node]: _root->children) {
        deleteQueue.push(node);
    }
    _root->children.clear();
    while (!deleteQueue.empty()) {
        current = deleteQueue.front();
        deleteQueue.pop();
        if (current == nullptr) {
            continue;
        }
        for (auto&& [letter, node]: current->children) {
            deleteQueue.push(node);
        }
        delete current;
    }
}

PredictionTrie::PredictionTrieNode* PredictionTrie::find(const std::string& word) {
    return const_cast<PredictionTrieNode*>(const_cast<const PredictionTrie*>(this)->find(word));
}

const PredictionTrie::PredictionTrieNode* PredictionTrie::find(const std::string& word) const
{
    auto* current = _root;
    for (auto letter : word)
    {
        auto foundIt = current->children.find(letter);

        if (foundIt == current->children.end())
        {
            return nullptr;
        }

        current = foundIt->second;
    }

    return current;
}

bool PredictionTrie::isPresented(const std::string& word) const
{
    auto* found = find(word);

    return found && found->type == PredictionTrie::PredictionTrieNode::Type::Leaf;
}

std::vector<MatchedPair> PredictionTrie::allWordsStartedWith(const std::string& wordBegin) const
{
    auto* found = find(wordBegin);
    if (!found)
    {
        return {};
    }

    std::vector<MatchedPair> result;
    collectAllWordsStartedWith(wordBegin, found->children, result);

    return result;
}

void PredictionTrie::collectAllWordsStartedWith(
    const std::string& wordPart,
    const std::unordered_map<char, PredictionTrieNode*>& letterLayer,
    std::vector<MatchedPair>& result) const
{
    for (auto&& [letter, node] : letterLayer)
    {
        std::string word = wordPart + letter;
        if (node->type == PredictionTrie::PredictionTrieNode::Type::Leaf)
        {
            result.push_back(std::make_tuple(word, node->count));
        }

        collectAllWordsStartedWith(word, node->children, result);
    }
}

std::string PredictionTrie::findBestMatch(const std::string& wordBegin) const
{
    return findBestMatches(wordBegin, 1).front();
}

std::vector<std::string> PredictionTrie::findBestMatches(
            const std::string& wordBegin,
            unsigned count) const
{
    auto allWords = allWordsStartedWith(wordBegin);

    count = allWords.size() < count ? allWords.size() : count;

    std::partial_sort(
        std::begin(allWords),
        allWords.begin() + count,
        std::end(allWords),
        [](const auto& first, const auto& second) {
            return std::get<1>(first) > std::get<1>(second);
        }
    );

    std::vector<std::string> result;
    for (size_t i =  0; i < count; i++) {
        result.push_back(std::get<0>(allWords[i]));
    }

    return result;
}
