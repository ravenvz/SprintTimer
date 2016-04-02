#include "TodoItem.h"
#include <regex>


TodoItem::TodoItem() {}

TodoItem::TodoItem(std::string name,
                   int estimatedPomodoros,
                   int spentPomodoros,
                   std::list<std::string> tags,
                   bool completed)
    : mName(name)
    , mEstimatedPomodoros(estimatedPomodoros)
    , mSpentPomodoros(spentPomodoros)
    , mTags(tags)
    , mCompleted(completed)
{
}

TodoItem::TodoItem(std::string encodedDescription)
{
    decodeDescription(std::move(encodedDescription));
    mCompleted = false;
    mSpentPomodoros = 0;
}

std::string TodoItem::name() const { return mName; }

int TodoItem::estimatedPomodoros() const { return mEstimatedPomodoros; }

int TodoItem::spentPomodoros() const { return mSpentPomodoros; }

std::list<std::string> TodoItem::tags() const { return mTags; }

std::string TodoItem::tagsAsString() const
{
    std::vector<std::string> res;
    std::transform(mTags.cbegin(),
                   mTags.cend(),
                   std::back_inserter(res),
                   [&](const auto& elem) {
                       std::string temp;
                       temp += tagPrefix;
                       temp += elem;
                       return temp;
                   });
    // for_each(
    //     res.begin(), res.end(), [&](auto& elem) { elem.insert(0, tagPrefix);
    //     });

    return StringUtils::join(res, " ");

    // QStringList hashedTags;
    // for (QString tag : mTags) {
    //     if (!tag.isEmpty()) {
    //         hashedTags.append(tag.prepend("#"));
    //     }
    // }
    // return hashedTags.join(" ");
}

bool TodoItem::isCompleted() const { return mCompleted; }

void TodoItem::setCompleted(bool completed) { mCompleted = completed; }

void TodoItem::setSpentPomodoros(int spentPomodoros)
{
    mSpentPomodoros = spentPomodoros;
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
    // result.append(
    //     QString(" %1/%2 ").arg(mSpentPomodoros).arg(mEstimatedPomodoros));
    return StringUtils::join(parts, " ");
    // return result.join(" ");
}

// std::string TodoItem::tagsAsHashedString() const
// {
//     QStringList hashedTags;
//     std::transform(mTags.cbegin(),
//                    mTags.cend(),
//                    std::back_inserter(hashedTags),
//                    [](const auto& tag) { return QString("#%1").arg(tag); });
//     return hashedTags.join(" ");
// }

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
