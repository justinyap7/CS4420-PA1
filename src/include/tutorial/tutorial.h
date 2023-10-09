
#include <string>
#include <unordered_map>
#include <vector>

namespace buzzdb {
namespace tutorial {

class CommandExecutor {
 public:
  CommandExecutor();

  /// Execute a command
  /// Return the appropriate output as a string
  std::string execute(const std::string command);

 private:
  bool isWordCharacter(char ch);
  std::string toLower(const std::string& str);
  void printWordOccurrences(
      const std::unordered_map<std::string, std::vector<int>>& wordMap);
  std::string tokenize(const std::string& input,
                       std::vector<std::string>& tokens);
  void indexWordsInFile(const std::string& filename, int& wordCount);
  std::string locateNthOccurrence(const std::string& word, int n);
  bool isValidWord(const std::string& word);
  std::string trim(const std::string& str);

  std::unordered_map<std::string, std::vector<int>> wordOccurrences;
};

}  // namespace tutorial
}  // namespace buzzdb
