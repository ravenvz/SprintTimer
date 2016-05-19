#include "TodoItem.h"
#include <regex>


// static
BoostUUIDGenerator TodoItem::generator;

TodoItem::TodoItem(std::string name,
                   int estimatedPomodoros,
                   int spentPomodoros,
                   std::list<std::string> tags,
                   bool completed)
    : mName(name)
    , mEstimatedPomodoros(estimatedPomodoros)
    , mSpentPomodoros(spentPomodoros)
    , mUuid{generator.generateUUID()}
    , mTags(tags)
    , mCompleted(completed)
{
}

TodoItem::TodoItem(std::string name,
                   int estimatedPomodoros,
                   int spentPomodoros,
                   const std::string& uuid,
                   std::list<std::string> tags,
                   bool completed)
    : mName(name)
    , mEstimatedPomodoros(estimatedPomodoros)
    , mSpentPomodoros(spentPomodoros)
    , mUuid{uuid}
    , mTags(tags)
    , mCompleted(completed)
{
}

TodoItem::TodoItem(std::string encodedDescription)
    : mUuid{generator.generateUUID()}
{
    decodeDescription(std::move(encodedDescription));
}

std::string TodoItem::tagPrefix = std::string{"#"};

std::string TodoItem::estimatedPrefix = std::string{"*"};

std::string TodoItem::name() const { return mName; }

bool TodoItem::isCompleted() const { return mCompleted; }

int TodoItem::estimatedPomodoros() const { return mEstimatedPomodoros; }

int TodoItem::spentPomodoros() const { return mSpentPomodoros; }

std::string TodoItem::uuid() const { return mUuid; }

std::list<std::string> TodoItem::tags() const { return mTags; }

void TodoItem::setName(const std::string& name) { mName = name; }

void TodoItem::setCompleted(bool completed) { mCompleted = completed; }

void TodoItem::setEstimatedPomodoros(int estimatedPomodoros)
{
    mEstimatedPomodoros = estimatedPomodoros;
}

void TodoItem::setTags(const std::list<std::string>& newTags)
{
    mTags = newTags;
}

void TodoItem::setSpentPomodoros(int spentPomodoros)
{
    mSpentPomodoros = spentPomodoros;
}

std::string TodoItem::tagsAsString() const
{
    std::list<std::string> res{mTags.begin(), mTags.end()};
    std::for_each(res.begin(), res.end(), [&](auto& tag) {
        tag.insert(0, tagPrefix);
        return tag;
    });

    return StringUtils::join(res.begin(), res.end(), " ");
}


std::string TodoItem::toString() const
{
    std::vector<std::string> parts;
    std::string result;
    parts.push_back(tagsAsString());
    parts.push_back(mName);
    parts.push_back(StringUtils::join(
        {std::to_string(mSpentPomodoros), std::to_string(mEstimatedPomodoros)},
        "/"));
    return StringUtils::join(parts, " ");
}

void TodoItem::decodeDescription(std::string&& encodedDescription)
{
    std::regex tagRegex{"^" + tagPrefix + R"(\w+)"};
    std::regex estimatedRegex{"^\\" + estimatedPrefix + R"(\w+)"};
    std::regex anyNonWhitespace{"\\S+"};

    std::sregex_iterator words_begin{
        encodedDescription.begin(), encodedDescription.end(), anyNonWhitespace};
    std::sregex_iterator words_end;

    std::vector<std::string> nameParts;

    for (auto it = words_begin; it != words_end; ++it) {
        std::string word{it->str()};
        if (std::regex_match(word, tagRegex)) {
            mTags.push_back(word.substr(1));
        }
        else if (std::regex_match(word, estimatedRegex)) {
            mEstimatedPomodoros = stoi(word.substr(1));
        }
        else {
            nameParts.push_back(word);
        }
    }

    mName = StringUtils::join(nameParts, " ");
}
