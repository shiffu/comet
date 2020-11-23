#include <core/stringUtils.h>


namespace comet
{

    void StringUtils::split(std::vector<std::string>& result, const std::string& toSplit, const char token /*= ' '*/)
    {
        size_t startIdx{0};
        size_t currentLength{0}; 
        for(auto c : toSplit)
        {
            currentLength++;
            if (c == token)
            {
                result.push_back(toSplit.substr(startIdx, currentLength - 1));
                startIdx += currentLength;
                currentLength = 0;
            }
        }
        result.push_back(toSplit.substr(startIdx, currentLength));
    }

} // namespace comet